#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "EnemyInRange.generated.h"

UCLASS()
class UEnemyInRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UEnemyInRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};