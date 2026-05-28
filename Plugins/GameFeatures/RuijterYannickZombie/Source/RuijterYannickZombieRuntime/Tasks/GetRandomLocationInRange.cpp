#include "GetRandomLocationInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UGetRandomLocationInRange::UGetRandomLocationInRange()
{
    bNotifyTick = true;
	NodeName = TEXT("Move to location");
}

EBTNodeResult::Type UGetRandomLocationInRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto randomLocation = CalculateRandomPoint(AIController);
	BlackBoard->SetValueAsVector(TargetKey, randomLocation);
	return EBTNodeResult::Succeeded;
}

FString UGetRandomLocationInRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("Moves to location with blackboardKey %s"), *TargetKey.ToString());
}

FVector UGetRandomLocationInRange::CalculateRandomPoint(AAIController* AIController) const
{
	auto NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation RandomLocation;
	NavSys->GetRandomPointInNavigableRadius(
		AIController->GetPawn()->GetActorLocation(),
		RangeRadius,
		RandomLocation
	);

	return RandomLocation.Location;
}
