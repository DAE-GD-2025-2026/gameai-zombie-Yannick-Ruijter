#include "CalculateNextBounds.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCalculateNextBounds::UCalculateNextBounds()
{
	NodeName = TEXT("Explore House");
}

EBTNodeResult::Type UCalculateNextBounds::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackBoard = AIController ? AIController->GetBlackboardComponent() : nullptr;
	if (!BlackBoard) return EBTNodeResult::Failed;

	UObject* HouseObject = BlackBoard->GetValueAsObject(LastHouseSpottedKey);
	AHouse* House = Cast<AHouse>(HouseObject);
	if (!House) return EBTNodeResult::Failed;

	// Reset if the house changed
	if (House != PreviousHouse)
	{
		CurrentStep = 0;
		PreviousHouse = House;
	}

	FHouseBounds Bounds = House->GetBounds();
	Bounds.Extent *= 1.1;
	FVector Target;

	if (CurrentStep == 0)
	{
		Target = Bounds.Origin;
	}
	else
	{
		//go to next corner of the house
		const float X = (CurrentStep == 1 || CurrentStep == 2) ? Bounds.Extent.X : -Bounds.Extent.X;
		const float Y = (CurrentStep == 1 || CurrentStep == 4) ? Bounds.Extent.Y : -Bounds.Extent.Y;
		Target = Bounds.Origin + FVector(X, Y, 0);
	}

	BlackBoard->SetValueAsVector(TargetKey, Target);

	//if we've gone to every corner/center pos
	CurrentStep++;
	if (CurrentStep > 4)
	{
		CurrentStep = 0;
		BlackBoard->SetValueAsBool("HouseSpotted", false);
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}

FString UCalculateNextBounds::GetStaticDescription() const
{
	return FString::Printf(TEXT("Explores house corners -> %s"), *TargetKey.ToString());
}