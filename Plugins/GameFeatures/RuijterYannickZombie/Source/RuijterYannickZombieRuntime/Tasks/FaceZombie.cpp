#include "FaceZombie.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Zombies/BaseZombie.h"

UFaceZombie::UFaceZombie()
{
	bNotifyTick = true;
	NodeName = TEXT("Aim at zombie");
}

EBTNodeResult::Type UFaceZombie::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto TargetZombie = Cast<ABaseZombie>(BlackBoard->GetValueAsObject("ClosestEnemy"));
	AIController->SetFocus(Cast<AActor>(TargetZombie));
	return EBTNodeResult::Succeeded;
}

FString UFaceZombie::GetStaticDescription() const
{
	return FString::Printf(TEXT("Aims in the direction of the zombie"));
}
