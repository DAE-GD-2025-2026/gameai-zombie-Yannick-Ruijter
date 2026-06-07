#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "HasFoodRuijterYannick.generated.h"

UCLASS()
class UHasFoodRuijterYannick : public UBTDecorator
{
	GENERATED_BODY()
public:
	UHasFoodRuijterYannick();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};