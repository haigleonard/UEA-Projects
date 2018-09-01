// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "GameFramework/Pawn.h"
#include "CCTVPawn.generated.h"

class URoom;
UCLASS()
class PCGTESTING_API ACCTVPawn : public APawn
{
	GENERATED_BODY()

private:
	enum class Alertness { GREEN, YELLOW, RED };

	Alertness state = Alertness::GREEN;

	UPROPERTY()
		URoom *containingRoom = nullptr;
	UPROPERTY()
		int level;
	UPROPERTY()
		float rotationAmmount = 0.5f;
	UPROPERTY()
		FVector eyePos;
	UPROPERTY(BlueprintReadWrite, Replicated, meta = (AllowPrivateAccess = "true"))
		float sTime = 0.0f;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float lightRed = 0.0f;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float lightGreen = 1.0f;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FRotator oldRot;
	UPROPERTY(BlueprintReadWrite, Replicated, meta = (AllowPrivateAccess = "true"))
		bool seesPlayer;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool hacked = false;

	UPROPERTY()
		FTimerHandle cameraUpdateTimer;
	UPROPERTY()
		float everyNSecs = 1.0f / 30.0f;

	UFUNCTION(BlueprintCallable)
		void changeDirection();

	UFUNCTION(BlueprintCallable)
		void setEyePosition(FVector ep);

public:
	// Sets default values for this pawn's properties
	ACCTVPawn();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void init(URoom *containingRoom);

	void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent)
		void updateCCTV();

	virtual void updateCCTV_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		ATP_ThirdPersonCharacter *getClosestGuard(int cameraLevel);

	virtual ATP_ThirdPersonCharacter *getClosestGuard_Implementation(int cameraLevel) { return nullptr; };

	UFUNCTION(BlueprintCallable)
		void hackCamera();

	UFUNCTION(BlueprintCallable)
		bool hackedCheck();
	
};
