// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "InstancedFloorActor.h"
//#include "GridCell.h"
//#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "FP_FirstPerson/FP_FirstPersonCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavMeshBoundsVolume.h"
#include "Floor.h"
#include "AStar.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelActor.generated.h"



UCLASS()
class PCGTESTING_API ALevelActor : public AActor {
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		ANavMeshBoundsVolume *navMesh = nullptr;

	UPROPERTY()
	UFloor *groundFloor = nullptr;
	UPROPERTY()
	UFloor *firstFloor = nullptr;

	UPROPERTY()
	AFP_FirstPersonCharacter *player = nullptr;
	UPROPERTY()
	APlayerStart *startPoint = nullptr;

	static bool serverDone;

	UPROPERTY()
		TSubclassOf<AFP_FirstPersonCharacter> playerBP;

	//UPROPERTY()
	float cellWidth = 170.0f;
	//UPROPERTY()
	float wallHeight = 300.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Sets default values for this actor's properties
	ALevelActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		int randSeed = 0;

	UPROPERTY(EditAnywhere)
		ANavMeshBoundsVolume *nav2 = nullptr;

	UPROPERTY(EditAnywhere)
		int32 numRows = 64;

	UPROPERTY(EditAnywhere)
		int32 numCols = 64;

	UFUNCTION(BlueprintNativeEvent)
		void setSeedInGameState(int seed);

	virtual void setSeedInGameState_Implementation(int seed) {};

	UFUNCTION(BlueprintNativeEvent)
		int getSeedFromGameState();

	virtual int getSeedFromGameState_Implementation() { return 0; };


	UFUNCTION(BlueprintCallable)
		void Spawn();

	UFUNCTION(BlueprintCallable)
		FVector getStartPoint();

	UFUNCTION(BlueprintCallable)
		FVector getEndPoint();

	UFUNCTION(BlueprintCallable)
		FVector getfirstFloorEndPoint();

	UFUNCTION(BlueprintCallable)
		float getLadderRot();

	UFUNCTION(BlueprintCallable, Category = "Action")
		FVector getLadderNei();

	UFUNCTION(BlueprintCallable, Category = "CCTV")
		ACCTVPawn *getCCTVFromRoom(int roomNum, int floorNum);
};
