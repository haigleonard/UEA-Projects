// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Floor.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "MyAIController.h"
#include "TP_ThirdPersonCharacter.generated.h"

class UFloor;
class UCorridor;

UENUM(BlueprintType)
enum class EGuardState : uint8 {
	GS_Patrol		UMETA(DisplayName = "Patrol"),
	GS_Talking		UMETA(DisplayName = "Talking"),
	GS_Return		UMETA(DisplayName = "Return"),
	GS_Suspicious	UMETA(DisplayName = "Suspicious"),
	GS_Alerted		UMETA(DisplayName = "Alerted"),
	GS_AlertedPerm	UMETA(DisplayName = "AlertedPerm"),
	GS_HighAlert	UMETA(DisplayName = "HighAlert")
};

UCLASS(config=Game)
class ATP_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ATP_ThirdPersonCharacter();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

	void setPatrolPath(FVector one, FVector two, UFloor *patrolFloor);

	inline FVector getTwo() {
		return two;
	}

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	bool chokeAnim = false;
	bool dead = false;
	int timer = 0;
	bool animComplete = false;
	UFUNCTION(BlueprintCallable, Category = "Action")
		bool ChokeAnimation();
	UFUNCTION(BlueprintCallable, Category = "Action")
		bool getanimComplete();
	//setChokeAniamtiontoTrue
	UFUNCTION(BlueprintNativeEvent)
		void setChoke(bool choke);
	virtual void setChoke_Implementation(bool choke);

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface		

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void pause(UCorridor *waitFor);
	void resume();
	inline bool isPaused() {
		return paused;
	}

	inline bool respectsLocks() {
		return respectCorridorLocks;
	}

	AActor *standingOn();

	UFUNCTION(BlueprintNativeEvent)
		void setFOV(float newFOV);

	virtual void setFOV_Implementation(float newFOV) {};

	UFUNCTION(BlueprintNativeEvent)
		FVector getPlayerLocFromBP();

	virtual FVector getPlayerLocFromBP_Implementation();

	UFUNCTION(BlueprintCallable)
		int getFloor();
	bool firing = false;
	float bulletCountdown = 0.0;
	void suspiciousByCamera();

	void alertedByCamera();

private:
	UPROPERTY()
	bool start = false;
	UPROPERTY()
	bool start2 = false;
	UPROPERTY()
	FVector one;
	UPROPERTY()
	FVector two;
	//UPROPERTY()
		FVector *currentlyMovingTo = nullptr;

	UPROPERTY()
		AAIController *aiC = nullptr;
	bool shooting = false;
	UPROPERTY()
	UFloor *patrolFloor = nullptr;
	UPROPERTY(BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = "true"))
		int floor;

	UPROPERTY()
	bool paused = false;
	UPROPERTY()
	float MaxUseDistance = 200;

	UPROPERTY()
		UCorridor *waitFor = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EGuardState guardState;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector individualLastSighting;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector indivOldSighting;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float timeSeenFor = 0.0f;
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool getshooting();
	UPROPERTY()
		bool respectCorridorLocks = true;
	UPROPERTY()
		float waitTimer = 0.0f;
	UPROPERTY()
		FRotator oldRot;
	UPROPERTY()
		FVector indivPlayerLastPosition;
	UPROPERTY()
		FVector indivPlayerOldPosition;

	UPROPERTY()
		float weaponRange = 500.0f;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool firingFlag = false;

	UPROPERTY()
		bool manuallyAlerted;

	UFUNCTION()
		void handlePatrol(float deltaTime);
	UFUNCTION()
		void handleTalking(float deltaTime);
	UFUNCTION()
		void handleReturn(float deltaTime);
	UFUNCTION()
		void handleSuspicious(float deltaTime);
	UFUNCTION()
		void handleAlerted(float deltaTime);
	UFUNCTION()
		void handleAlertedPerm(float deltaTime);
	UFUNCTION()
		void handleHighAlert(float deltaTime);

	UFUNCTION()
		void updateFOV(float newFOV);
};

