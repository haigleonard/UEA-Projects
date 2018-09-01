// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Engine/TriggerBox.h"
#include "RoomTriggerBox.generated.h"

/**
 * 
 */
class URoom;
class ACCTVPawn;
UCLASS()
class PCGTESTING_API ARoomTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
private:

	UPROPERTY()
		float width;
	UPROPERTY()
		float length;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int floorNum;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int roomNum;

protected:

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;


public:

	ARoomTriggerBox();
	~ARoomTriggerBox();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	inline void init(float width, float length, int floorNum, int roomNum) {
		this->width = width;
		this->length = length;
		this->floorNum = floorNum;
		this->roomNum = roomNum;
		SetActorRelativeScale3D(FVector(width, length, 1.0f));
	}	
};
