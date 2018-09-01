// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelActor.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PCGTESTING_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

private:
	UPROPERTY(Replicated)
		int randSeed;
	UPROPERTY(Replicated)
	float time;
	UPROPERTY(Replicated)
	FString timeString;
	UPROPERTY(Replicated)
		TArray<bool> floorVisibilities;
	UPROPERTY(Replicated)
		FVector p1Location;
	UPROPERTY(Replicated)
		FVector p1UpVector;
	UPROPERTY(Replicated)
		FTransform p1Transform;
	UPROPERTY(Replicated)
		int p1Floor;
	UPROPERTY(Replicated)
		float p1Health;
	UPROPERTY()
		ALevelActor *pcgLevelBPRef = nullptr;

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Seed")
		void setRandSeed(int seed);

	UFUNCTION(BlueprintCallable, Category = "Seed")
		int getRandSeed();

	UFUNCTION(BlueprintCallable, Category = "Timer")
		void setTime(float newTime, FString newTimeString);

	UFUNCTION(BlueprintCallable, Category = "Timer")
		float getTime();

	UFUNCTION(BlueprintCallable, Category = "Timer")
		FString getTimeString();

	UFUNCTION(BlueprintCallable, Category = "Visibility")
		void setFloorVisibility(int floorNum, bool visible);

	UFUNCTION(BlueprintCallable, Category = "Visibility")
		bool getFloorVisibility(int floorNum);

	UFUNCTION(BlueprintCallable, Category = "Player 1 loc")
		void setP1Data(FVector p1Loc, FVector p1Up, FTransform p1TForm, int floor, float health);

	UFUNCTION(BlueprintCallable, Category = "Player 1 loc")
		FVector getP1Location();

	UFUNCTION(BlueprintCallable, Category = "Player 1 loc")
		FVector getP1UpVector();

	UFUNCTION(BlueprintCallable, Category = "Player 1 loc")
		FTransform getP1Transform();

	UFUNCTION(BlueprintCallable, Category = "Player 1 loc")
		int getP1Floor();

	UFUNCTION(BlueprintCallable, Category = "Player 1 loc")
		float getP1Health();
};
