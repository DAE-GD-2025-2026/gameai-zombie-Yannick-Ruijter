#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "IsItemInRangeNeeded.generated.h"

UCLASS()
class UIsItemInRangeNeeded : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsItemInRangeNeeded();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};