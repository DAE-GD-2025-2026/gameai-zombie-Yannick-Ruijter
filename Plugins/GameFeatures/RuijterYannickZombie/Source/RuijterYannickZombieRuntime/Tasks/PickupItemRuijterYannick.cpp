#include "PickupItemRuijterYannick.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Common/InventoryComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "RuijterYannickZombieRuntime/StudentPerceptorRuijterYannick.h"
#include "Survivor/SurvivorPawn.h"

UPickupItemRuijterYannick::UPickupItemRuijterYannick()
{
    bNotifyTick = true;
	NodeName = TEXT("Pick Up Item");
}

EBTNodeResult::Type UPickupItemRuijterYannick::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

FString UPickupItemRuijterYannick::GetStaticDescription() const
{
	return FString::Printf(TEXT("Shoot gun in current direction"));
}
