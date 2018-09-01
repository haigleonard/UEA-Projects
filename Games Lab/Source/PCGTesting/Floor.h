// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Room.h"
#include "Corridor.h"
#include "InstancedFloorActor.h"
#include "FogActor.h"
//#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"//already in corridor
#include "Runtime/Engine/Classes/Engine/World.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Floor.generated.h"

/**
 * 
 */
UCLASS()
class PCGTESTING_API UFloor : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	int numRows;
	UPROPERTY()
	int numCols;
	UPROPERTY()
	int level;
	UPROPERTY()
	float cellWidth;
	UPROPERTY()
	float wallHeight;
	UPROPERTY()
		int actualRoomCount = 0;
	UPROPERTY()
	TArray<F2dArray> grid;
	UPROPERTY()
	TArray<URoom *> rooms;
	UPROPERTY()
	TArray<UCorridor *> corridors;

	UPROPERTY()
	URoom *startRoom = nullptr;
	UPROPERTY()
	URoom *furthest = nullptr;

	UPROPERTY()
		FVector playerLastSeen;

	UPROPERTY(Replicated)
	AInstancedFloorActor *ifa = nullptr;

	void calcNeighbours();
	void determineStartRoom();

	UPROPERTY()
		TSubclassOf<AInstancedFloorActor> ifaBP;

	UPROPERTY()
		TSubclassOf<ATP_ThirdPersonCharacter> guardBP;

	UPROPERTY()
		TArray<ATP_ThirdPersonCharacter *> guards;

	UPROPERTY()
		TSubclassOf<AFogActor> fogBP;

	UPROPERTY()
		AFogActor *fog = nullptr;

	UPROPERTY()
		bool visible = true;
	UPROPERTY()
		bool oldVis = true;

	inline void digTunnels() {
		for (URoom *r : rooms) {
			r->makeTunnels(rooms, &grid);
		}
	}

	void resolveOrphans();
	void processCell(UGridCell *cell, TArray<UGridCell *> *corridorArray, TArray<UGridCell *> *entrances);
	void generateCorridors();

public:
	UFloor();
	~UFloor();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void init(int numRows, int numCols, int level, float cellWidth, float wallHeight);

	//just make private and do everything in init? rename init to generate?
	void generateRooms(int numRooms, URoom *ladderBase);
	void generateRooms(int numRooms);


	void placeMeshes(UWorld *world, FActorSpawnParameters params);
	
	void spawnCorridorTriggers(UWorld *world, FActorSpawnParameters params);

	void spawnCameras(UWorld *world, FActorSpawnParameters params);

	void spawnGuards(int numGuards, UWorld *world, FActorSpawnParameters params);

	URoom *getContainingRoom(FVector point);

	URoom *getClosestRoom(FVector point);

	void setVisibility(bool vis);

	void spawnFog(UWorld *world, FActorSpawnParameters params);

	inline FVector getStartPoint() {
		return startRoom->getOriginCell()->getPosition();
	}

	inline URoom *getRoom(int roomNum) {
		return rooms[roomNum];
	}

	inline URoom *getStartRoom() {
		return startRoom;
	}

	inline UGridCell *getStartCell() {
		return startRoom->getOriginCell();
	}

	inline URoom * getEndRoom() {
		return furthest;
	}

	void setPlayerLastSeen(FVector playerLastSeen);

	inline void placeRoomTriggers(UWorld *world, FActorSpawnParameters params) {
		int num = rooms.Num();
		for (int i = 0; i < num; i++) {
			rooms[i]->spawnTriggerBox(world, params, level, i);
		}
	}

	inline FVector getPlayerLastSeen() {
		return this->playerLastSeen;
	}

	inline int getLevel() {
		return this->level;
	}
};
