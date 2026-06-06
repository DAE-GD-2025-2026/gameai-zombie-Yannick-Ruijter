#include "TakeDistanceFromZombies.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "RuijterYannickZombieRuntime/StudentPerceptor.h"

UTakeDistanceFromZombies::UTakeDistanceFromZombies()
{
	bNotifyTick = true;
	NodeName = TEXT("Move to location");
}

EBTNodeResult::Type UTakeDistanceFromZombies::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto Pawn = Cast<ASurvivorPawn>(AIController->GetPawn());
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto Perceptor = Cast<UStudentPerceptor>(BlackBoard->GetValueAsObject("Perceptor"));
	Pawn->StartRunning();
	//gets average location of zombies in neighborhood
	auto LocationToAvoid = Perceptor->GetAverageZombieLocation();
	auto Dir = (Pawn->GetActorLocation() - LocationToAvoid);
	Dir.Normalize();
	TargetLocation = (Dir * Distance) + Pawn->GetActorLocation();
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld());
	FNavLocation ProjectedLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
		TargetLocation = ProjectedLocation.Location;
	AIController->MoveToLocation(TargetLocation, 50.f);
	return EBTNodeResult::InProgress;
}

void UTakeDistanceFromZombies::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto Pawn = AIController->GetPawn();
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto Perceptor = Cast<UStudentPerceptor>(BlackBoard->GetValueAsObject("Perceptor"));
	auto LocationToAvoid = Perceptor->GetAverageZombieLocation();
	auto SurvivorLocation = Pawn->GetActorLocation();
	auto Dir = (SurvivorLocation - LocationToAvoid);
	Dir.Normalize();
	TargetLocation = (Dir * Distance) + SurvivorLocation;
	
	//make sure it falls within the navmesh
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld());
	FNavLocation ProjectedLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
		TargetLocation = ProjectedLocation.Location;
	
	double distanceSquared{FVector2D::DistSquared(
		FVector2D{SurvivorLocation}, FVector2D{TargetLocation})
	};
	
	//is pickuprange squared :)
	if (distanceSquared < 1000)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		auto survivor = Cast<ASurvivorPawn>(Pawn);
		survivor->StopRunning();
	};
	
	AIController->MoveToLocation(TargetLocation, 50.f);
}

FString UTakeDistanceFromZombies::GetStaticDescription() const
{
	return FString::Printf(TEXT("Moves away from nearby zombies"));
}
