// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridCell.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AStar.generated.h"

/**
 * 
 */
UCLASS()
class PCGTESTING_API UAStar : public UObject
{
	GENERATED_BODY()
	
private:
	static float calcHeuristic(UGridCell* one, UGridCell *two);
	static float calcDist(UGridCell *one, UGridCell *two);
	static UGridCell *findMin(TMap<TPair<int, int>, UGridCell *> open, TMap<UGridCell *, TPair<float, float>> costMap);
	static bool shareWall(UGridCell *one, UGridCell *two);
	static bool isDiag(UGridCell *one, UGridCell *two);

public:
	static TArray<UGridCell *> aStar(UGridCell *start, UGridCell *end);
	
	
};
