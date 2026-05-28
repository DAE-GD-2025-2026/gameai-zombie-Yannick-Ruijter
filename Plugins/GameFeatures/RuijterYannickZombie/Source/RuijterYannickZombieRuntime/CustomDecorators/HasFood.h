#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "HasFood.generated.h"

UCLASS()
class UHasFood : public UBTDecorator
{
	GENERATED_BODY()
public:
	UHasFood();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};