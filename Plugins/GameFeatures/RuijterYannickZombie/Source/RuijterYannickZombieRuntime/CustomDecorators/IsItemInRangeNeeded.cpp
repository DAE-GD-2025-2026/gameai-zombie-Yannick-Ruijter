// Fill out your copyright notice in the Description page of Project Settings.


#include "IsItemInRangeNeeded.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "../BlackBoardClasses/ObjectList.h"
UIsItemInRangeNeeded::UIsItemInRangeNeeded()
{
}

bool UIsItemInRangeNeeded::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//todo change the item to pick up to the wanted item
	//set the inventory index
	return false;
}