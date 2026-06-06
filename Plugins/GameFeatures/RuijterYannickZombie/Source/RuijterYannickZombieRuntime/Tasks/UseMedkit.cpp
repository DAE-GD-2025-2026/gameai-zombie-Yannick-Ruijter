#include "UseMedkit.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"

UUseMedkit::UUseMedkit()
{
	bNotifyTick = true;
	NodeName = TEXT("Eats food");
}

EBTNodeResult::Type UUseMedkit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto InventoryComponent = AIController->GetPawn()->FindComponentByClass<UInventoryComponent>();
	auto Inventory = InventoryComponent->GetInventory();
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i] == nullptr) continue;
		if (Inventory[i]->GetItemType() == EItemType::Medkit)
		{
			InventoryComponent->UseItem(i);
			InventoryComponent->RemoveItem(i);
			break;
		}
	}
	return EBTNodeResult::Succeeded;
}

FString UUseMedkit::GetStaticDescription() const
{
	return FString::Printf(TEXT("Uses medkit"));
}
