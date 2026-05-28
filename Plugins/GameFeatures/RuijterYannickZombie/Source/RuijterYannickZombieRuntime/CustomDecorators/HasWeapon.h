#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "HasWeapon.generated.h"

UCLASS()
class UHasWeapon : public UBTDecorator
{
	GENERATED_BODY()
public:
	UHasWeapon();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};