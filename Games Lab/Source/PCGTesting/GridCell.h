// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCell.Generated.h"

/**
*
*/
UCLASS()
class PCGTESTING_API UGridCell : public UObject {
	GENERATED_BODY()

public:
	enum class CellType { FLOOR, WALL, ROOM, DOOR, LADDER_TOP, LADDER_BOTTOM };

private:
	//might just be xy size, z(height) could be different?
	//above may not even be applicable because this is just to work out room position, level layout etc
	static const float SIZE;

	UPROPERTY()
		FVector centre;

	CellType type;
	UPROPERTY()
		int row;
	UPROPERTY()
		int col;
	TArray<UGridCell *> neighbours;
	UPROPERTY()
		bool isInRoom = false;
	UPROPERTY()
		bool corridorProcessed = false;
	UPROPERTY()
		bool guardStart = false;

public:
	UGridCell();
	~UGridCell();

	void init(int row, int col);

	static bool isDiagonal(UGridCell *one, UGridCell *two);

	bool isOneCellWide();

	bool insideCell(FVector test);

	TArray<UGridCell *> getNonDiagNeighbours();

	bool validCameraCell();

	inline int getRow() {
		return row;
	}

	inline int getCol() {
		return col;
	}

	inline void setPosition(FVector newPosition) {
		this->centre = newPosition;
	}

	inline FVector getPosition() {
		return this->centre;
	}

	inline void setType(CellType type) {
		this->type = type;
	}

	inline CellType getType() {
		return type;
	}

	inline void setNeighbours(TArray<UGridCell *> neighbours) {
		this->neighbours = neighbours;
	}

	inline int getNumNeighbours() {
		return neighbours.Num();
	}

	inline UGridCell *getNeighbour(int i) {
		return neighbours[i];
	}

	inline void setInRoom() {
		isInRoom = true;
	}

	inline bool inRoom() {
		return isInRoom;
	}

	inline void setCorridor() {
		corridorProcessed = true;
	}

	inline bool isCorridor() {
		return corridorProcessed;
	}

	inline void setGuardStart() {
		guardStart = true;
	}

	inline bool getGuardStart() {
		return guardStart;
	}
};
