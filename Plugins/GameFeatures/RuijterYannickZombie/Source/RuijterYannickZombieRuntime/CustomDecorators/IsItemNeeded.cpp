// Fill out your copyright notice in the Description page of Project Settings.


#include "IsItemNeeded.h"

#include "AIController.h"
#include "IDetailTreeNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Items/Weapon.h"

UIsItemNeeded::UIsItemNeeded()
{
}

bool UIsItemNeeded::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto AIController = OwnerComp.GetAIOwner();
	auto BlackBoard = AIController->GetBlackboardComponent();
	//before doing this check, a check to see if  we spotted an item happens
	//settings this as false prevents this check happening each frame
	BlackBoard->SetValueAsBool("ItemSpotted", false);
	
	auto SpottedItem = Cast<ABaseItem>(BlackBoard->GetValueAsObject("LastSpottedItem"));
	auto ItemType = SpottedItem->GetItemType();
	//we really dont need garbage
	if (ItemType == EItemType::Garbage) return false;
	BlackBoard->SetValueAsObject("ItemToPickUp", SpottedItem);
	auto InventoryComponent = AIController->GetPawn()->FindComponentByClass<UInventoryComponent>();
	auto Inventory = InventoryComponent->GetInventory();
	int WeaponCount = 0;
	int FewestAmmo = 100000;
	int LastWeaponIndex = -1;
	int FoodCount = 0;
	int LastFoodIndex = -1;
	int MedkitCount = 0;
	int LastMedkitIndex = -1;
	int totalItemCount = 0;

	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i] != nullptr) totalItemCount++;
		else
		{
			//we always take an item if we have free space
			BlackBoard->SetValueAsInt("FreeItemSlot", i);
			return true;
		}
		
		switch (Inventory[i]->GetItemType())
		{
		case EItemType::Food:
			FoodCount++;
			LastFoodIndex = i;
			break;
		case EItemType::Medkit:
			MedkitCount++;
			LastMedkitIndex = i;
			break;
		case EItemType::Shotgun:
		case EItemType::Pistol:
			WeaponCount++;
			//get the ammo and compare to the current weapon with the fewest ammo
			if (Inventory[i]->GetValue() < FewestAmmo)
			{
				FewestAmmo = Inventory[i]->GetValue();
				LastWeaponIndex = i;
			}
			break;
		case EItemType::Garbage:
			//if it's garbage (normally should not happen but just in case)
			InventoryComponent->RemoveItem(i);
			break;
		}
	}
	
	//our optimal layout is 2 guns, 1 medkti and 2 foods
	switch (ItemType)
	{
	case EItemType::Pistol:
	case EItemType::Shotgun:
	{
	    auto Weapon = Cast<AWeapon>(SpottedItem);
	    // we need at least 2 weapons
	    if (WeaponCount <= 1)
	    {
	        // if we have too many medkits replace the medkit
	        if (MedkitCount >= FoodCount)
	            BlackBoard->SetValueAsInt("FreeItemSlot", LastMedkitIndex);
	        // if we have too much food, replace the food
	        else
	            BlackBoard->SetValueAsInt("FreeItemSlot", LastFoodIndex);
	        return true;
	    }
	    // if this weapon has less or equal ammo than the gun with the least ammo we possess we dont need it
	    if (Weapon->GetValue() <= FewestAmmo) return false;
	    // if this gun has more ammo than our gun with the least ammo, switch it
	    BlackBoard->SetValueAsInt("FreeItemSlot", LastWeaponIndex);
	    return true;
	}
	case EItemType::Medkit:
	{
	    // optimal is 1 medkit, so if we already have one we don't need more
	    if (MedkitCount >= 1) return false;
		//drop whatever we has too much off
	    if (WeaponCount > 2)
	        BlackBoard->SetValueAsInt("FreeItemSlot", LastWeaponIndex);
	    else if (FoodCount > 2)
	        BlackBoard->SetValueAsInt("FreeItemSlot", LastFoodIndex);
		return true;
	}
	case EItemType::Food:
	{
	    // optimal is 2 foods, so if we already have 2 or more we don't need more
	    if (FoodCount >= 2) return false;
			
		//drop whatever we have too much off			
	    if (WeaponCount > 2)
	        BlackBoard->SetValueAsInt("FreeItemSlot", LastWeaponIndex);
	    else if (MedkitCount > 1)
	        BlackBoard->SetValueAsInt("FreeItemSlot", LastMedkitIndex);
	    return true;
	}
	default:
	    break;
	}
	return false;
}
