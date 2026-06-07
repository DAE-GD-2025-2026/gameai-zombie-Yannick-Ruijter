#include "IsHealthUnderThresholdRuijterYannick.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UIsHealthUnderThresholdRuijterYannick::UIsHealthUnderThresholdRuijterYannick()
{
}

bool UIsHealthUnderThresholdRuijterYannick::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Survivor = Cast<ASurvivorPawn>(OwnerComp.GetAIOwner()->GetPawn());
	auto HealthComponent = Survivor->FindComponentByClass<UHealthComponent>();
	int ThresholdHealth = ThresHoldPercentage / 10.f;
	return HealthComponent->GetHealth() <= ThresholdHealth;
}
