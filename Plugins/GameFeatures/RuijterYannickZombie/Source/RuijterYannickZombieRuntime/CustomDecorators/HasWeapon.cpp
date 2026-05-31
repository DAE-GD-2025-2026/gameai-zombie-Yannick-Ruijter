// Fill out your copyright notice in the Description page of Project Settings.


#include "HasWeapon.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "Items/Weapon.h"

UHasWeapon::UHasWeapon()
{
}

bool UHasWeapon::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Survivor = Cast<ASurvivorPawn>(OwnerComp.GetAIOwner()->GetPawn());
	auto ItemsInInventory = Survivor->FindComponentByClass<UInventoryComponent>()->GetInventory();
	int WeaponCount = 0;
	//count the amount of weapons we have
	for (auto Item : ItemsInInventory)
	{
		if (Item == nullptr) continue;
		if (Item->GetItemType() == EItemType::Shotgun || Item->GetItemType() == EItemType::Pistol) WeaponCount++;
	}
	return WeaponCount > 0;
}