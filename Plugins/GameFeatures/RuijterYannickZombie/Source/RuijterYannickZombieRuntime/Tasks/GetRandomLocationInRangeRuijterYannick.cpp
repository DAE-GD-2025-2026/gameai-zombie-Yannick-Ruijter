#include "GetRandomLocationInRangeRuijterYannick.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UGetRandomLocationInRangeRuijterYannick::UGetRandomLocationInRangeRuijterYannick()
{
    bNotifyTick = true;
	NodeName = TEXT("Move to location");
}

EBTNodeResult::Type UGetRandomLocationInRangeRuijterYannick::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto randomLocation = CalculateRandomPoint(AIController);
	BlackBoard->SetValueAsVector(TargetKey, randomLocation);
	return EBTNodeResult::Succeeded;
}

FString UGetRandomLocationInRangeRuijterYannick::GetStaticDescription() const
{
	return FString::Printf(TEXT("Moves to location with blackboardKey %s"), *TargetKey.ToString());
}

FVector UGetRandomLocationInRangeRuijterYannick::CalculateRandomPoint(AAIController* AIController) const
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
