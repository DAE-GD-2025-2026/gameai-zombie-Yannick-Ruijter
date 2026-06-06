#include "IsHealthUnderThreshold.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UIsHealthUnderThreshold::UIsHealthUnderThreshold()
{
}

bool UIsHealthUnderThreshold::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Survivor = Cast<ASurvivorPawn>(OwnerComp.GetAIOwner()->GetPawn());
	auto HealthComponent = Survivor->FindComponentByClass<UHealthComponent>();
	int ThresholdHealth = ThresHoldPercentage / 10.f;
	return HealthComponent->GetHealth() <= ThresholdHealth;
}
