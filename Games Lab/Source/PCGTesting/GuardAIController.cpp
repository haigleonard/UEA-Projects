// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardAIController.h"
#include "Runtime/AIModule/Classes/Navigation/CrowdFollowingComponent.h"

AGuardAIController::AGuardAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {
	SetGenericTeamId(FGenericTeamId(1));
}
