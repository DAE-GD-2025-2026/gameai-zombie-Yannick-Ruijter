// Fill out your copyright notice in the Description page of Project Settings.


#include "IsItemNeeded.h"

#include "BehaviorTree/BlackboardComponent.h"
UIsItemNeeded::UIsItemNeeded()
{
}

bool UIsItemNeeded::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//todo change the item to pick up to the wanted item
	//set the inventory index
	return false;
}