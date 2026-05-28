#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "IsStaminaUnderThreshold.generated.h"
UCLASS()
class UIsStaminaUnderThreshold : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsStaminaUnderThreshold();
	UPROPERTY(EditAnywhere)
	int ThresHoldPercentage = 30;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};