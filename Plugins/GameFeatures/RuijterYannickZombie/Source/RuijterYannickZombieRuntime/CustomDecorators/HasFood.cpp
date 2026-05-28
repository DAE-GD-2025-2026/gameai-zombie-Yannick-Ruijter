// Fill out your copyright notice in the Description page of Project Settings.


#include "HasFood.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"

UHasFood::UHasFood()
{
}

bool UHasFood::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Survivor = Cast<ASurvivorPawn>(OwnerComp.GetAIOwner()->GetPawn());
	auto ItemsInInventory = Survivor->FindComponentByClass<UInventoryComponent>()->GetInventory();
	int FoodCount = 0;
	//count the amount of weapons we have
	for (auto Item : ItemsInInventory)
	{
		if (Cast<AFood>(Item)) FoodCount++;
	}
	return FoodCount > 0;
}