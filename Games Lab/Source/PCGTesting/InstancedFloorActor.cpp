// Fill out your copyright notice in the Description page of Project Settings.

#include "InstancedFloorActor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/MaterialInterface.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

// Sets default values
AInstancedFloorActor::AInstancedFloorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInstancedFloorActor::BeginPlay()
{
	Super::BeginPlay();

}

void AInstancedFloorActor::OnConstruction(const FTransform & Transform) {
	Super::OnConstruction(Transform);

}

// Called every frame
void AInstancedFloorActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

