#include "ExploreVillageTaskRuijterYannick.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"

UExploreVillageTaskRuijterYannick::UExploreVillageTaskRuijterYannick()
{
	bNotifyTick = true;
	NodeName = TEXT("Explore village");
}

EBTNodeResult::Type UExploreVillageTaskRuijterYannick::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	APawn* Survivor = OwnerComp.GetAIOwner()->GetPawn();
	if (!Survivor) return EBTNodeResult::Failed;
	CalculateRandomPoint(AIController);
	StudentPerceptor = NewObject<UStudentPerceptorRuijterYannick>(this);

	
	return EBTNodeResult::InProgress;
}

void UExploreVillageTaskRuijterYannick::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto AIController = OwnerComp.GetAIOwner();
	APawn* Survivor = OwnerComp.GetAIOwner()->GetPawn();
	double distanceSquared{FVector2D::DistSquared(
		FVector2D{Survivor->GetActorLocation()}, FVector2D{LastRandomPoint})
	};
	
	if (distanceSquared < 1000) CalculateRandomPoint(AIController);
}

FString UExploreVillageTaskRuijterYannick::GetStaticDescription() const
{
	return FString::Printf(TEXT("Explores village"));
}

void UExploreVillageTaskRuijterYannick::CalculateRandomPoint(AAIController* AIController)
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
