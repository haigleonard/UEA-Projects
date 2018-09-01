// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelActor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include <time.h>

//uint64 ALevelActor::randSeed = 0;
bool ALevelActor::serverDone = false;

// Sets default values
ALevelActor::ALevelActor() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass> playerBPAsset(TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter_C'"));
	playerBP = playerBPAsset.Object;

	
}

void ALevelActor::Spawn() {
	if (HasAuthority()) {
		if (randSeed == 0) {
			randSeed = static_cast<int>(time(NULL));
		}
		//set rand seed in bp (game state)
		setSeedInGameState(randSeed);
	}
	else {
		//get rand seed from bp (game state)
		randSeed = getSeedFromGameState();
	}

	srand(randSeed);

	groundFloor = NewObject<UFloor>();

	//Grid cells are 100x100 (Unreal cube, plane, etc.)

	//Generate the grid (32x32), start at 0,0 and work out
	groundFloor->init(numRows, numCols, 0, cellWidth, wallHeight);

	//randomly place rooms
	groundFloor->generateRooms(25);

	//find the start room

	//make tunnels between rooms

	//A* from start room to every other room, group into rooms with path and orphaned

	//pass GridCells pointers to the instanced static mesh actor that will be drawn in that cell?
	//code handling creating a new instance can be handled in GridCell


	firstFloor = NewObject<UFloor>();
	firstFloor->init(numRows, numCols, 1, cellWidth, wallHeight);
	firstFloor->generateRooms(25, groundFloor->getEndRoom());

	FActorSpawnParameters params;
	params.Owner = this;
	UWorld *world = GetWorld();

	groundFloor->placeMeshes(world, params);
	firstFloor->placeMeshes(world, params);

	if (!HasAuthority()) {
		groundFloor->placeRoomTriggers(world, params);
		firstFloor->placeRoomTriggers(world, params);

		groundFloor->spawnFog(world, params);
		firstFloor->spawnFog(world, params);
	}

	if (!serverDone && HasAuthority()) {
		groundFloor->spawnCorridorTriggers(world, params);
		firstFloor->spawnCorridorTriggers(world, params);
		//spawn the player character
		FActorSpawnParameters spawnInfo;
		spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//obviously won't really be AlwaysSpawn
		FVector spawnLoc = firstFloor->getStartPoint();
		//FVector spawnLoc = groundFloor->getStartPoint();
		//FVector spawnLoc = FVector(150.0f, 150.0f, 1000.0f);
		spawnLoc.Z += 97.0f;
		//spawnLoc.Z += 97.0f * 3.0f;
		//need to add scale??
		AFP_FirstPersonCharacter::resetPlayerStatus();
		//spawnLoc.Z += 97.0f  * 10.0f;
		player = GetWorld()->SpawnActor<AFP_FirstPersonCharacter>(playerBP, spawnLoc, FRotator::ZeroRotator, spawnInfo);
		player->setladNei(getLadderNei());

		//place CCTV cameras
		groundFloor->spawnCameras(world, params);
		firstFloor->spawnCameras(world, params);

		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//obviously won't really be AlwaysSpawn
		int numGuards = 20;
		groundFloor->spawnGuards(numGuards, world, params);
		firstFloor->spawnGuards(numGuards, world, params);

		serverDone = true;
	}
}

// Called when the game starts or when spawned
void ALevelActor::BeginPlay() {
	Spawn();
	Super::BeginPlay();

	if (HasAuthority()) {
		//Move and resize navmesh, navmesh is 200x200 units so halve scale compared to cube
		//abs nav size
		FVector navSize = FVector(numRows * cellWidth, numCols * cellWidth, 2 * wallHeight);//walls are 180 tall for now..., navmesh bounds need to be slightly larger, might have been radius...
		FVector navScale = FVector(navSize.X / 200.0f, navSize.Y / 200.0f, navSize.Z / 200.0f);//formula should stay the same regardless of actual size
																							   //missing out the gap between floors but that won't be there in the end
		FVector navCentre = FVector((float)numRows * cellWidth / 2.0f, (float)numCols * cellWidth / 2.0f, wallHeight);//not sure on z yet, not 0...

		nav2->SetActorLocation(navCentre);
		nav2->SetActorScale3D(navScale);
		UNavigationSystem *navSys = UNavigationSystem::GetCurrent(GetWorld());
		navSys->OnNavigationBoundsUpdated(nav2);
		//navMesh->SetActorTransform(FTransform(navCentre));//combine transformations
	}
}

void ALevelActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	serverDone = false;
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void ALevelActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (HasAuthority() && player != nullptr) {
		if (player->GetActorLocation().Z < 300.0f) {
			groundFloor->setVisibility(true);
			firstFloor->setVisibility(false);
		}
		else {
			groundFloor->setVisibility(false);
			firstFloor->setVisibility(true);
		}
	}
}

FVector ALevelActor::getStartPoint() {
	return FVector(0.0f, 0.0f, 100.0f);
	//FVector spawnLoc = groundFloor->getStartPoint();
	//spawnLoc.Z = 97.0f;
	//return spawnLoc;
}

FVector ALevelActor::getEndPoint() {
	return firstFloor->getEndRoom()->getOriginCell()->getPosition();
}

FVector ALevelActor::getfirstFloorEndPoint() {
	return firstFloor->getStartPoint();
}

float ALevelActor::getLadderRot() {
	UGridCell *cell = firstFloor->getStartCell();

	int num = cell->getNumNeighbours();
	for (int i = 0; i < num; i++)
	{
		if (cell->getNeighbour(i)->getType() == UGridCell::CellType::WALL) {
			if (i == 1) {
				return 90.00;
			}
			if (i == 2) {
				return 0.00;
			}
			if (i == 3) {
				return 270.00;
			}
			if (i == 4) {
				return 180.00;
			}
		}
	}
	return 90.00;
}

FVector ALevelActor::getLadderNei() {
	UGridCell *cell = firstFloor->getStartCell();

	TArray<UGridCell *> cells = cell->getNonDiagNeighbours();

	for (UGridCell *cell : cells) {
		if(cell->getType() != UGridCell::CellType::WALL)
			GEngine->AddOnScreenDebugMessage(-1, 300.0f, FColor::Yellow, FString::Printf(TEXT("name: %s"), *cell->getPosition().ToString()));
		FVector  W1 = cell->getPosition();
		W1.X += cellWidth / 2.0f;
		W1.Y += cellWidth / 2.0f;
		return W1;
			//W1.X =  (cellWidth / 2);
		//	W1.Y =  (cellWidth / 2);
			//return cell->getPosition() + W1;
	}
	/*
	int num = cell->getNumNeighbours();
	for (int i = 0; i < num; i++)
	{
		if (cell->getNeighbour(i)->getType() != UGridCell::CellType::WALL) {

			return cell->getPosition();

		}
	}*/
	return FVector(0, 0, 0);
	
}

ACCTVPawn *ALevelActor::getCCTVFromRoom(int roomNum, int floorNum) {
	if (floorNum == 0) {
		return groundFloor->getRoom(roomNum)->getCamera();
	}
	else if (floorNum == 1) {
		return firstFloor->getRoom(roomNum)->getCamera();
	}
	else {
		return nullptr;
	}
}