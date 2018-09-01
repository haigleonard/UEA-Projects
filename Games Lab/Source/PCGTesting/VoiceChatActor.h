// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OnlineSubsystemUtils.h"
#include "Voice.h"
#include "Sound/SoundWaveProcedural.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoiceChatActor.generated.h"

UCLASS()
class PCGTESTING_API AVoiceChatActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	bool runOnce = false;
	
public:	
	// Sets default values for this actor's properties
	AVoiceChatActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		float VoiceCaptureVolume;
	UPROPERTY()
		bool VoiceCaptureTest;
	UPROPERTY()
		bool PlayVoiceCaptureFlag;

	UPROPERTY()
		FTimerHandle VoiceCaptureTickTimer;
	UPROPERTY()
		FTimerHandle PlayVoiceCaptureTimer;

	/*TSharedPtr<class IVoiceCapture> VoiceCapture;

	UPROPERTY()
		USoundWaveProcedural *VoiceCaptureSoundWaveProcedural;
	UPROPERTY()
		UAudioComponent* VoiceCaptureAudioComponent;*/

	UPROPERTY()
		TArray<uint8> VoiceCaptureBuffer;

	UFUNCTION()
		void VoiceCaptureTick();
	UFUNCTION()
		void PlayVoiceCapture();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
