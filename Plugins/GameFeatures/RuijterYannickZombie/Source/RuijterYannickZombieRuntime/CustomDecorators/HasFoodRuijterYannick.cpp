// Fill out your copyright notice in the Description page of Project Settings.


#include "HasFoodRuijterYannick.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"

UHasFoodRuijterYannick::UHasFoodRuijterYannick()
{
}

bool UHasFoodRuijterYannick::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Survivor = Cast<ASurvivorPawn>(OwnerComp.GetAIOwner()->GetPawn());
	auto ItemsInInventory = Survivor->FindComponentByClass<UInventoryComponent>()->GetInventory();
	int FoodCount = 0;
	//count the amount of weapons we have
	for (auto Item : ItemsInInventory)
	{
		if (Item == nullptr) continue;
		if (Item->GetItemType() == EItemType::Food) FoodCount++;
	}
	return FoodCount > 0;
}