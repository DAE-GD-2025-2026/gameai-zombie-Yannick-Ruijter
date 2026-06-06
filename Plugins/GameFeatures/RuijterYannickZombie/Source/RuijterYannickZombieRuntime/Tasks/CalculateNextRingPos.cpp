#include "CalculateNextRingPos.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCalculateNextRingPos::UCalculateNextRingPos()
{
	NodeName = TEXT("Calculate Next Ring Position");
}

EBTNodeResult::Type UCalculateNextRingPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackBoard = AIController ? AIController->GetBlackboardComponent() : nullptr;
	if (!BlackBoard) return EBTNodeResult::Failed;

	FVector Current = BlackBoard->GetValueAsVector(TargetKey);

	// Move in current direction by SideLength
	SideLength += SideIncrease;
	Current.X += CurrentDirection.X * SideLength;
	Current.Y += CurrentDirection.Y * SideLength;

	// Turn left (rotate 90 degrees)
	CurrentDirection = FVector2D(-CurrentDirection.Y, CurrentDirection.X);

	// Every 2 turns, increase the side length
	SidesWalked++;
	if (SidesWalked % 2 == 0)
		SideIncrease += StepSize;

	BlackBoard->SetValueAsVector(TargetKey, Current);
	return EBTNodeResult::Succeeded;
}

FString UCalculateNextRingPos::GetStaticDescription() const
{
	return FString::Printf(TEXT("Calculates next ring search position -> %s"), *TargetKey.ToString());
}