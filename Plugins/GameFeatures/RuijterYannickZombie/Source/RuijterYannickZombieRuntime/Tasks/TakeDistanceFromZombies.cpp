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
	auto Pawn = AIController->GetPawn();
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto Perceptor = Cast<UStudentPerceptor>(BlackBoard->GetValueAsObject("Perceptor"));
	
	//gets average location of zombies in neighborhood
	auto LocationToAvoid = Perceptor->GetAverageZombieLocation();
	auto Dir = (Pawn->GetActorLocation() - LocationToAvoid);
	Dir.Normalize();
	TargetLocation = (Dir * Distance) + Pawn->GetActorLocation();
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
	
	double distanceSquared{FVector2D::DistSquared(
		FVector2D{SurvivorLocation}, FVector2D{TargetLocation})
	};
	
	//is pickuprange squared :)
	if (distanceSquared < 1000)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

FString UTakeDistanceFromZombies::GetStaticDescription() const
{
	return FString::Printf(TEXT("Moves away from nearby zombies"));
}
