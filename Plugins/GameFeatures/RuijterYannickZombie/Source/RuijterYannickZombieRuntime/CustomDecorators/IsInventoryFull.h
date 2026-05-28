#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "IsInventoryFull.generated.h"

UCLASS()
class UIsInventoryFull : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsInventoryFull();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};