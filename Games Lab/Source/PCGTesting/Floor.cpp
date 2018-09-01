// Fill out your copyright notice in the Description page of Project Settings.

#include "Floor.h"
#include "AStar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"

UFloor::UFloor() {
	static ConstructorHelpers::FObjectFinder<UClass> ifaBPAsset(TEXT("Blueprint'/Game/InstancedFloorActor_BP.InstancedFloorActor_BP_C'"));
	ifaBP = ifaBPAsset.Object;

	static ConstructorHelpers::FObjectFinder<UClass> guardBPAsset(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter_C'"));
	guardBP = guardBPAsset.Object;

	static ConstructorHelpers::FObjectFinder<UClass> fogBPAsset(TEXT("Blueprint'/Game/FogOfWar/MaskingActor_BP.MaskingActor_BP_C'"));
	fogBP = fogBPAsset.Object;
}

UFloor::~UFloor() {

}

void UFloor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFloor, ifa);
}

void UFloor::init(int numRows, int numCols, int level, float cellWidth, float wallHeight) {
	this->numRows = numRows;
	this->numCols = numCols;
	this->level = level;
	this->cellWidth = cellWidth;
	this->wallHeight = wallHeight;

	//FVector cellPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector cellPosition = FVector(0.0f, 0.0f, static_cast<float>(level) * wallHeight);
	if (level > 0) {
		cellPosition.Z += 0.1f;
	}
	//^ will be * 300.0f for 3m wall
	for (int i = 0; i < numRows; i++) {

		F2dArray row;
		for (int j = 0; j < numCols; j++) {
			UGridCell *cell = NewObject<UGridCell>();
			cell->init(i, j);
			cell->setPosition(cellPosition);
			row.Add(cell);

			cellPosition.X += cellWidth;
		}

		grid.Add(row);

		cellPosition.X = 0.0f;
		cellPosition.Y += cellWidth;
	}

	calcNeighbours();
}

void UFloor::generateRooms(int numRooms, URoom *ladderBase) {
	int originRow = ladderBase->getOriginCell()->getRow();
	int originCol = ladderBase->getOriginCell()->getCol();
	int numRows = ladderBase->getNumRows();
	int numCols = ladderBase->getNumCols();

	URoom *room = NewObject<URoom>();
	TArray<F2dArray> roomGrid;
	for (int r = originRow; r < originRow + numRows; r++) {

		F2dArray row;
		for (int c = originCol; c < originCol + numCols; c++) {
			grid[r][c]->setType(UGridCell::CellType::ROOM);
			grid[r][c]->setInRoom();
			row.Add(grid[r][c]);
		}

		roomGrid.Add(row);
	}

	room->init(roomGrid, static_cast<float>(level) * 300.0f);
	room->getOriginCell()->setType(UGridCell::CellType::LADDER_TOP);

	startRoom = room;

	rooms.Add(room);
	actualRoomCount++;

	generateRooms(numRooms - 1);
}

void UFloor::generateRooms(int numRooms) {
	for (int i = 0; i < numRooms; i++) {
		//have a counter to break infinite loops, not needed for now
		int maxAttempts = 30;
		int currentAttempts = 0;
		//int currentAttempts = 0;
		bool success = false;
		while (!success && currentAttempts < maxAttempts) {
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString("placing room"));
			int r = rand() % (numRows - 1 + 1) + 1;
			int c = rand() % (numCols - 1 + 1) + 1;

			int length = rand() % 6 + 4;
			int width = rand() % 6 + 4;
			//int length = rand() % (6 - 4 + 1) + 4;
			//int width = rand() % (6 - 4 + 1) + 4;
			//int width = 2;
			//int length = 2;

			bool valid = true;

			for (int a = r - 1; a < r + width + 1; a++) {
				for (int b = c - 1; b < c + length + 1; b++) {
					if (!valid || a >= numCols - 1 || b >= numRows - 1 || (a >= 0 && b >= 0 && grid[a][b]->getType() != UGridCell::CellType::WALL)) {
						valid = false;
					}
				}
			}

			if (valid) {
				URoom *room = NewObject<URoom>();
				TArray<F2dArray> roomGrid;
				for (int a = r; a < r + width; a++) {

					F2dArray row;
					for (int b = c; b < c + length; b++) {
						grid[a][b]->setType(UGridCell::CellType::ROOM);
						grid[a][b]->setInRoom();
						row.Add(grid[a][b]);
					}

					roomGrid.Add(row);
				}
				room->init(roomGrid, static_cast<float>(level) * 300.0f);
				success = true;

				rooms.Add(room);
				actualRoomCount++;
			}
			else {
				currentAttempts++;
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, FString::Printf(TEXT("Room count: %d"), actualRoomCount));
	//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, FString::Printf(TEXT("Most attempts for one room: %d"), currentAttempts));

	determineStartRoom();
	digTunnels();
	resolveOrphans();
}

void UFloor::determineStartRoom() {
	if (level == 0) {
		float closestDist = TNumericLimits<float>::Max();
		for (URoom *room : rooms) {
			FVector position = room->getPosition();
			float dist = pow(position.X, 2) + pow(position.Y, 2);
			if (dist < closestDist) {
				startRoom = room;
				closestDist = dist;
			}
		}
	}
}

void UFloor::resolveOrphans() {
	//A* from start room to every other room, group into rooms with path and orphaned
	int longestPath = 0;
	bool done = false;
	while (!done) {
		TArray<URoom *> havePath;
		TArray<URoom *> orphaned;
		UGridCell *startOrigin = startRoom->getOriginCell();

		for (URoom *room : rooms) {
			if (room != startRoom) {
				UGridCell *roomOrigin = room->getOriginCell();
				TArray<UGridCell *> path = UAStar::aStar(startOrigin, roomOrigin);

				if (path.Num() == 0) {
					orphaned.Add(room);
				}
				else {
					havePath.Add(room);

					if (path.Num() > longestPath) {
						longestPath = path.Num();
						furthest = room;
					}
				}
			}
		}

		if (orphaned.Num() == 0) {
			done = true;
		}
		else {
			//find the closest pair of rooms from the two groups
			float closestDist = TNumericLimits<float>::Max();
			TPair<URoom *, URoom *> closestPair;
			for (URoom *room : havePath) {
				for (URoom *oRoom : orphaned) {
					float dist = room->getDistToRoom(oRoom);
					if (dist > 0.0f && dist < closestDist) {
						closestDist = dist;
						closestPair = TPair<URoom *, URoom *>(room, oRoom);
					}
				}
			}

			closestPair.Key->createTunnel(closestPair.Value, &grid);
		}
	}

	generateCorridors();

	TArray<UGridCell *> path = UAStar::aStar(startRoom->getOriginCell(), furthest->getOriginCell());
	for (UGridCell * cell : path) {
		//cell->setType(UGridCell::CellType::FLOOR);
	}
	//furthest->getOriginCell()->setType(UGridCell::CellType::FLOOR);
	if (level == 0) {
		//startRoom->getOriginCell()->setType(UGridCell::CellType::FLOOR);
		furthest->getOriginCell()->setType(UGridCell::CellType::LADDER_BOTTOM);
	}
	else {
		startRoom->getOriginCell()->setType(UGridCell::CellType::LADDER_TOP);
	}
}

void UFloor::processCell(UGridCell *cell, TArray<UGridCell *> *corridorArray, TArray<UGridCell *> *entrances) {
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString("corridor"));

	if (!corridorArray->Contains(cell)) {
		corridorArray->Emplace(cell);
		cell->setCorridor();
		cell->setType(UGridCell::CellType::FLOOR);

		int num = cell->getNumNeighbours();

		for (int i = 0; i < num; i++) {
			UGridCell *c2 = cell->getNeighbour(i);
			bool cutsRoom = c2->isOneCellWide();
			if (c2->getType() == UGridCell::CellType::ROOM && !c2->inRoom() && !c2->isCorridor() && !cutsRoom) {
				processCell(c2, corridorArray, entrances);
			}
			else if (cutsRoom && !UGridCell::isDiagonal(cell, c2)) {
				entrances->Emplace(c2);
			}
			else if (c2->inRoom() && !cutsRoom) {
				entrances->Emplace(c2);
			}
		}
	}
}

void UFloor::generateCorridors() {
	for (int r = 0; r < numRows; r++) {
		for (int c = 0; c < numCols; c++) {
			UGridCell *cell = grid[r][c];
			bool cutsRoom = cell->isOneCellWide();
			if ((cell->getType() == UGridCell::CellType::ROOM || cell->getType() == UGridCell::CellType::LADDER_BOTTOM) && !cell->inRoom() && !cell->isCorridor() && !cutsRoom) {
				TArray<UGridCell *> corridorArray;
				TArray<UGridCell *> entrances;
				processCell(cell, &corridorArray, &entrances);

				//do stuff with corridor array
				UCorridor *corr = NewObject<UCorridor>();
				corr->init(corridorArray, entrances);
				corridors.Emplace(corr);
			}
		}

	}
}

void UFloor::placeMeshes(UWorld *world, FActorSpawnParameters params) {
	if (world) {
		ifa = world->SpawnActor<AInstancedFloorActor>(ifaBP, FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator, params);
		ifa->setFloorNum(level);

		for (int i = 0; i < numRows; i++) {

			for (int j = 0; j < numCols; j++) {

				FTransform t(grid[i][j]->getPosition());
				t.SetScale3D(FVector(cellWidth / 100.0f, cellWidth / 100.0f, 0.01f));

				switch (grid[i][j]->getType()) {
				case UGridCell::CellType::FLOOR: {
					ifa->addFloorInstance(t);
					ifa->addCeilingInstance(t);
					break;
				}
				case UGridCell::CellType::ROOM: {
					ifa->addRoomInstance(t);
					ifa->addCeilingInstance(t);
					break;
				}
				case UGridCell::CellType::WALL: {
					t.SetScale3D(FVector(cellWidth / 100.0f, cellWidth / 100.0f, wallHeight / 100.0f));//wall height will be about 300... (3.0f)
					ifa->addWallInstance(t);
					break;
				}
				case UGridCell::CellType::LADDER_BOTTOM: {
					ifa->addRoomInstance(t);
					break;
				}
				case UGridCell::CellType::LADDER_TOP: {
					ifa->addCeilingInstance(t);
					break;
				}
				}
			}
		}
	}
}

void UFloor::spawnCorridorTriggers(UWorld *world, FActorSpawnParameters params) {
	for (UCorridor * corr : corridors) {
		corr->spawnTriggers(world, params);
	}
}

void UFloor::spawnCameras(UWorld *world, FActorSpawnParameters params) {
	for (URoom * r : rooms) {
		r->spawnSceneryAndCameras(world, params, r != startRoom);
	}
}

void UFloor::spawnGuards(int numGuards, UWorld *world, FActorSpawnParameters params) {
	for (int i = 0; i < numGuards; i++) {
		URoom *firstRoom = nullptr;
		while (firstRoom == nullptr || firstRoom == startRoom || firstRoom == furthest) {
			//int roomNum = rand() % 25;//25 = numrooms
			int roomNum = rand() % actualRoomCount;
			firstRoom = getRoom(roomNum);
		}

		TArray<URoom *> exclude;
		exclude.Emplace(firstRoom);
		exclude.Emplace(startRoom);
		exclude.Emplace(furthest);

		int numHops = rand() % 2;
		URoom *secondRoom = firstRoom->getPatrolEnd(exclude, numHops);

		//obviously could infinitely loop atm
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString("hi"));
		UGridCell *tmp1 = nullptr;
		while (tmp1 == nullptr) {
			//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString("tmp1"));
			tmp1 = firstRoom->getRandCell();
			if (tmp1->getGuardStart()) {
				tmp1 = nullptr;
			}
			else {
				tmp1->setGuardStart();
			}
		}

		UGridCell *tmp2 = nullptr;
		while (tmp2 == nullptr) {
			//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString("tmp2"));
			tmp2 = secondRoom->getRandCell();
			if (tmp2->getGuardStart() || tmp2 == tmp1) {
				tmp2 = nullptr;
			}
			else {
				tmp2->setGuardStart();
			}
		}

		FVector spawnLoc = tmp1->getPosition();
		spawnLoc.Z += 97.0f;

		ATP_ThirdPersonCharacter *tmpGuard = world->SpawnActor<ATP_ThirdPersonCharacter>(guardBP, spawnLoc, FRotator::ZeroRotator, params);
		tmpGuard->setPatrolPath(tmp1->getPosition(), tmp2->getPosition(), this);

		guards.Emplace(tmpGuard);
	}
}

void UFloor::calcNeighbours() {
	for (int r = 0; r < numRows; r++) {
		for (int c = 0; c < numCols; c++) {
			UGridCell *cell = grid[r][c];
			TArray<UGridCell *> neighbours;

			if (c - 1 >= 0) {
				neighbours.Add(grid[r][c - 1]);
			}

			if (r - 1 >= 0) {
				neighbours.Add(grid[r - 1][c]);
			}

			if (c + 1 < numCols) {
				neighbours.Add(grid[r][c + 1]);
			}

			if (r + 1 < numRows) {
				neighbours.Add(grid[r + 1][c]);
			}

			if ((c - 1 >= 0) && (r - 1 >= 0)) {
				neighbours.Add(grid[r - 1][c - 1]);
			}

			if ((c + 1 < numCols) && (r + 1 < numRows)) {
				neighbours.Add(grid[r + 1][c + 1]);
			}

			if ((c + 1 < numCols) && (r - 1 >= 0)) {
				neighbours.Add(grid[r - 1][c + 1]);
			}

			if ((c - 1 >= 0) && (r + 1 < numRows)) {
				neighbours.Add(grid[r + 1][c - 1]);
			}

			cell->setNeighbours(neighbours);
		}
	}
}

URoom *UFloor::getContainingRoom(FVector point) {
	for (URoom *r : rooms) {
		if (r->isInside(point)) {
			return r;
		}
	}

	return nullptr;
}

URoom *UFloor::getClosestRoom(FVector point) {
	float closestDist = 0.0f;
	URoom *closest = nullptr;

	for (URoom *r : rooms) {
		if (closest == nullptr) {
			closest = r;
			closestDist = r->getSqDistFromCentre(point);
		}
		else if (r->getSqDistFromCentre(point) < closestDist) {
			closest = r;
		}
	}

	return closest;
}

void UFloor::setPlayerLastSeen(FVector playerLastSeen) {
	this->playerLastSeen = playerLastSeen;
}

void UFloor::setVisibility(bool vis) {
	if (visible != vis) {
		visible = vis;

		//loop over rooms and set visibility
		for (URoom *r : rooms) {
			r->setVisibility(visible);
		}

		//loop over guards and set visibility
		for (ATP_ThirdPersonCharacter *guard : guards) {
			guard->SetActorHiddenInGame(!visible);
		}
	}
}

void UFloor::spawnFog(UWorld *world, FActorSpawnParameters params) {
	fog = world->SpawnActor<AFogActor>(fogBP, FVector(0.0f, 0.0f, wallHeight * (level + 1) + 50.0f), FRotator::ZeroRotator, params);
	fog->SetActorScale3D(FVector((cellWidth * numRows) / 100.0f, (cellWidth * numCols) / 100.0f, 1.0f));
	fog->init(level);
}