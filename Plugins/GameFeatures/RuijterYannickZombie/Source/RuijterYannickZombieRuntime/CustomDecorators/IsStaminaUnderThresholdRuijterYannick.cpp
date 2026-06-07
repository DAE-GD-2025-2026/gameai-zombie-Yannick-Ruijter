#include "IsStaminaUnderThresholdRuijterYannick.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UIsStaminaUnderThresholdRuijterYannick::UIsStaminaUnderThresholdRuijterYannick()
{
}

bool UIsStaminaUnderThresholdRuijterYannick::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Survivor = Cast<ASurvivorPawn>(OwnerComp.GetAIOwner()->GetPawn());
	auto HealthComponent = Survivor->FindComponentByClass<UStaminaComponent>();
	auto ThresholdHealth = ThresHoldPercentage / 10.f;
	return HealthComponent->GetCurrentStamina() <= ThresholdHealth;
}
