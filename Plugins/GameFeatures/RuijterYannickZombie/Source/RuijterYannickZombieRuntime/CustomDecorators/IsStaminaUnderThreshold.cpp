#include "IsStaminaUnderThreshold.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UIsStaminaUnderThreshold::UIsStaminaUnderThreshold()
{
}

bool UIsStaminaUnderThreshold::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Survivor = Cast<ASurvivorPawn>(OwnerComp.GetAIOwner()->GetPawn());
	auto HealthComponent = Survivor->FindComponentByClass<UStaminaComponent>();
	int ThresholdHealth = HealthComponent->GetMaxStamina() / 100 * ThresHoldPercentage;
	return HealthComponent->GetCurrentStamina() <= ThresholdHealth;
}
