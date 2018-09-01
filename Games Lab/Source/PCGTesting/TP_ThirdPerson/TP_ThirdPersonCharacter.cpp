// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Floor.h"
#include "Corridor.h"
#include "Runtime/AIModule/Classes/Perception/AISense.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionTypes.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "FP_FirstPerson/FP_FirstPersonCharacter.h"
#include "Net/UnrealNetwork.h"

#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);//was 96

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 93.75f;
	//GetCharacterMovement()->MaxWalkSpeed = 50.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	/*CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	*/

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> playerSkeletalMeshAsset(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Meshes/TwinBlast"));//SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'
	if (playerSkeletalMeshAsset.Object != NULL) {
		GetMesh()->SetSkeletalMesh(playerSkeletalMeshAsset.Object);
	}

	GetMesh()->AddLocalOffset(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * -1.0f));
	GetMesh()->AddLocalRotation(FRotator(0.0f, -90.0f, 0.0f));

	//Blueprint'/Game/ThirdPersonCPP/Blueprints/BP_GuardAIController.BP_GuardAIController'
	//change this to blueprint
	static ConstructorHelpers::FObjectFinder<UClass> guardAIBPAsset(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/BP_GuardAIController.BP_GuardAIController_C'"));
	AIControllerClass = guardAIBPAsset.Object;
	//AIControllerClass = AMyAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UClass> animBP(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Meshes/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));//AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'
	if (animBP.Object != NULL) {
		GetMesh()->SetAnimInstanceClass(animBP.Object);
		GetMesh()->InitAnim(true);
	}
}

void ATP_ThirdPersonCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATP_ThirdPersonCharacter, floor);
}

void ATP_ThirdPersonCharacter::setPatrolPath(FVector one, FVector two, UFloor *patrolFloor) {
	this->patrolFloor = patrolFloor;
	this->one = one;
	this->two = two;
	//next = this->two;
	this->currentlyMovingTo = &this->two;
	start = true;
	start2 = false;
	this->floor = patrolFloor->getLevel();
}

void ATP_ThirdPersonCharacter::pause(UCorridor *waitFor) {
	this->waitFor = waitFor;
	paused = true;
	start = false;
	URoom *r = patrolFloor->getContainingRoom(GetActorLocation());
	if (r != nullptr) {
		aiC->MoveToLocation(r->getCentre());
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Guard: %s, should be moving to: %s"), *GetName(), *r->getCentre().ToString()));
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Guard: %s, doesn't know what room its in"), *GetName()));
		r = patrolFloor->getClosestRoom(GetActorLocation());
		if (r != nullptr) {
			aiC->MoveToLocation(r->getCentre());
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 1500.f, FColor::Green, FString::Printf(TEXT("Guard: %s, doesn't know what room its in"), *GetName()));
		}
	}
}

void ATP_ThirdPersonCharacter::resume() {
	paused = false;
	start = true;
	aiC->MoveToLocation(*currentlyMovingTo);
}

void ATP_ThirdPersonCharacter::BeginPlay() {
	SpawnDefaultController();
	aiC = Cast<AAIController>(GetController());
	guardState = EGuardState::GS_Patrol;
	Super::BeginPlay();
}

void ATP_ThirdPersonCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void ATP_ThirdPersonCharacter::suspiciousByCamera() {
	individualLastSighting = getPlayerLocFromBP();
	guardState = EGuardState::GS_Suspicious;
	manuallyAlerted = true;
	respectCorridorLocks = false;
	GetCharacterMovement()->MaxWalkSpeed = 70.0f;
	updateFOV(60.0f);
	if (paused) {
		resume();
	}
	aiC->MoveToLocation(individualLastSighting);
}

void ATP_ThirdPersonCharacter::alertedByCamera() {
	individualLastSighting = getPlayerLocFromBP();
	guardState = EGuardState::GS_Alerted;
	manuallyAlerted = true;
	GetCharacterMovement()->MaxWalkSpeed = 90.0f;
	respectCorridorLocks = false;
	updateFOV(80.0f);
	waitTimer = 0.0f;
	indivPlayerLastPosition = getPlayerLocFromBP();
}

void ATP_ThirdPersonCharacter::updateFOV(float newFOV) {
	FAISenseID id = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	if (id.IsValid()) {
		auto config = aiC->GetPerceptionComponent()->GetSenseConfig(id);
		if (config != nullptr) {
			auto configSight = Cast<UAISenseConfig_Sight>(config);
			configSight->PeripheralVisionAngleDegrees = newFOV;

			aiC->GetPerceptionComponent()->RequestStimuliListenerUpdate();
		}
	}
}

void ATP_ThirdPersonCharacter::handlePatrol(float deltaTime) {
	if (timeSeenFor > 0.0f) {
		guardState = EGuardState::GS_Suspicious;
		respectCorridorLocks = false;
		GetCharacterMovement()->MaxWalkSpeed = 70.0f;
		updateFOV(60.0f);
		if (paused) {
			resume();
		}
		aiC->MoveToLocation(individualLastSighting);
	}
	else {
		if (start) {
			if (!start2) {
				aiC->MoveToLocation(two);
				start2 = true;
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString("ai start"));
			}
			else if (waitTimer > 0.0f) {
				waitTimer -= deltaTime;
				if (waitTimer <= 0.0f) {
					waitTimer = 0.0f;
					aiC->MoveToLocation(*currentlyMovingTo);
				}
			}
			else {
				FVector distToOne = GetActorLocation() - one;
				FVector distToTwo = GetActorLocation() - two;

				if (distToOne.X < 0.0f) {
					distToOne.X *= -1.0f;
				}
				if (distToOne.Y < 0.0f) {
					distToOne.Y *= -1.0f;
				}
				if (distToTwo.X < 0.0f) {
					distToTwo.X *= -1.0f;
				}
				if (distToTwo.Y < 0.0f) {
					distToTwo.Y *= -1.0f;
				}

				URoom *r = patrolFloor->getContainingRoom(GetActorLocation());
				if (distToOne.X < 100.0f && distToOne.Y < 100.0f && aiC->GetMoveStatus() == EPathFollowingStatus::Idle) {
					//aiC->MoveToLocation(two);
					currentlyMovingTo = &two;
					waitTimer = 5.0f;

					if (r != nullptr) {
						FRotator faceCentre = FRotationMatrix::MakeFromX(r->getCentre() - GetActorLocation()).Rotator();

						faceCentre.Pitch = 0.0f;
						faceCentre.Roll = 0.0f;
						SetActorRotation(faceCentre);
					}
				}
				else if (distToTwo.X < 100.0f && distToTwo.Y < 100.0f && aiC->GetMoveStatus() == EPathFollowingStatus::Idle) {
					//aiC->MoveToLocation(one);
					currentlyMovingTo = &one;
					waitTimer = 5.0f;

					if (r != nullptr) {
						FRotator faceCentre = FRotationMatrix::MakeFromX(r->getCentre() - GetActorLocation()).Rotator();

						faceCentre.Pitch = 0.0f;
						faceCentre.Roll = 0.0f;
						SetActorRotation(faceCentre);
					}
				}
				//have an idle timer and if it isn't waiting for a corridor then just start moving to the other patrol point
				//aiC->MoveToLocation(*currentlyMovingTo);
			}
		}
		else if (paused) {
			//put on a timer? every 0.5 ish seconds? might not end up staying in tick
			if (waitFor->lock(this)) {
				resume();
			}
			else {
				pause(waitFor);
			}
		}
	}
}

void ATP_ThirdPersonCharacter::handleTalking(float deltaTime) {

}

void ATP_ThirdPersonCharacter::handleReturn(float deltaTime) {

}

void ATP_ThirdPersonCharacter::handleSuspicious(float deltaTime) {
	if (timeSeenFor > 2.5f) {
		guardState = EGuardState::GS_Alerted;
		GetCharacterMovement()->MaxWalkSpeed = 90.0f;
		updateFOV(80.0f);
		waitTimer = 0.0f;
		indivPlayerLastPosition = getPlayerLocFromBP();
		aiC->MoveToLocation(indivPlayerLastPosition);
		//aiC->MoveToLocation(individualLastSighting);
	}
	else if (individualLastSighting != indivOldSighting) {
		aiC->MoveToLocation(individualLastSighting);
		waitTimer = 0.0f;
	}
	else if (waitTimer > 0.0f) {
		waitTimer -= deltaTime;
		FRotator rot = GetActorRotation();
		rot.Yaw -= deltaTime * 70.0f;
		SetActorRotation(rot);

		if (waitTimer <= 0.0f) {
			waitTimer = 0.0f;
			timeSeenFor = 0.0f;
			guardState = EGuardState::GS_Patrol;
			respectCorridorLocks = true;
			GetCharacterMovement()->MaxWalkSpeed = 50.0f;
			updateFOV(55.0f);
			aiC->MoveToLocation(*currentlyMovingTo);
		}
	}
	else {
		FVector distToTarget = GetActorLocation() - individualLastSighting;
		if (distToTarget.X < 0.0f) {
			distToTarget.X *= -1.0f;
		}
		if (distToTarget.Y < 0.0f) {
			distToTarget.Y *= -1.0f;
		}

		if (distToTarget.X < 100.0f && distToTarget.Y < 100.0f) {
			waitTimer = 7.0f;
		}
		else {
			aiC->MoveToLocation(individualLastSighting);
		}
	}
}

void ATP_ThirdPersonCharacter::handleAlerted(float deltaTime) {
	if (waitTimer > 0.0f) {
		waitTimer -= deltaTime;

		if (waitTimer <= 0.0f) {
			waitTimer = 0.0f;
			timeSeenFor = 2.5f;//don't hardcode this, needs to be <= the value used to transition to alerted from susp
			guardState = EGuardState::GS_Suspicious;
			GetCharacterMovement()->MaxWalkSpeed = 70.0f;
			updateFOV(60.0f);
			aiC->MoveToLocation(individualLastSighting);
		}
	}

	if (manuallyAlerted) {
		timeSeenFor = 2.0f;
	}

	indivPlayerLastPosition = getPlayerLocFromBP();
	if (timeSeenFor > 0.0f) {
		waitTimer = 10.0f;
	}

	//in range check:
	FVector dirToPlayer;//might not need to actually use
	float distToPlayer;
	//if guard faces wrong way then swap these round
	(indivPlayerLastPosition - GetActorLocation()).ToDirectionAndLength(dirToPlayer, distToPlayer);
	dirToPlayer.Z = 0.0f;//might break everything, safer to set pitch? manually later

	
	if (distToPlayer <= (weaponRange * 0.9f)) {
		manuallyAlerted = false;
		//fire ray from eye to player
		FVector start = FVector(0.0f, 0.0f, BaseEyeHeight) + GetActorLocation();
		FVector ProjDirection = GetControlRotation().Vector() * weaponRange;
		FVector end = start + ProjDirection;

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		//Re-initialize hit info
		FHitResult RV_Hit(ForceInit);

		GetWorld()->LineTraceSingleByChannel(RV_Hit, start, end, ECC_Pawn, RV_TraceParams);

		bool hit = RV_Hit.bBlockingHit; //did hit something? (bool)
		if (hit) {
			AFP_FirstPersonCharacter *player = Cast<AFP_FirstPersonCharacter>(RV_Hit.GetActor());
			if (player != nullptr) {
				shooting = true;
				aiC->StopMovement();
				FRotator facePlayer = FRotationMatrix::MakeFromX(indivPlayerLastPosition - GetActorLocation()).Rotator();

				facePlayer.Pitch = 0.0f;
				facePlayer.Roll = 0.0f;
				SetActorRotation(facePlayer);//change to aic get pawn, cast, set rotation

				//trace ray and shoot
				//don't play anim here, just set a flag

				//have bullet countdown in seconds, also need to reset it on state changes
				
				bulletCountdown -= deltaTime;
				if(bulletCountdown <= 0.0) {
					player->reduceHealth();
					bulletCountdown = 0.7f;
				}
				
			}
		}
	}
	else {
		shooting = false;
	}

	if (!shooting && indivPlayerLastPosition != indivPlayerOldPosition) {
		EPathFollowingRequestResult::Type res = aiC->MoveToLocation(indivPlayerLastPosition);
		if (res == EPathFollowingRequestResult::Type::Failed) {
			indivPlayerLastPosition = indivPlayerOldPosition;
			res = aiC->MoveToLocation(indivPlayerLastPosition);

			if (res == EPathFollowingRequestResult::Type::Failed) {
				aiC->MoveToLocation(individualLastSighting);
			}
		}
	}
}

bool ATP_ThirdPersonCharacter::getshooting() {
	return shooting;
}

void ATP_ThirdPersonCharacter::handleAlertedPerm(float deltaTime) {
	//basically do handleAlerted but reset status to alertedperm each time...
	//also need to duplicate some patrol stuff in here
	//ie. if it would have left alerted state then instead do custom patrolling here
}

void ATP_ThirdPersonCharacter::handleHighAlert(float deltaTime) {
	//basically handleAlerted too, also check hiding places and go to AlertedPerm when done
}

void ATP_ThirdPersonCharacter::Tick(float DeltaTime) {
	if (!dead) {
		switch (guardState) {
			case EGuardState::GS_Patrol: {
				handlePatrol(DeltaTime);
				break;
			}
			case EGuardState::GS_Talking: {
				handleTalking(DeltaTime);
				break;
			}
			case EGuardState::GS_Return: {
				handleReturn(DeltaTime);
				break;
			}
			case EGuardState::GS_Suspicious: {
				handleSuspicious(DeltaTime);
				break;
			}
			case EGuardState::GS_Alerted: {
				handleAlerted(DeltaTime);
				break;
			}
			case EGuardState::GS_AlertedPerm: {
				handleAlertedPerm(DeltaTime);
				break;
			}
			case EGuardState::GS_HighAlert: {
				handleHighAlert(DeltaTime);
				break;
			}
		}

		//body check for high alert here?

		indivOldSighting = individualLastSighting;
		indivPlayerOldPosition = indivPlayerLastPosition;
	}
	else {
		if (timer > 270) {
			GetMesh()->SetSimulatePhysics(true);
			animComplete = true;
		}
		else {
			timer++;
		}
	}

	Super::Tick(DeltaTime);
}

AActor *ATP_ThirdPersonCharacter::standingOn() {
	FVector example = FVector(0.0f, 0.0f, BaseEyeHeight);
	FVector ActorLocation = GetActorLocation();
	FVector start = example + ActorLocation;
	//FVector ProjDirection = GetControlRotation().Vector() * MaxUseDistance;
	FVector ProjDirection = FVector(0.0f, 0.0f, -1.0f) * MaxUseDistance;
	FVector end = start + ProjDirection;

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(RV_Hit, start, end, ECC_Pawn, RV_TraceParams);

	bool hit = RV_Hit.bBlockingHit; //did hit something? (bool)
	if (hit) {
		return RV_Hit.GetActor();
	}
	else {
		return nullptr;
	}
}

FVector ATP_ThirdPersonCharacter::getPlayerLocFromBP_Implementation() {
	return FVector(0.0f, 0.0f, 0.0f);
}

int ATP_ThirdPersonCharacter::getFloor() {
	return floor;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATP_ThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_ThirdPersonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATP_ThirdPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATP_ThirdPersonCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATP_ThirdPersonCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATP_ThirdPersonCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATP_ThirdPersonCharacter::OnResetVR);
}

void ATP_ThirdPersonCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATP_ThirdPersonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ATP_ThirdPersonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ATP_ThirdPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATP_ThirdPersonCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

bool ATP_ThirdPersonCharacter::ChokeAnimation() {
	return chokeAnim;
}

void ATP_ThirdPersonCharacter::setChoke_Implementation(bool choke) {
	chokeAnim = choke;
	start = false;
	dead = true;
	//aiC->MoveToLocation(GetActorLocation());
	aiC->StopMovement();

}

bool ATP_ThirdPersonCharacter::getanimComplete() {
	return animComplete;
}