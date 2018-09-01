// Fill out your copyright notice in the Description page of Project Settings.

#include "CorridorEntrance.h"
#include "DrawDebugHelpers.h"
#include "Corridor.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

ACorridorEntrance::ACorridorEntrance() {
	OnActorBeginOverlap.AddDynamic(this, &ACorridorEntrance::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACorridorEntrance::OnOverlapEnd);
	SetActorRelativeScale3D(FVector(2.0f, 2.0f, 1.0f));
}

ACorridorEntrance::~ACorridorEntrance() {

}

void ACorridorEntrance::BeginPlay() {
	Super::BeginPlay();

	//DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
}


void ACorridorEntrance::OnOverlapBegin(class AActor *overlappedActor, class AActor *otherActor) {
	ATP_ThirdPersonCharacter *guard = Cast<ATP_ThirdPersonCharacter>(otherActor);

	//printFString("Overlapped actor: %s", *overlappedActor->GetName());

	if (guard != nullptr && !guard->isPaused() && !parent->hasLock(guard->GetName())) {
		if (parent->lock(guard)) {
			//guard successfully locked corridor access
		}
		else if(guard->respectsLocks()) {
			//guard must wait for corridor access
			guard->pause(parent);
		}
	}
}

void ACorridorEntrance::OnOverlapEnd(class AActor *overlappedActor, class AActor *otherActor) {
	ATP_ThirdPersonCharacter *guard = Cast<ATP_ThirdPersonCharacter>(otherActor);

	if (guard != nullptr && !guard->isPaused() && parent->hasLock(guard->GetName())) {
		parent->unlock(guard);
	}
}