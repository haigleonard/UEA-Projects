// Fill out your copyright notice in the Description page of Project Settings.

#include "GridCell.h"

const float UGridCell::SIZE = 170.0f;

UGridCell::UGridCell() {
	this->centre = FVector(0.0f, 0.0f, 0.0f);
	//if default cell type changes remember to change in the new room location validation method in LevelActor.Spawn
	this->type = CellType::WALL;
}

UGridCell::~UGridCell() {

}

void UGridCell::init(int row, int col) {
	this->row = row;
	this->col = col;
}

bool UGridCell::isDiagonal(UGridCell *one, UGridCell *two) {
	return (one->getRow() == two->getRow() - 1 || one->getRow() == two->getRow() + 1) && (one->getCol() == two->getCol() - 1 || one->getCol() == two->getCol() + 1);
}

bool UGridCell::isOneCellWide() {
	int count = 0;
	int wallCount = 0;
	bool neighbourInRoom = false;

	TArray<UGridCell *> nonWallNeighbours;
	for (UGridCell *cell : neighbours) {
		if (cell->inRoom()) {
			neighbourInRoom = true;
		}
		if (!(cell->getType() == CellType::WALL) && !isDiagonal(this, cell)) {
			count++;
			nonWallNeighbours.Emplace(cell);
		}
		if (cell->getType() == CellType::WALL) {
			wallCount++;
		}
	}

	if (count == 2 && neighbourInRoom && wallCount != 5) {
		//check if non-wall and non-diagonal neighbours are perpendicular or straight through
		//return not perpendicular
		return isDiagonal(nonWallNeighbours[0], nonWallNeighbours[1]);
	}
	else {
		return (count > 2 && neighbourInRoom && wallCount != 5);
	}
}

TArray<UGridCell *> UGridCell::getNonDiagNeighbours() {
	TArray<UGridCell *> nonDN;
	for (UGridCell *cell : neighbours) {
		if (!isDiagonal(this, cell)) {
			nonDN.Emplace(cell);
		}
	}

	return nonDN;
}

bool UGridCell::validCameraCell() {
	TArray<UGridCell *> nonDN = getNonDiagNeighbours();

	int wallCount = 0;
	for (UGridCell *cell : nonDN) {
		if (cell->getType() == CellType::WALL) {
			wallCount++;
		}
		else if (cell->isCorridor()) {
			return false;
		}
	}


	return wallCount == 1;
}

bool UGridCell::insideCell(FVector test) {
	return (test.X < (centre.X + SIZE / 2.0f)) && (test.X >(centre.X - SIZE / 2.0f)) && (test.Y < (centre.Y + SIZE / 2.0f)) && (test.Y >(centre.Y - SIZE / 2.0f));
}