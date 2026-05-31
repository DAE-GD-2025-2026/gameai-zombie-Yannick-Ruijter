#include "EatFood.h"
#include "AIController.h"
#include "IDetailTreeNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"
#include "Navigation/PathFollowingComponent.h"

UEatFood::UEatFood()
{
	bNotifyTick = true;
	NodeName = TEXT("Eats food");
}

EBTNodeResult::Type UEatFood::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto InventoryComponent = AIController->FindComponentByClass<UInventoryComponent>();
	auto Inventory = InventoryComponent->GetInventory();
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i]->GetItemType() == EItemType::Food)
		{
			InventoryComponent->UseItem(i);
			InventoryComponent->RemoveItem(i);
			break;
		}
	}
	return EBTNodeResult::Succeeded;
}

FString UEatFood::GetStaticDescription() const
{
	return FString::Printf(TEXT("Eats food"));
}
