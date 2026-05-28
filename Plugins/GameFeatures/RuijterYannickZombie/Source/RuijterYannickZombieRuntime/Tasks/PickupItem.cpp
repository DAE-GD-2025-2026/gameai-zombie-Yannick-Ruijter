#include "PickupItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UPickupItem::UPickupItem()
{
    bNotifyTick = true;
	NodeName = TEXT("Pick Up Item");
}

EBTNodeResult::Type UPickupItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto BlackBoard = AIController->GetBlackboardComponent();
	return EBTNodeResult::Succeeded;
}

FString UPickupItem::GetStaticDescription() const
{
	return FString::Printf(TEXT("Picks up the item specified in blackboard"));
}
