#include "ShootGun.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"
#include "Items/Weapon.h"

UShootGun::UShootGun()
{
	bNotifyTick = true;
	NodeName = TEXT("Pick Up Item");
}

EBTNodeResult::Type UShootGun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto InventoryComponent = AIController->FindComponentByClass<UInventoryComponent>();
	auto Inventory = InventoryComponent->GetInventory();
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (auto Weapon = Cast<AWeapon>(Inventory[i]))
		{
			InventoryComponent->UseItem(i);
			if (Weapon->GetValue() <= 0)
				InventoryComponent->RemoveItem(i);
			break;
		}
	}
	return EBTNodeResult::Succeeded;
}

FString UShootGun::GetStaticDescription() const
{
	return FString::Printf(TEXT("Picks up the item specified in blackboard"));
}
