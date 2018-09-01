// Fill out your copyright notice in the Description page of Project Settings.

#include "VoiceChatActor.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "TimerManager.h"


// Sets default values
AVoiceChatActor::AVoiceChatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*FModuleManager::Get().LoadModule("Voice");
	if (FVoiceModule::IsAvailable()) {
		if (FVoiceModule::Get().IsVoiceEnabled()) {
			VoiceCapture = FVoiceModule::Get().CreateVoiceCapture(FString(""), 16000, 2);
			VoiceCapture->Start();
		}
	}

	VoiceCaptureAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceCaptureAudioComponent"));
	VoiceCaptureAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	VoiceCaptureAudioComponent->bAutoActivate = true;
	VoiceCaptureAudioComponent->bAlwaysPlay = true;
	VoiceCaptureAudioComponent->PitchMultiplier = 5.0f;
	VoiceCaptureAudioComponent->VolumeMultiplier = 5.f;*/

}

void AVoiceChatActor::VoiceCaptureTick() {
	/*if (!VoiceCapture.IsValid()) {
		return;
	}

	if (!runOnce) {
		VoiceCaptureSoundWaveProcedural = NewObject<USoundWaveProcedural>();
		VoiceCaptureSoundWaveProcedural->SampleRate = 16000;
		VoiceCaptureSoundWaveProcedural->NumChannels = 1;
		VoiceCaptureSoundWaveProcedural->Duration = INDEFINITELY_LOOPING_DURATION;
		VoiceCaptureSoundWaveProcedural->SoundGroup = SOUNDGROUP_Voice;
		VoiceCaptureSoundWaveProcedural->bLooping = false;
		VoiceCaptureSoundWaveProcedural->bProcedural = true;
		VoiceCaptureSoundWaveProcedural->Pitch = 5.0f;
		VoiceCaptureSoundWaveProcedural->Volume = 5.f;

		runOnce = true;
	}

	uint32 VoiceCaptureBytesAvaiable = 0;
	EVoiceCaptureState::Type CaptureState = VoiceCapture->GetCaptureState(VoiceCaptureBytesAvaiable);

	VoiceCaptureBuffer.Reset();
	PlayVoiceCaptureFlag = false;

	if (CaptureState == EVoiceCaptureState::Ok && VoiceCaptureBytesAvaiable > 0) {
		int16_t VoiceCaptureSample;
		uint32 VoiceCaptureReadBytes;
		float VoiceCaptureTotalSquared = 0;

		VoiceCaptureBuffer.SetNumUninitialized(VoiceCaptureBytesAvaiable);
		VoiceCapture->GetVoiceData(VoiceCaptureBuffer.GetData(), VoiceCaptureBytesAvaiable, VoiceCaptureReadBytes);

		for (uint32 i = 0; i < (VoiceCaptureReadBytes / 2); i++) {
			VoiceCaptureSample = (VoiceCaptureBuffer[i * 2 + 1] << 8) | VoiceCaptureBuffer[i * 2];
			VoiceCaptureTotalSquared += ((float)VoiceCaptureSample * (float)VoiceCaptureSample);
		}

		float VoiceCaptureMeanSquare = (2 * (VoiceCaptureTotalSquared / VoiceCaptureBuffer.Num()));
		float VoiceCaptureRMs = FMath::Sqrt(VoiceCaptureMeanSquare);
		float VoiceCaptureFinalVolume = ((VoiceCaptureRMs / 32768.0) * 200.0f);

		VoiceCaptureVolume = VoiceCaptureFinalVolume;

		VoiceCaptureSoundWaveProcedural->QueueAudio(VoiceCaptureBuffer.GetData(), VoiceCaptureReadBytes);
		VoiceCaptureAudioComponent->SetSound(VoiceCaptureSoundWaveProcedural);

		PlayVoiceCaptureFlag = true;
	}*/
}

void AVoiceChatActor::PlayVoiceCapture() {
	/*if (!PlayVoiceCaptureFlag) {
		VoiceCaptureAudioComponent->FadeOut(0.3f, 0.f);
		return;
	}

	if (VoiceCaptureAudioComponent->IsPlaying()) {
		return;
	}

	VoiceCaptureAudioComponent->Play();*/

}

// Called when the game starts or when spawned
void AVoiceChatActor::BeginPlay() {
	Super::BeginPlay();
	//GetWorldTimerManager().SetTimer(PlayVoiceCaptureTimer, this, &AVoiceChatActor::PlayVoiceCapture, 0.45f, true, 0.0f);
}

// Called every frame
void AVoiceChatActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//VoiceCaptureTick();
}

