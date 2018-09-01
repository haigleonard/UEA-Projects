// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameStateBase.h"
#include "Net/UnrealNetwork.h"

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyGameStateBase, randSeed);
	DOREPLIFETIME(AMyGameStateBase, time);
	DOREPLIFETIME(AMyGameStateBase, timeString);
	DOREPLIFETIME(AMyGameStateBase, floorVisibilities);
	DOREPLIFETIME(AMyGameStateBase, p1Location);
	DOREPLIFETIME(AMyGameStateBase, p1UpVector);
	DOREPLIFETIME(AMyGameStateBase, p1Transform);
	DOREPLIFETIME(AMyGameStateBase, p1Floor);
	DOREPLIFETIME(AMyGameStateBase, p1Health);
}

void AMyGameStateBase::setRandSeed(int seed) {
	this->randSeed = seed;
}

int AMyGameStateBase::getRandSeed() {
	return randSeed;
}

void AMyGameStateBase::setTime(float newTime, FString newTimeString) {
	this->time = newTime;
	this->timeString = newTimeString;
}

float AMyGameStateBase::getTime() {
	return time;
}

FString AMyGameStateBase::getTimeString() {
	return timeString;
}

void AMyGameStateBase::setFloorVisibility(int floorNum, bool visible) {
	while (floorVisibilities.Num() < (floorNum + 1)) {
		floorVisibilities.Add(false);
	}

	floorVisibilities[floorNum] = visible;
}

bool AMyGameStateBase::getFloorVisibility(int floorNum) {
	if (floorVisibilities.Num() < (floorNum + 1)) {
		return false;
	}
	else {
		return floorVisibilities[floorNum];
	}
}

void AMyGameStateBase::setP1Data(FVector p1Loc, FVector p1Up, FTransform p1TForm, int floor, float health) {
	this->p1Location = p1Loc;
	this->p1UpVector = p1Up;
	this->p1Transform = p1TForm;
	this->p1Floor = floor;
	this->p1Health = health;
}

FVector AMyGameStateBase::getP1Location() {
	return p1Location;
}

FVector AMyGameStateBase::getP1UpVector() {
	return p1UpVector;
}

FTransform AMyGameStateBase::getP1Transform() {
	return p1Transform;
}

int AMyGameStateBase::getP1Floor() {
	return p1Floor;
}

float AMyGameStateBase::getP1Health() {
	return p1Health;
}