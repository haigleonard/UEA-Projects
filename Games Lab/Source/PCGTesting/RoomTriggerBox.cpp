// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomTriggerBox.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Room.h"

ARoomTriggerBox::ARoomTriggerBox() {
	PrimaryActorTick.bCanEverTick = true;
}

ARoomTriggerBox::~ARoomTriggerBox() {

}

void ARoomTriggerBox::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ARoomTriggerBox, collisionStatus);
}

void ARoomTriggerBox::BeginPlay() {
	Super::BeginPlay();
	//DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString("trigger boxes"));
}

void ARoomTriggerBox::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
