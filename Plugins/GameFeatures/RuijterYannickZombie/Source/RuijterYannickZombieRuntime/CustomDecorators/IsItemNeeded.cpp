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
	auto InventoryComponent = AIController->GetPawn()->FindComponentByClass<UInventoryComponent>();
	auto Inventory = InventoryComponent->GetInventory();
	int WeaponCount = 0;
	int FewestAmmo = 100000;
	int LastWeaponIndex = -1;
	int FoodCount = 0;
	int MedkitCount = 0;
	
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (auto Weapon = Cast<AWeapon>(Inventory[i]))
		{
			WeaponCount++;
			//get the ammo and compare to the current weapon with the fewest ammo
			if (Weapon->GetValue() < FewestAmmo)
			{
				FewestAmmo = Weapon->GetValue();
				LastWeaponIndex = i;
			}
		}
		else if (Cast<AFood>(Inventory[i]))
		{
			FoodCount++;
		}
		else if (Cast<AMedkit>(Inventory[i]))
		{
			MedkitCount++;
		}
		else
		{
			//if it's garbage (normally should not happen but just in case
			InventoryComponent->RemoveItem(i);
		}
	}
	
	
	return false;
}