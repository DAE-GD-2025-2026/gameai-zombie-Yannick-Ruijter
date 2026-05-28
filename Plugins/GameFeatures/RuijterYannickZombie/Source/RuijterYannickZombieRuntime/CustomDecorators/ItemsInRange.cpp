// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemsInRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "../BlackBoardClasses/ObjectList.h"
UItemsInRange::UItemsInRange()
{
}

bool UItemsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto BlackBoard = OwnerComp.GetBlackboardComponent();
	return BlackBoard->GetValue<UObjectList>("ItemsInRange").Num() > 0;
}