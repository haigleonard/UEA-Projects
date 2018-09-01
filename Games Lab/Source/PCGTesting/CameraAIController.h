// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CameraAIController.generated.h"

/**
 * 
 */
UCLASS()
class PCGTESTING_API ACameraAIController : public AAIController
{
	GENERATED_BODY()
	
	
	ACameraAIController(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool seesPlayer;
	
};
