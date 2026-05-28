#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "IsItemNeeded.generated.h"

UCLASS()
class UIsItemNeeded : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsItemNeeded();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};