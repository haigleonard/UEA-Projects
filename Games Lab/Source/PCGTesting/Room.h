// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridCell.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "CCTVPawn.h"
#include "LockerActor.h"
#include "RoomTriggerBox.h"
#include "CeilingLight.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Room.generated.h"

//rename?
USTRUCT()
struct F2dArray {
	GENERATED_BODY()
private:
	UPROPERTY()
		TArray<UGridCell *> row;

public:
	UGridCell * operator[] (int32 i) {
		return row[i];
	}

	void Add(UGridCell *cell) {
		row.Add(cell);
	}

	int32 Num() {
		return row.Num();
	}
};

/**
 * 
 */
UCLASS()
class PCGTESTING_API URoom : public UObject
{
	GENERATED_BODY()
	
private:
	enum class TunnelDirection { UP, DOWN, LEFT, RIGHT };//change to horiz/vertical
	enum class TunnelDirection2 { HORIZONTAL, VERTICAL };
	enum class DoorLocation { TOP, BOTTOM, LEFT, RIGHT };

	UPROPERTY()
		TArray<F2dArray> cells;
	//TArray<UGridCell *> doors;
	//TArray<TPair<DoorLocation, UGridCell *>> doors;
	TMap<UGridCell *, DoorLocation> doors;

	TArray<URoom *> existingPaths;

	UPROPERTY()
		FVector centre;
	UPROPERTY()
		float xDist;
	UPROPERTY()
		float yDist;

	UPROPERTY()
		TSubclassOf<ACCTVPawn> cctvBP;

	UPROPERTY()
		TSubclassOf<ALockerActor> lockerBP;

	UPROPERTY()
		TSubclassOf<ARoomTriggerBox> triggerBP;

	UPROPERTY()
		ACCTVPawn *cctvCamera = nullptr;
	UPROPERTY()
		ALockerActor *locker = nullptr;
	UPROPERTY()
		ARoomTriggerBox *triggerBox = nullptr;

	UPROPERTY()
		TSubclassOf<ACeilingLight> ceilingLightBP;
	UPROPERTY()
		ACeilingLight *ceilingLight = nullptr;

	DoorLocation getDoorLocation(int row, int col);

	TArray<TPair<TunnelDirection2, int32>> getMovement(UGridCell *from, UGridCell *to, DoorLocation toLoc);

	TMap<URoom *, float> roomDistances;//squ(euclidian dist), not A* dist

public:
	URoom();
	~URoom();

	void init(TArray<F2dArray> cells, float height);

	void makeTunnels(TArray<URoom *> otherRooms, TArray<F2dArray> *grid);

	void createTunnel(URoom *tunnelTo, TArray<F2dArray> *grid);

	URoom * getPatrolEnd(TArray<URoom *> &exclude, int numHops);

	bool isInside(FVector point);

	FVector getCentre();

	float getSqDistFromCentre(FVector point);

	UGridCell *getRandCell();

	void spawnSceneryAndCameras(UWorld *world, FActorSpawnParameters params, bool camera);

	void spawnTriggerBox(UWorld *world, FActorSpawnParameters params, int floorNum, int roomNum);

	void setVisibility(bool visible);

	inline FVector getPosition() {
		return centre;
	}

	inline TMap<UGridCell *, DoorLocation> getDoors() {
		return doors;
	}

	inline void addExistingPath(URoom *r) {
		existingPaths.Add(r);
	}

	inline UGridCell *getOriginCell() {
		return cells[0][0];
	}

	inline UGridCell *getOtherCorner() {
		return cells[cells.Num() - 1][cells[0].Num() - 1];
		//return cells[cells[0].Num() - 1][cells.Num() - 1];
	}

	inline float getDistToRoom(URoom *room) {
		if (roomDistances.Contains(room)) {
			return roomDistances[room];
		}
		else {
			return -1.0f;
		}
	}

	inline int getNumRows() {
		return cells.Num();
	}

	inline int getNumCols() {
		return cells[0].Num();
	}

	inline ACCTVPawn *getCamera() {
		return cctvCamera;
	}
};
