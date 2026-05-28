// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessWeapon.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "../BlackBoardClasses/ObjectList.h"
#include "Items/Weapon.h"

UPossessWeapon::UPossessWeapon()
{
}

bool UPossessWeapon::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto const BlackBoard = OwnerComp.GetBlackboardComponent();
	auto ItemsInInventory = BlackBoard->GetValue<UObjectList>("Inventory");
	int WeaponCount = 0;
	//count the amount of weapons we have
	for (UObject* Item : ItemsInInventory)
	{
		if (Cast<AWeapon>(Item)) WeaponCount++;
	}
	return WeaponCount > 0;
}