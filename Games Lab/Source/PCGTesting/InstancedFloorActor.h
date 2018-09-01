// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "InstancedFloorActor.generated.h"

//rename
//if materials dont appear/show as default material, need to set use with ism components in material editor
UCLASS()
class PCGTESTING_API AInstancedFloorActor : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform &Transform) override;

public:
	// Sets default values for this actor's properties
	AInstancedFloorActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline void setFloorNum(int floorNum) {
		this->floorNum = floorNum;
	}

	UFUNCTION(BlueprintNativeEvent)
		void addFloorInstance(FTransform transform);

	virtual void addFloorInstance_Implementation(FTransform transform) {};

	UFUNCTION(BlueprintNativeEvent)
		void addRoomInstance(FTransform transform);

	virtual void addRoomInstance_Implementation(FTransform transform) {};

	UFUNCTION(BlueprintNativeEvent)
		void addWallInstance(FTransform transform);

	virtual void addWallInstance_Implementation(FTransform transform) {};

	UFUNCTION(BlueprintNativeEvent)
		void addCeilingInstance(FTransform transform);

	virtual void addCeilingInstance_Implementation(FTransform transform) {};

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int floorNum;
};
