#include "ShootGunRuijterYannick.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"

UShootGunRuijterYannick::UShootGunRuijterYannick()
{
	bNotifyTick = true;
	NodeName = TEXT("Shoot gun");
}

EBTNodeResult::Type UShootGunRuijterYannick::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto InventoryComponent = AIController->GetPawn()->FindComponentByClass<UInventoryComponent>();
	auto Inventory = InventoryComponent->GetInventory();
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i] == nullptr) continue;
		if (Inventory[i]->GetItemType() == EItemType::Shotgun || Inventory[i]->GetItemType() == EItemType::Pistol)
		{
			InventoryComponent->UseItem(i);
			if (Inventory[i]->GetValue() <= 0)
				InventoryComponent->RemoveItem(i);
			break;
		}
	}
	
	auto BlackBoard = AIController->GetBlackboardComponent();
	if (BlackBoard->GetValueAsObject("ClosestEnemy") == nullptr)
		BlackBoard->SetValueAsBool("EnemySpotted", false);
	
	//make sure it continues rotating
	BlackBoard->SetValueAsBool("NeedsRefocus", true);
	return EBTNodeResult::Succeeded;
}

FString UShootGunRuijterYannick::GetStaticDescription() const
{
	return FString::Printf(TEXT("Picks up the item specified in blackboard"));
}
