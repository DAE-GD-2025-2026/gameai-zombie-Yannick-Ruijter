#include "SetVillageCenterRuijterYannick.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USetVillageCenterRuijterYannick::USetVillageCenterRuijterYannick()
{
	NodeName = TEXT("Explore House");
}

EBTNodeResult::Type USetVillageCenterRuijterYannick::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackBoard = AIController ? AIController->GetBlackboardComponent() : nullptr;
	if (!BlackBoard) return EBTNodeResult::Failed;

	UObject* HouseObject = BlackBoard->GetValueAsObject(LastHouseSpottedKey);
	AHouse* House = Cast<AHouse>(HouseObject);
	if (!House)
	{
		BlackBoard->SetValueAsBool("HouseSpotted", false);
		return EBTNodeResult::Failed;
	}

	// Reset if the house changed
	if (House == PreviousHouse)
	{
		BlackBoard->SetValueAsBool("HouseSpotted", false);
		return EBTNodeResult::Failed;
	}

	FHouseBounds Bounds = House->GetBounds();
	Bounds.Extent *= 1.1; //the bounds might be inside the walls of the house
	FVector Target;
	
	Target = Bounds.Origin;
	

	BlackBoard->SetValueAsVector(TargetKey, Target);

	return EBTNodeResult::Succeeded;
}

FString USetVillageCenterRuijterYannick::GetStaticDescription() const
{
	return FString::Printf(TEXT("Explores house corners -> %s"), *TargetKey.ToString());
}