#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "IsHealthUnderThreshold.generated.h"
UCLASS()
class UIsHealthUnderThreshold : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsHealthUnderThreshold();
	UPROPERTY(EditAnywhere)
	int ThresHoldPercentage = 30;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};