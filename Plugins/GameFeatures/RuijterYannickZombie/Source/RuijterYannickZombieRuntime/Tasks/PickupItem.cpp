#include "PickupItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Common/InventoryComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "RuijterYannickZombieRuntime/StudentPerceptor.h"
#include "Survivor/SurvivorPawn.h"

UPickupItem::UPickupItem()
{
    bNotifyTick = true;
	NodeName = TEXT("Pick Up Item");
}

EBTNodeResult::Type UPickupItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto ItemToPickup = BlackBoard->GetValueAsObject("NeededItem");
	int SlotIndex = BlackBoard->GetValueAsInt("FreeItemSlot");
	auto Inventory = Cast<ASurvivorPawn>(AIController->GetPawn())->FindComponentByClass<UInventoryComponent>();
	if (Inventory->GetInventory()[SlotIndex] != nullptr)
		Inventory->RemoveItem(SlotIndex);
	Inventory->GrabItem(SlotIndex, Cast<ABaseItem>(ItemToPickup));
	return EBTNodeResult::Succeeded;
}

FString UPickupItem::GetStaticDescription() const
{
	return FString::Printf(TEXT("Shoot gun in current direction"));
}
