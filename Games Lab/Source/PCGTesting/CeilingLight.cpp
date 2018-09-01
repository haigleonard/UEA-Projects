// Fill out your copyright notice in the Description page of Project Settings.

#include "CeilingLight.h"


// Sets default values
ACeilingLight::ACeilingLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACeilingLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACeilingLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

