// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TP_ThirdPersonGameMode.generated.h"

UCLASS(minimalapi)
class ATP_ThirdPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()


private:
	//timer stuff...
	void updateTimer(float tTimer);
	UPROPERTY()
	float StartingTimer = 600.0f;
	UPROPERTY()
	int Minutes;
	UPROPERTY()
	float Seconds;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float Timer;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FString timeString;


	UPROPERTY()
	APlayerStart *startPoint = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	ATP_ThirdPersonGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Timer")
		float getTime();
	UFUNCTION(BlueprintCallable, Category = "Timer")
		FString getTimeString();
};



