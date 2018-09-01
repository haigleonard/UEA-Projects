// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CorridorEntrance.generated.h"

/**
 * 
 */
class UCorridor;
UCLASS()
class PCGTESTING_API ACorridorEntrance : public ATriggerBox
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UCorridor *parent;
	//FCriticalSection mutex;


protected:
	virtual void BeginPlay() override;


public:
	ACorridorEntrance();
	~ACorridorEntrance();

	inline void setParent(UCorridor *parent) {
		this->parent = parent;
	}

	UFUNCTION()
		void OnOverlapBegin(class AActor *overlappedActor, class AActor *otherActor);

	UFUNCTION()
		void OnOverlapEnd(class AActor *overlappedActor, class AActor *otherActor);
	
};
