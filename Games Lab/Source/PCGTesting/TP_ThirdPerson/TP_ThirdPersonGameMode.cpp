// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonGameMode.h"
#include "TP_ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/GameFramework/SpectatorPawn.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

ATP_ThirdPersonGameMode::ATP_ThirdPersonGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/

	Timer = StartingTimer;
	Minutes = Timer / 60;
}

void ATP_ThirdPersonGameMode::updateTimer(float tTimer) {
	Timer += tTimer;
	Minutes = Timer / 60;

	float temp = Minutes - (Timer / 60);

	Seconds = (temp *= -100);

	Seconds = (Seconds / 100) * 60;

	timeString = FString::FromInt(Minutes) + ":" + FString::FromInt(Seconds);//don't need to do this every tick, just when calling get, same with most of above
	//move some stuff to get methods of game state
	if (GEngine) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("timer tick %f"), Timer));
	}
}

void ATP_ThirdPersonGameMode::Tick(float DeltaTime) {
	updateTimer(-DeltaTime * (0.0016f * (600 / StartingTimer)) * (StartingTimer));
	Super::Tick(DeltaTime);
}

float ATP_ThirdPersonGameMode::getTime() {
	return Timer;
}

FString ATP_ThirdPersonGameMode::getTimeString() {
	return timeString;
}

void ATP_ThirdPersonGameMode::BeginPlay() {
	//player = GetWorld()->SpawnActor<ATP_ThirdPersonCharacter>(ATP_ThirdPersonCharacter::StaticClass(), spawnLoc, FRotator::ZeroRotator, spawnInfo);
	DefaultPawnClass = ASpectatorPawn::StaticClass();
	if (GEngine) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("game mode"));
	}

	FActorSpawnParameters spawnInfo;
	spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnInfo.Owner = this;
	spawnInfo.Instigator = NULL;
	spawnInfo.bDeferConstruction = false;
	//startPoint = GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), FVector(100.0f, 100.0f, 500.0f), FRotator::ZeroRotator, spawnInfo);
	
	Super::BeginPlay();
}

void ATP_ThirdPersonGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}