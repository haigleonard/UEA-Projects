// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAIController.h"
#include "Runtime/AIModule/Classes/Navigation/CrowdFollowingComponent.h"

AMyAIController::AMyAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {
	SetGenericTeamId(FGenericTeamId(1));
}


