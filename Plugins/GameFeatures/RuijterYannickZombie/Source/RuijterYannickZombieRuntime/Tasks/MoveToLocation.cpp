#include "MoveToLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UMoveToLocationTask::UMoveToLocationTask()
{
    bNotifyTick = true;
	NodeName = TEXT("Move to location");
}

EBTNodeResult::Type UMoveToLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto BlackBoard = AIController->GetBlackboardComponent();
	TargetLocation = BlackBoard->GetValueAsVector(TargetKey);
	AIController->MoveTo(TargetLocation);
	
	return EBTNodeResult::InProgress;
}

void UMoveToLocationTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* Survivor = OwnerComp.GetAIOwner()->GetPawn();
	
	double distanceSquared{FVector2D::DistSquared(
		FVector2D{Survivor->GetActorLocation()}, FVector2D{TargetLocation})
	};
	
	//is pickuprange squared :)
	if (distanceSquared < 1000)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

FString UMoveToLocationTask::GetStaticDescription() const
{
	return FString::Printf(TEXT("Moves to location with blackboardKey %s"), *TargetKey.ToString());
}

void UMoveToLocationTask::CalculateRandomPoint(AAIController* AIController)
{
	auto NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation RandomLocation;
	NavSys->GetRandomPointInNavigableRadius(
		AIController->GetPawn()->GetActorLocation(),
		1000.f,
		RandomLocation
	);

	LastRandomPoint = RandomLocation.Location;
	AIController->MoveTo(LastRandomPoint);
}
