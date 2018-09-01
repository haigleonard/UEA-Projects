// Fill out your copyright notice in the Description page of Project Settings.

#include "FogActor.h"


// Sets default values
AFogActor::AFogActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFogActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFogActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AFogActor::getVisibility() {
	return visible;
}