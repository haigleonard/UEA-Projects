// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "GridCell.h"
#include "CorridorEntrance.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "UObject/NoExportTypes.h"
#include "Corridor.generated.h"

/**
 * 
 */
UCLASS()
class PCGTESTING_API UCorridor : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	bool triggersSpawned = false;
	UPROPERTY()
	TArray<UGridCell *> cells;
	UPROPERTY()
	FString owner = "";
	//ATP_ThirdPersonCharacter *owner = nullptr;
	TArray<UGridCell *> entranceLocations;
	TArray<ACorridorEntrance *> entrances;

	UPROPERTY()
	TArray<ATP_ThirdPersonCharacter *> waitingGuards;

	//UPROPERTY()
		FCriticalSection mutex;

public:
	UCorridor();
	~UCorridor();

	void init(TArray<UGridCell *> cells, TArray<UGridCell *> entranceLocations);

	void spawnTriggers(UWorld *world, FActorSpawnParameters params);

	bool lock(ATP_ThirdPersonCharacter *newOwner);

	inline bool hasLock(FString check) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Corridor: %s, Curent owner: %s, Checked by: %s"), *GetName(), *owner, *check));
		return owner.Equals(check);
	}

	void unlock(ATP_ThirdPersonCharacter *oldOwner);
	
};
