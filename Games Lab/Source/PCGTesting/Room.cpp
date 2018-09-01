// Fill out your copyright notice in the Description page of Project Settings.

#include "Room.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

URoom::URoom() {
	static ConstructorHelpers::FObjectFinder<UClass> cctvBPAsset(TEXT("Blueprint'/Game/Geometry/CCTV/CCTV_BP.CCTV_BP_C'"));
	cctvBP = cctvBPAsset.Object;

	static ConstructorHelpers::FObjectFinder<UClass> lockerBPAsset(TEXT("Blueprint'/Game/Mannequin/Assets/LockerBP.LockerBP_C'"));
	lockerBP = lockerBPAsset.Object;

	static ConstructorHelpers::FObjectFinder<UClass> triggerBPAsset(TEXT("Blueprint'/Game/RoomTrigger_BP.RoomTrigger_BP_C'"));
	triggerBP = triggerBPAsset.Object;

	static ConstructorHelpers::FObjectFinder<UClass> ceilingLightBPAsset(TEXT("Blueprint'/Game/Geometry/CeilingLight/CeilingLight_BP.CeilingLight_BP_C'"));
	ceilingLightBP = ceilingLightBPAsset.Object;
}

URoom::~URoom() {

}

void URoom::init(TArray<F2dArray> cells, float height) {
	this->cells = cells;

	int rows = cells.Num();
	int cols = cells[0].Num();

	//find co-ordinates of room centre point
	FVector corner1 = cells[0][0]->getPosition();
	FVector corner2 = cells[rows - 1][cols - 1]->getPosition();

	this->xDist = corner2.X - corner1.X;
	this->yDist = corner2.Y - corner1.Y;

	centre = FVector(corner1.X + (xDist / 2.0f), corner1.Y + (yDist / 2.0f), height);

	//position a door along each wall
	//rand()%(max-min + 1) + min;
	//could just do rand() % rows; then dont sub 1???
	//really needs to be between 1 and rows - 2???
	//re-arrange these to be consistent with getDoorLocation(...)

	//max = rows - 2, min = 1

	//make doors array of pair of doorlocation, cell
	//bring door location back to room class, remove null_location
	//have some error check for top->top etc

	//place a door along left wall
	int doorPos = rand() % (rows - 2) + 1;
	doors.Add(cells[doorPos][0], DoorLocation::LEFT);

	//place a door along right wall
	doorPos = rand() % (rows - 2) + 1;
	doors.Add(cells[doorPos][cols - 1], DoorLocation::RIGHT);

	//place a door along bottom wall
	doorPos = rand() % (cols - 2) + 1;
	doors.Add(cells[0][doorPos], DoorLocation::BOTTOM);

	//place a door along top wall
	doorPos = rand() % (cols - 2) + 1;
	doors.Add(cells[rows - 1][doorPos], DoorLocation::TOP);
}

void URoom::makeTunnels(TArray<URoom *> otherRooms, TArray<F2dArray> *grid) {
	//start with one tunnel
	URoom *closest = nullptr;
	float distance = TNumericLimits<float>::Max();



	for (URoom *r : otherRooms) {
		if (r != this && !existingPaths.Contains(r)) {
			FVector otherPos = r->getPosition();
			float d = pow((float)otherPos.X - centre.X, 2) + pow((float)otherPos.Y - centre.Y, 2);

			roomDistances.Add(r, d);

			if (d < distance) {
				distance = d;
				closest = r;
			}
		}
	}

	if (closest != nullptr) {
		createTunnel(closest, grid);
	}

	for (TPair<UGridCell *, DoorLocation> pair : doors) {
		pair.Key->setType(UGridCell::CellType::ROOM);
	}
}

void URoom::createTunnel(URoom *tunnelTo, TArray<F2dArray> *grid) {
	//find closest pair of doors between the two rooms
	UGridCell *tunnelFrom = nullptr;
	UGridCell *closestDoor = nullptr;
	DoorLocation closestLoc;
	float distance = TNumericLimits<float>::Max();

	TMap<UGridCell *, DoorLocation> otherDoors = tunnelTo->getDoors();
	//for (UGridCell *d : doors) {
	for (TPair<UGridCell *, DoorLocation> pair : doors) {
		UGridCell * d = pair.Key;
		FVector thisPos = d->getPosition();

		//for (UGridCell *od : otherDoors) {
		for (TPair<UGridCell *, DoorLocation> otherPair : otherDoors) {
			UGridCell *od = otherPair.Key;
			FVector otherPos = od->getPosition();

			float dist = pow(otherPos.X - thisPos.X, 2) + pow(otherPos.Y - thisPos.Y, 2);

			if (dist < distance) {
				distance = dist;
				tunnelFrom = d;
				closestDoor = od;
				closestLoc = otherPair.Value;
			}
		}
	}

	//create the tunnel
	if (tunnelFrom != nullptr && closestDoor != nullptr) {
		TArray<TPair<TunnelDirection2, int32>> movement = getMovement(tunnelFrom, closestDoor, closestLoc);
		int r = tunnelFrom->getRow();
		int c = tunnelFrom->getCol();
		TArray<UGridCell *> corridorCells;

		for (TPair<TunnelDirection2, int32> p : movement) {
			TunnelDirection2 dir = p.Key;
			int numSteps = p.Value;

			int abNumSteps = abs(numSteps);

			for (int i = 0; i < abNumSteps; i++) {
				if (dir == TunnelDirection2::VERTICAL) {
					if (numSteps > 0) {
						r++;
					}
					else {
						r--;
					}
				}
				else {
					if (numSteps > 0) {
						c++;
					}
					else {
						c--;
					}
				}

				//add each grid cell to array for corridor init

				if (r >= 0 && c >= 0 && r < grid->Num() && c < grid[0].Num()) {
					(*grid)[r][c]->setType(UGridCell::CellType::ROOM);
				}


			}
		}

		//so the other room won't try to tunnel back to this one (because a path already exists)
		tunnelTo->addExistingPath(this);

		addExistingPath(tunnelTo);
	}
}

TArray<TPair<URoom::TunnelDirection2, int32>> URoom::getMovement(UGridCell *from, UGridCell *to, DoorLocation toLoc) {
	TArray<TPair<TunnelDirection2, int32>> movement;

	//find the locations of the start and end of the tunnel
	DoorLocation fromLoc = doors[from];//from->getDoorLocation();//getDoorLocation(from->getRow(), from->getCol());
									   //DoorLocation toLoc = to->getDoorLocation();//getDoorLocation(to->getRow(), to->getCol());
	int verticalDist = to->getRow() - from->getRow();
	int horizDist = to->getCol() - from->getCol();

	//move two cells directly away from this room first
	switch (fromLoc) {
	case DoorLocation::TOP: {
		if (!((toLoc == DoorLocation::LEFT || toLoc == DoorLocation::RIGHT) && verticalDist < 2)) {
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, 2));
			verticalDist -= 2;
		}

		if (toLoc == DoorLocation::BOTTOM || toLoc == DoorLocation::TOP) {
			//move horizontally first
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, horizDist));

			//now move up
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, verticalDist));
		}
		else if (toLoc == DoorLocation::LEFT || toLoc == DoorLocation::RIGHT) {
			//move up first
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, verticalDist));

			//now move horizontally
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, horizDist));
		}

		break;
	}
	case DoorLocation::BOTTOM: {
		if (!((toLoc == DoorLocation::LEFT || toLoc == DoorLocation::RIGHT) && verticalDist > -2)) {
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, -2));
			verticalDist += 2;
		}

		if (toLoc == DoorLocation::TOP || toLoc == DoorLocation::BOTTOM) {
			//move horizontally first
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, horizDist));

			//now move down
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, verticalDist));//*-1
		}
		else if (toLoc == DoorLocation::LEFT || toLoc == DoorLocation::RIGHT) {
			//move down first
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, verticalDist));//*-1

																								   //now move horizontally
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, horizDist));
		}

		break;
	}
	case DoorLocation::LEFT: {
		if (!((toLoc == DoorLocation::TOP || toLoc == DoorLocation::BOTTOM) && horizDist > -2)) {
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, -2));
			horizDist += 2;
		}

		if (toLoc == DoorLocation::RIGHT || toLoc == DoorLocation::LEFT) {
			//move vertically first
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, verticalDist));

			//now move left
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, horizDist));//*-1
		}
		else if (toLoc == DoorLocation::TOP || toLoc == DoorLocation::BOTTOM) {
			//move left first
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, horizDist));//*-1

																								  //now move vertically
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, verticalDist));
		}

		break;
	}
	case DoorLocation::RIGHT: {
		if (!((toLoc == DoorLocation::TOP || toLoc == DoorLocation::BOTTOM) && horizDist < 2)) {
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, 2));
			horizDist -= 2;
		}

		if (toLoc == DoorLocation::LEFT || toLoc == DoorLocation::RIGHT) {
			//move vertically first
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, verticalDist));

			//now move right
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, horizDist));
		}
		else if (toLoc == DoorLocation::TOP || toLoc == DoorLocation::BOTTOM) {
			//move right first
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::HORIZONTAL, horizDist));

			//now move vertically
			movement.Add(TPair<TunnelDirection2, int32>(TunnelDirection2::VERTICAL, verticalDist));
		}

		break;
	}
	}

	return movement;
}

URoom * URoom::getPatrolEnd(TArray<URoom *> &exclude, int numHops) {
	//rand()%(max-min + 1) + min;
	TArray<URoom *> tmp = existingPaths;
	for (URoom * r : exclude) {
		if (tmp.Contains(r)) {
			tmp.Remove(r);
		}
	}

	if (tmp.Num() == 0) {
		return this;
	}
	else {
		int selected = rand() % (tmp.Num());
		URoom * selectedRoom = tmp[selected];

		if (numHops <= 0) {
			return selectedRoom;
		}
		else {
			if (!exclude.Contains(this)) {
				exclude.Emplace(this);
			}

			return selectedRoom->getPatrolEnd(exclude, numHops - 1);
		}
	}
}

bool URoom::isInside(FVector point) {
	return (point.X < (centre.X + xDist / 2.0f)) && (point.X >(centre.X - xDist / 2.0f)) && (point.Y < (centre.Y + yDist / 2.0f)) && (point.Y >(centre.Y - yDist / 2.0f));
}

FVector URoom::getCentre() {
	return centre;
}

float URoom::getSqDistFromCentre(FVector point) {
	FVector tmp = centre - point;
	return tmp.X * tmp.X + tmp.Y * tmp.Y;
}

UGridCell *URoom::getRandCell() {
	int r = rand() % (cells.Num() - 2) + 1;
	int c = rand() % (cells[0].Num() - 2) + 1;

	return cells[r][c];
}

void URoom::spawnSceneryAndCameras(UWorld *world, FActorSpawnParameters params, bool camera) {
	//spawn camera
	TArray<UGridCell *> validCells;
	int numR = cells.Num();
	int numC = cells[0].Num();

	for (int r = 1; r < numR - 1; r++) {
		UGridCell *tmp = cells[r][0];
		if (tmp->validCameraCell()) {
			validCells.Emplace(tmp);
		}

		tmp = cells[r][numC - 1];
		if (tmp->validCameraCell()) {
			validCells.Emplace(tmp);
		}
	}
	for (int c = 1; c < numC - 1; c++) {
		UGridCell *tmp = cells[0][c];
		if (tmp->validCameraCell()) {
			validCells.Emplace(tmp);
		}

		tmp = cells[numR - 1][c];
		if (tmp->validCameraCell()) {
			validCells.Emplace(tmp);
		}
	}

	//if validCells.Num() > 0...
	//UGridCell *cameraCell = validCells[rand() % ((validCells.Num() - 1) - 0) + 0];

	TArray<UGridCell *> nondn;

	if (camera && validCells.Num() > 0) {
		UGridCell *cameraCell = validCells[rand() % ((validCells.Num() - 1) + 1 - 0) + 0];
		validCells.Remove(cameraCell);

		//really messy + inefficient
		nondn = cameraCell->getNonDiagNeighbours();
		FVector cameraLoc;
		for (UGridCell *nei : nondn) {
			if (nei->getType() == UGridCell::CellType::WALL) {
				cameraLoc = nei->getPosition() + cameraCell->getPosition();
				cameraLoc /= 2.0f;
			}
		}

		cameraLoc.Z += 220.0f;

		cctvCamera = world->SpawnActor<ACCTVPawn>(cctvBP, cameraLoc, FRotator::ZeroRotator, params);
		cctvCamera->init(this);
	}


	//spawn locker
	if (validCells.Num() > 0) {
		//rand() % (max_number + 1 - minimum_number) + minimum_number
		UGridCell *lockerCell = validCells[rand() % ((validCells.Num() - 1) + 1 - 0) + 0];	//this crashes sometimes (when validCells.Num() == 1 because divide by zero
		nondn = lockerCell->getNonDiagNeighbours();
		FRotator lockerRot;
		for (UGridCell *nei : nondn) {
			if (nei->getType() == UGridCell::CellType::WALL) {
				lockerRot = FRotationMatrix::MakeFromX(lockerCell->getPosition() - nei->getPosition()).Rotator();
				lockerRot.Yaw -= 90.0f;
			}
		}

		locker = world->SpawnActor<ALockerActor>(lockerBP, lockerCell->getPosition(), lockerRot, params);
	}

	//spawn ceiling light
	FVector lightCentre = FVector(centre.X, centre.Y, centre.Z + 300.0f);
	ceilingLight = world->SpawnActor<ACeilingLight>(ceilingLightBP, lightCentre, FRotator::ZeroRotator, params);
}

void URoom::spawnTriggerBox(UWorld *world, FActorSpawnParameters params, int floorNum, int roomNum) {
	triggerBox = world->SpawnActor<ARoomTriggerBox>(triggerBP, centre, FRotator::ZeroRotator, params);
	triggerBox->init((cells[0].Num() * 170.0f / 80.0f), (cells.Num() * 170.0f / 80.0f), floorNum, roomNum);
}

void URoom::setVisibility(bool visible) {
	if (cctvCamera != nullptr) {
		cctvCamera->SetActorHiddenInGame(!visible);
	}
	if (locker != nullptr) {
		locker->SetActorHiddenInGame(!visible);
	}
}