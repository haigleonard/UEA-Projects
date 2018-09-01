// Fill out your copyright notice in the Description page of Project Settings.

#include "Corridor.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

UCorridor::UCorridor() {

}

UCorridor::~UCorridor() {

}

void UCorridor::init(TArray<UGridCell *> cells, TArray<UGridCell *> entranceLocations) {
	this->cells = cells;

	this->entranceLocations = entranceLocations;
}

void UCorridor::spawnTriggers(UWorld *world, FActorSpawnParameters params) {
	if (!triggersSpawned) {
		for (UGridCell *cell : entranceLocations) {
			entrances.Emplace(world->SpawnActor<ACorridorEntrance>(ACorridorEntrance::StaticClass(), cell->getPosition(), FRotator::ZeroRotator, params));
		}

		for (ACorridorEntrance *ent : entrances) {
			ent->setParent(this);
		}

		triggersSpawned = true;
	}
}

bool UCorridor::lock(ATP_ThirdPersonCharacter *newOwner) {
	//printFString("Checking guard: %s", *newOwner);
	//waitingGuards.Num();

	//could re-add list of waiting guards and if num>0 only allow [0] to lock, see unlock section for more

	FScopeLock lock(&mutex);

	if (owner.Equals("")) {
		if (GEngine) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Corridor: %s, Locked by: %s"), *GetName(), *newOwner->GetName()));
		}
		owner = newOwner->GetName();
		
		return true;
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Guard: %s, Failed to lock: %s"), *newOwner->GetName(), *GetName()));
		//printFString("Adding guard to waiting: %s", *newOwner);
		return false;
	}
}

void UCorridor::unlock(ATP_ThirdPersonCharacter *oldOwner) {
	if (owner.Equals(oldOwner->GetName())) {

		bool inside = false;
		UGridCell *inCell = nullptr;
		for (UGridCell *cell : cells) {
			if (!inside) {
				inside = cell->insideCell(oldOwner->GetActorLocation());
				inCell = cell;
			}
		}

		if (!inside) {
			owner = "";

			if (GEngine) {
				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Corridor: %s, Unlocked by: %s"), *GetName(), *oldOwner->GetName()));
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Unlocked, Corr cell: %s,    guard loc: %s"), *inCell->getPosition().ToString(), *guardLocation.ToString()));
			}

			//remove the guard from waiting guard list, will need to mutex this method too
			//FScopeLock lock(&mutex);
			/*if (waitingGuards.Num() > 0 && lock(waitingGuards[0])) {
				waitingGuards[0]->resume();
				waitingGuards.RemoveAt(0);
			}*/
		}
	}
}