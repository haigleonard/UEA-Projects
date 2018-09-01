// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogActor.generated.h"

UCLASS()
class PCGTESTING_API AFogActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFogActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline void setVisibility(bool vis) {
		this->visible = vis;
	}

	inline void init(int floor) {
		floorNumber = floor;
	}

	UFUNCTION(BlueprintCallable)
		bool getVisibility();

private:
	UPROPERTY()
		bool visible = true;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int floorNumber;

};
