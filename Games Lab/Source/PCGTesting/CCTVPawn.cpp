// Fill out your copyright notice in the Description page of Project Settings.

#include "CCTVPawn.h"
#include "Room.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Net/UnrealNetwork.h"

#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))
#define printString(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString(TEXT(text)))

// Sets default values
ACCTVPawn::ACCTVPawn() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UClass> cameraAIBPAsset(TEXT("Blueprint'/Game/Geometry/CCTV/CameraAI_BP.CameraAI_BP_C'"));
	AIControllerClass = cameraAIBPAsset.Object;
}

void ACCTVPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCTVPawn, sTime);
	DOREPLIFETIME(ACCTVPawn, seesPlayer);
}

void ACCTVPawn::init(URoom *containingRoom) {
	this->containingRoom = containingRoom;
	FRotator faceCentre = FRotationMatrix::MakeFromX(containingRoom->getCentre() - GetActorLocation()).Rotator();

	if (containingRoom->getCentre().Z >= 300.0f) {
		level = 1;
	}
	else {
		level = 0;
	}
	
	faceCentre.Pitch = 0.0f;
	SetActorRotation(faceCentre);
}

void ACCTVPawn::changeDirection() {
	rotationAmmount *= -1.0f;
}

void ACCTVPawn::setEyePosition(FVector ep) {
	this->eyePos = ep;
}

void ACCTVPawn::updateCCTV_Implementation() {
	if (true) {
		float yellowTime = 1.5f;
		float redTime = 4.0f;

		if (!seesPlayer && HasAuthority()) {
			FRotator rot = GetActorRotation();
			rot.Yaw += rotationAmmount;
			SetActorRelativeRotation(rot);
		}

		//change light colour based on sTime...
		if (sTime <= yellowTime) {
			lightRed = sTime / yellowTime;

			if (state == Alertness::YELLOW && HasAuthority()) {
				state = Alertness::GREEN;
			}
		}
		else if (sTime <= redTime) {
			lightRed = 1.0f;
			lightGreen = 1.0f - (sTime / redTime);

			if (state == Alertness::GREEN && HasAuthority()) {
				//alert a guard (suspicious)
				state = Alertness::YELLOW;
				getClosestGuard(level)->suspiciousByCamera();
			}
			else if (state == Alertness::RED && HasAuthority()) {
				state = Alertness::YELLOW;
			}
		}
		else if (sTime > redTime) {
			if (state == Alertness::YELLOW && HasAuthority()) {
				//alert a guard
				getClosestGuard(level)->alertedByCamera();
				state = Alertness::RED;
			}
		}
	}
}

// Called when the game starts or when spawned
void ACCTVPawn::BeginPlay() {
	if (true) {
		SpawnDefaultController();
		GetWorldTimerManager().SetTimer(cameraUpdateTimer, this, &ACCTVPawn::updateCCTV, everyNSecs, true, 2.0f);
	}
	Super::BeginPlay();
}

void ACCTVPawn::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const {
	out_Location = eyePos;
	out_Rotation = GetActorRotation();
}

// Called every frame
void ACCTVPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACCTVPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACCTVPawn::hackCamera() {
	hacked = true;
}

bool ACCTVPawn::hackedCheck() {
	return hacked;
}