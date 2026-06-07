#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "IsHealthUnderThresholdRuijterYannick.generated.h"
UCLASS()
class UIsHealthUnderThresholdRuijterYannick : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsHealthUnderThresholdRuijterYannick();
	UPROPERTY(EditAnywhere)
	int ThresHoldPercentage = 30;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};