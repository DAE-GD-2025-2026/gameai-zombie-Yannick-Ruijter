#include "FaceZombieRuijterYannick.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Zombies/BaseZombie.h"

UFaceZombieRuijterYannick::UFaceZombieRuijterYannick()
{
	bNotifyTick = true;
	NodeName = TEXT("Aim at zombie");
}

EBTNodeResult::Type UFaceZombieRuijterYannick::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto TargetZombie = Cast<ABaseZombie>(BlackBoard->GetValueAsObject("ClosestEnemy"));
	if (TargetZombie == nullptr) return EBTNodeResult::Failed;
	AIController->SetFocus(Cast<AActor>(TargetZombie));
	return EBTNodeResult::Succeeded;
}

FString UFaceZombieRuijterYannick::GetStaticDescription() const
{
	return FString::Printf(TEXT("Aims in the direction of the zombie"));
}
