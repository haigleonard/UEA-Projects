// Fill out your copyright notice in the Description page of Project Settings.

#include "AStar.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"

float UAStar::calcDist(UGridCell *one, UGridCell *two) {
	if (isDiag(one, two)) {
		return 2.0f;
	}
	else {
		return 1.0f;
	}
}

float UAStar::calcHeuristic(UGridCell *one, UGridCell *two) {
	float dRow = abs(two->getRow() - one->getRow());
	float dCol = abs(two->getCol() - one->getCol());

	return FMath::Max(dRow, dCol);
}

bool UAStar::shareWall(UGridCell *one, UGridCell *two) {
	int a = one->getNumNeighbours();
	int b = two->getNumNeighbours();

	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) {
			UGridCell *oneNeighbour = one->getNeighbour(i);
			UGridCell *twoNeighbour = two->getNeighbour(j);

			if ((oneNeighbour == twoNeighbour) && oneNeighbour->getType() == UGridCell::CellType::WALL) {
				return true;
			}
		}
	}

	return false;
}

bool UAStar::isDiag(UGridCell *one, UGridCell *two) {
	return (one->getRow() == two->getRow() + 1 || one->getRow() == two->getRow() - 1) && (one->getCol() == two->getCol() + 1 || one->getCol() == two->getCol() - 1);
}

TArray<UGridCell *> UAStar::aStar(UGridCell *start, UGridCell *end) {
	TMap<TPair<int, int>, UGridCell *> closed;

	TMap<TPair<int, int>, UGridCell *> open;
	open.Add(TPair<int, int>(start->getRow(), start->getCol()), start);

	TArray<UGridCell *> path;
	path.Add(end);

	TMap<UGridCell *, TPair<float, float>> costMap;//map of gridcell to pair of gcost, fcost
	costMap.Add(start, TPair<float, float>(0, calcHeuristic(start, end)));

	//not sure about replacement
	TMap<UGridCell *, UGridCell *> parentMap;
	
	while (open.Num() > 0) {
		UGridCell *current = findMin(open, costMap);

		open.Remove(TPair<int, int>(current->getRow(), current->getCol()));
		closed.Add(TPair<int, int>(current->getRow(), current->getCol()), current);

		bool best = false;
		unsigned numNeighbours = current->getNumNeighbours();

		for (unsigned i = 0; i < numNeighbours; i++) {
			UGridCell *next = current->getNeighbour(i);
			
			if (next->getType() != UGridCell::CellType::WALL && !(isDiag(current, next) && shareWall(current, next))) {
				if (!closed.Contains(TPair<int, int>(next->getRow(), next->getCol()))) {

					if (!open.Contains(TPair<int, int>(next->getRow(), next->getCol()))) {
						if (next == end) {
							parentMap.Add(end, current);

							while (parentMap[end] != start) {
								end = parentMap[end];
								path.Insert(end, 0);
							}

							path.Insert(start, 0);

							//if path size is one then no solution
							if (path.Num() == 1) {
								return TArray<UGridCell *>();
							}
							else {
								return path;
							}
						}

						float gCost = calcDist(next, current) + costMap[current].Key;
						float fCost = gCost + calcHeuristic(next, end);
						costMap.Add(next, TPair<float, float>(gCost, fCost));

						open.Add(TPair<int, int>(next->getRow(), next->getCol()), next);
						best = true;
					}
					else {
						best = costMap[current].Key > costMap[next].Key;
					}

					if (best) {
						parentMap.Add(next, current);
					}
				}
			}
		}
	}

	//if path size is one then no solution
	if (path.Num() == 1) {
		return TArray<UGridCell *>();
	}
	else {
		return path;
	}
}

UGridCell * UAStar::findMin(TMap<TPair<int, int>, UGridCell *> open, TMap<UGridCell *, TPair<float, float>> costMap) {
	UGridCell *result = nullptr;
	float min = TNumericLimits<float>::Max();

	TPair<int, int> hi;
	hi = TPair<int, int>(1, 2);

	for (const auto &entry : open) {
		UGridCell *cell = entry.Value;
		float fCost = costMap[cell].Value;

		if (fCost < min) {
			min = fCost;
			result = cell;
		}
	}

	return result;
}