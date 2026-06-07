#include "TakeDistanceFromZombiesRuijterYannick.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "RuijterYannickZombieRuntime/StudentPerceptorRuijterYannick.h"

UTakeDistanceFromZombiesRuijterYannick::UTakeDistanceFromZombiesRuijterYannick()
{
	bNotifyTick = true;
	NodeName = TEXT("Move to location");
}

EBTNodeResult::Type UTakeDistanceFromZombiesRuijterYannick::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto Pawn = Cast<ASurvivorPawn>(AIController->GetPawn());
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto Perceptor = Cast<UStudentPerceptorRuijterYannick>(BlackBoard->GetValueAsObject("Perceptor"));
	Pawn->StartRunning();
	//gets average location of zombies in neighborhood
	auto LocationToAvoid = Perceptor->GetAverageZombieLocation();
	auto Dir = GetCurvedFleeDir(Pawn->GetActorLocation(), LocationToAvoid);
	TargetLocation = (Dir * Distance) + Pawn->GetActorLocation();
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld());
	FNavLocation ProjectedLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
		TargetLocation = ProjectedLocation.Location;
	AIController->MoveToLocation(TargetLocation, 50.f);
	return EBTNodeResult::InProgress;
}

void UTakeDistanceFromZombiesRuijterYannick::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto Pawn = AIController->GetPawn();
	auto Perceptor = Cast<UStudentPerceptorRuijterYannick>(AIController->GetBlackboardComponent()->GetValueAsObject("Perceptor"));

	auto ZombieLocation = Perceptor->GetAverageZombieLocation();
	if (ZombieLocation.ContainsNaN())
	{
		Cast<ASurvivorPawn>(Pawn)->StopRunning();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	auto SurvivorLocation = Pawn->GetActorLocation();

	// Finish when far enough from the zombie, not from the target
	double distFromZombiesSq = FVector2D::DistSquared(FVector2D{SurvivorLocation}, FVector2D{ZombieLocation});
	if (distFromZombiesSq > Distance * Distance)
	{
		Cast<ASurvivorPawn>(Pawn)->StopRunning();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	auto Dir = GetCurvedFleeDir(SurvivorLocation, ZombieLocation);
	TargetLocation = (Dir * Distance) + SurvivorLocation;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld());
	FNavLocation ProjectedLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
		TargetLocation = ProjectedLocation.Location;

	AIController->MoveToLocation(TargetLocation, 50.f);
}

FString UTakeDistanceFromZombiesRuijterYannick::GetStaticDescription() const
{
	return FString::Printf(TEXT("Moves away from nearby zombies"));
}

FVector UTakeDistanceFromZombiesRuijterYannick::GetCurvedFleeDir(const FVector& From, const FVector& To)
{
	FVector Dir = (From - To).GetSafeNormal();
	// Perpendicular in the XY plane
	FVector Perp = FVector(-Dir.Y, Dir.X, 0.f).GetSafeNormal();
	
	return (Dir + Perp * 0.4f).GetSafeNormal();
}
