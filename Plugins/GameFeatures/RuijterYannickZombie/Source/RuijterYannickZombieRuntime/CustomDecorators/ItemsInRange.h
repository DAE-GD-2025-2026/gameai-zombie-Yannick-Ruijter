#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "ItemsInRange.generated.h"

UCLASS()
class UItemsInRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UItemsInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};