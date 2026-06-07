#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "IsStaminaUnderThresholdRuijterYannick.generated.h"
UCLASS()
class UIsStaminaUnderThresholdRuijterYannick : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsStaminaUnderThresholdRuijterYannick();
	UPROPERTY(EditAnywhere)
	int ThresHoldPercentage = 30;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};