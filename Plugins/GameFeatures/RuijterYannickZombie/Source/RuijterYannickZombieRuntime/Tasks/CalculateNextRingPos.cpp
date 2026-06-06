#include "CalculateNextRingPos.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCalculateNextRingPos::UCalculateNextRingPos()
{
	NodeName = TEXT("Calculate Next Ring Position");
}

#include "NavigationSystem.h"

EBTNodeResult::Type UCalculateNextRingPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackBoard = AIController ? AIController->GetBlackboardComponent() : nullptr;
	if (!BlackBoard) return EBTNodeResult::Failed;

	FVector Current = BlackBoard->GetValueAsVector(TargetKey);

	Current.X += CurrentDirection.X * SideLength;
	Current.Y += CurrentDirection.Y * SideLength;

	CurrentDirection = FVector2D(-CurrentDirection.Y, CurrentDirection.X);

	SidesWalked++;
	if (SidesWalked % 2 == 0)
		SideLength += StepSize;

	// Project to nearest navmesh point
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld());
	FNavLocation ProjectedLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(Current, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
		Current = ProjectedLocation.Location;

	BlackBoard->SetValueAsVector(TargetKey, Current);
	return EBTNodeResult::Succeeded;
}

FString UCalculateNextRingPos::GetStaticDescription() const
{
	return FString::Printf(TEXT("Calculates next ring search position -> %s"), *TargetKey.ToString());
}