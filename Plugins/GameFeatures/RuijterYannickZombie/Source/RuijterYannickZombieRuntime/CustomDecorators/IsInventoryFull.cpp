// Fill out your copyright notice in the Description page of Project Settings.


#include "IsInventoryFull.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"

#include "Items/Medkit.h"

UIsInventoryFull::UIsInventoryFull()
{
}

bool UIsInventoryFull::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Survivor = Cast<ASurvivorPawn>(OwnerComp.GetAIOwner()->GetPawn());
	auto Inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	auto ItemsInInventory = Inventory->GetInventory();
	int ItemCount = 0;
	//count the amount of items we have
	for (auto Item : ItemsInInventory)
	{
		if (Item != nullptr) ItemCount++;
	}
	return ItemCount == Inventory->GetInventoryCapacity();
}