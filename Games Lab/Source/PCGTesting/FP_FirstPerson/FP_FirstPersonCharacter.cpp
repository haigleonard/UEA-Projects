// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FP_FirstPersonCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "LockerActor.h"
#include "Engine.h"

#define COLLISION_WEAPON		ECC_GameTraceChannel1

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFP_FirstPersonCharacter

bool AFP_FirstPersonCharacter::playerCreated = false;

AFP_FirstPersonCharacter::AFP_FirstPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(30.0f, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	//Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	//Mesh1P->SetOnlyOwnerSee(false);				// Set so only owner can see mesh
	//Mesh1P->SetupAttachment(FirstPersonCameraComponent);	// Attach mesh to FirstPersonCameraComponent
	//Mesh1P->bCastDynamicShadow = true;			// Disallow mesh to cast dynamic shadows
	//Mesh1P->CastShadow = true;				// Disallow mesh to cast other shadows

	// Create a gun mesh component
	/*FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// Only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;		// Disallow mesh to cast dynamic shadows
	FP_Gun->CastShadow = false;			// Disallow mesh to cast other shadows
	FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));

	// Set weapon damage and range
	WeaponRange = 5000.0f;
	WeaponDamage = 500000.0f;*/

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	GetCharacterMovement()->JumpZVelocity = 900.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	CameraBoom2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom2"));
	CameraBoom2->SetupAttachment(RootComponent);
	CameraBoom2->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom2->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												 // Create a follow camera
	FollowCamera2 = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera2"));
	FollowCamera2->SetupAttachment(CameraBoom2, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera2->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> playerSkeletalMeshAsset(TEXT("/Game/SeanConnery/SNTest3.SNTest3"));//SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'
	if (playerSkeletalMeshAsset.Object != NULL) {
		GetMesh()->SetSkeletalMesh(playerSkeletalMeshAsset.Object);
		//Mesh1P->SetSkeletalMesh(playerSkeletalMeshAsset.Object);
		//GetMesh()->SetupAttachment(FirstPersonCameraComponent);
	}

	//Mesh1P->AddLocalOffset(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * -1.0f));
	//Mesh1P->AddLocalRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->AddLocalOffset(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * -1.0f));
	GetMesh()->AddLocalRotation(FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UClass> animBP(TEXT("/Game/SeanConnery/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));//AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'
	if (animBP.Object != NULL) {
		//Mesh1P->SetAnimInstanceClass(animBP.Object);
		//Mesh1P->InitAnim(true);
		GetMesh()->SetAnimInstanceClass(animBP.Object);
		GetMesh()->InitAnim(true);
	}
	/*
	static ConstructorHelpers::FObjectFinder<UClass> ladderBPAsset(TEXT("Blueprint'/Game/Bomb/Bomb/Ladder/UELadder_Blueprint.UELadder_Blueprint_C'"));
	if (ladderBPAsset.Object != NULL) {
		ladderBP = ladderBPAsset.Object;
	}
	*/
	teamId = FGenericTeamId(0);

	if (!playerCreated) {
		AutoReceiveInput = EAutoReceiveInput::Player0;
		AutoPossessPlayer = EAutoReceiveInput::Player0;

		playerCreated = true;
		isPlayer = true;
	}
}

FGenericTeamId AFP_FirstPersonCharacter::GetGenericTeamId() const {
	return teamId;
}

void AFP_FirstPersonCharacter::setladNei(FVector ladCoor) {
	ladderNei = ladCoor;
}

void AFP_FirstPersonCharacter::Tick(float DeltaTime) {
	FVector example = FVector(0.0f, 0.0f, BaseEyeHeight);
	FVector ActorLocation = GetActorLocation();
	FVector start = example + ActorLocation;
	FVector ProjDirection = GetControlRotation().Vector() * MaxUseDistance;
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
	if (hit)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString("hiittt "));
		AActor* actor = RV_Hit.GetActor(); //the hit actor if there is one
										   //could try casting to ptr to bomb?
		FString actorName = actor->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("name: %s"), *actorName));
		if (actorName.Contains("Bomb_") || actorName.Contains("Ladder")) {
			bomb = true;
			ladder = true;
		}
		else {
			bomb = false;
			ladder = false;
		}
	}
	else {
		bomb = false;
		ladder = false;
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString("hiittt2 "));
	}
	if (ladderAnim == true) {
		if (timer > 270) {
			ladderAnim = false;
			timer = 0;
			
		}
		else {
			timer++;
		}
	}
	if (ladderClim == true) {
		if (timer > 130) {
			ladderClim = false;
			timer = 0; 
			GetController()->GetPawn()->SetActorLocation(ladderNei + (0.0f, 0.0f, 97.0f));
		}
		else {
			timer++;
		}
	}
	if (CCTVBool == true) {
		if (timer > 300) {
			CCTVBool = false;
			timer = 0;
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (currentlyHacking != nullptr) {
				currentlyHacking->hackCamera();
			}
			EnableInput(PlayerController);
		}
		else {
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			DisableInput(PlayerController);
			timer++;
		}
	}

	Super::Tick(DeltaTime);
}

bool AFP_FirstPersonCharacter::getbombDetec() {
	return bomb;
}
bool AFP_FirstPersonCharacter::getLadderDetec() {
	return ladder;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFP_FirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);
	
	// Set up gameplay key bindings

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Bind fire event
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AFP_FirstPersonCharacter::OnFire);
	InputComponent->BindAction("Choke", IE_Pressed, this, &AFP_FirstPersonCharacter::ChokeMethod);
	InputComponent->BindAction("Cheat", IE_Pressed, this, &AFP_FirstPersonCharacter::CheatMethod);
	InputComponent->BindAction("Health", IE_Pressed, this, &AFP_FirstPersonCharacter::HealthMethod);
	
	// Attempt to enable touch screen movement
	TryEnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFP_FirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFP_FirstPersonCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFP_FirstPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFP_FirstPersonCharacter::LookUpAtRate);
}

void AFP_FirstPersonCharacter::CheatMethod() {
	UGameplayStatics::OpenLevel(GetWorld(), "Bomb");
}

void AFP_FirstPersonCharacter::HealthMethod() {
	health = 1000;
}

void AFP_FirstPersonCharacter::ChokeMethod() {
	
	FVector example = FVector(0.0f, 0.0f, BaseEyeHeight);
	FVector ActorLocation = GetActorLocation();
	FVector start = example + ActorLocation;
	FVector ProjDirection = GetControlRotation().Vector() * MaxUseDistance;
	FVector end = start + ProjDirection;

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	//call GetWorld() from within an actor extending class
	//GetWorld()->LineTraceSingleByChannel(RV_Hit, start, end, ECC_Pawn, RV_TraceParams);
	GetWorld()->LineTraceSingleByChannel(RV_Hit, start, end, ECC_WorldDynamic, RV_TraceParams);

	bool hit = RV_Hit.bBlockingHit; //did hit something? (bool)

	if (hit)
	{
		AActor* actor = RV_Hit.GetActor(); //the hit actor if there is one
		ATP_ThirdPersonCharacter *AICharacter = Cast<ATP_ThirdPersonCharacter>(actor);
		ALockerActor *locker = Cast<ALockerActor>(actor);

		//AStaticMeshActor *ladderActor = Cast<AStaticMeshActor>(actor);
		FString actorName = actor->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Name: %s"), *actorName));
		if (AICharacter != nullptr) {
			AICharacter->setChoke(true);
			ladderAnim = true;
			choke = false;
		}
		if (actorName.Contains("Ladder"))
		{
			ladderClim = true;
		}
		if (actorName.Contains("CCTV"))
		{
			CCTVBool = true;
			currentlyHacking = Cast<ACCTVPawn>(actor);
		}

		if (locker != nullptr) {
			locker->openClose();
		}
	}
}

bool AFP_FirstPersonCharacter::getladderClim() {
	return ladderClim;
}


bool AFP_FirstPersonCharacter::getCCTVBool() {
	return CCTVBool;
}

void AFP_FirstPersonCharacter::setChoke(bool ChokeB) {
	choke = ChokeB;
}

float AFP_FirstPersonCharacter::circleRetX(float loc, float rot) {
	if (rot > 360) {
		rot = rot - 360;
	}
	float x = loc + 10 * cos(rot);
	return x;
}

float AFP_FirstPersonCharacter::circleRetY(float loc, float rot) {
	if (rot > 360) {
		rot = rot - 360;
	}
	float y = loc + 10 * sin(rot);
	return y;
}


FVector AFP_FirstPersonCharacter::AILoc()
{


	FVector example = FVector(0.0f, 0.0f, BaseEyeHeight);
	FVector ActorLocation = GetActorLocation();
	FVector start = example + ActorLocation;
	FVector ProjDirection = GetControlRotation().Vector() * MaxUseDistance;
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
	if (hit)
	{
		AActor* actor = RV_Hit.GetActor(); //the hit actor if there is one
		ATP_ThirdPersonCharacter *AICharacter = Cast<ATP_ThirdPersonCharacter>(actor);

		FString actorName = actor->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Name: %s"), *actorName));
		if (AICharacter != nullptr) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Name: %s"), *actorName));
			return AICharacter->GetActorLocation();

		}

	}
	else {
		return GetActorLocation();
	}
	return GetActorLocation();
}

FRotator AFP_FirstPersonCharacter::AIRot()
{


	FVector example = FVector(0.0f, 0.0f, BaseEyeHeight);
	FVector ActorLocation = GetActorLocation();
	FVector start = example + ActorLocation;
	FVector ProjDirection = GetControlRotation().Vector() * MaxUseDistance;
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

	if (hit)
	{
		AActor* actor = RV_Hit.GetActor(); //the hit actor if there is one
		ATP_ThirdPersonCharacter *AICharacter = Cast<ATP_ThirdPersonCharacter>(actor);

		FString actorName = actor->GetName();
		if (AICharacter != nullptr) {
			return AICharacter->GetActorRotation();
		}

	}
	else {
		return GetActorRotation();
	}
	return GetActorRotation();
}

FVector AFP_FirstPersonCharacter::GetAIForwardVector() {
	FVector example = FVector(0.0f, 0.0f, BaseEyeHeight);
	FVector ActorLocation = GetActorLocation();
	FVector start = example + ActorLocation;
	FVector ProjDirection = GetControlRotation().Vector() * MaxUseDistance;
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

	if (hit)
	{
		AActor* actor = RV_Hit.GetActor(); //the hit actor if there is one
		ATP_ThirdPersonCharacter *AICharacter = Cast<ATP_ThirdPersonCharacter>(actor);

		FString actorName = actor->GetName();
		if (AICharacter != nullptr) {
			return AICharacter->GetActorForwardVector();
		}

	}
	else {
		return GetActorForwardVector();
	}
	return GetActorForwardVector();
}

void AFP_FirstPersonCharacter::OnFire()
{
	FVector example = FVector(0.0f, 0.0f, BaseEyeHeight);
	FVector ActorLocation = GetActorLocation();
	FVector start = example + ActorLocation;
	FVector ProjDirection = GetControlRotation().Vector() * MaxUseDistance;
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

	if (hit)
	{
		AActor* actor = RV_Hit.GetActor(); //the hit actor if there is one
		FString actorName = actor->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("Name: %s"), *actorName));
		if (actorName.Contains("Bomb_")) {
			UGameplayStatics::OpenLevel(GetWorld(), "Bomb");
		}
	}
}


void AFP_FirstPersonCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// If touch is already pressed check the index. If it is not the same as the current touch assume a second touch and thus we want to fire
	if (TouchItem.bIsPressed == true)
	{
		if( TouchItem.FingerIndex != FingerIndex)
		{
			OnFire();			
		}
	}
	else 
	{
		// Cache the finger index and touch location and flag we are processing a touch
		TouchItem.bIsPressed = true;
		TouchItem.FingerIndex = FingerIndex;
		TouchItem.Location = Location;
		TouchItem.bMoved = false;
	}
}

void AFP_FirstPersonCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// If we didn't record the start event do nothing, or this is a different index
	if((TouchItem.bIsPressed == false) || ( TouchItem.FingerIndex != FingerIndex) )
	{
		return;
	}

	// If the index matches the start index and we didn't process any movement we assume we want to fire
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}

	// Flag we are no longer processing the touch event
	TouchItem.bIsPressed = false;
}

void AFP_FirstPersonCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// If we are processing a touch event and this index matches the initial touch event process movement
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (GetWorld() != nullptr)
		{
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
			{
				FVector MoveDelta = Location - TouchItem.Location;
				FVector2D ScreenSize;
				ViewportClient->GetViewportSize(ScreenSize);
				FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
				if (FMath::Abs(ScaledDelta.X) >= (4.0f / ScreenSize.X))
				{
					TouchItem.bMoved = true;
					float Value = ScaledDelta.X * BaseTurnRate;
					AddControllerYawInput(Value);
				}
				if (FMath::Abs(ScaledDelta.Y) >= (4.0f / ScreenSize.Y))
				{
					TouchItem.bMoved = true;
					float Value = ScaledDelta.Y* BaseTurnRate;
					AddControllerPitchInput(Value);
				}
				TouchItem.Location = Location;
			}
			TouchItem.Location = Location;
		}
	}
}

void AFP_FirstPersonCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFP_FirstPersonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFP_FirstPersonCharacter::TurnAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFP_FirstPersonCharacter::LookUpAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FHitResult AFP_FirstPersonCharacter::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams);

	return Hit;
}

bool AFP_FirstPersonCharacter::returnClimb() {
	return ladderAnim;
}
float AFP_FirstPersonCharacter::returnHealth() {
	return health;
}
void AFP_FirstPersonCharacter::reduceHealth() {
	health -= 0.1;
	
}

void AFP_FirstPersonCharacter::TryEnableTouchscreenMovement(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFP_FirstPersonCharacter::BeginTouch);
	PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFP_FirstPersonCharacter::EndTouch);
	PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFP_FirstPersonCharacter::TouchUpdate);	
}
