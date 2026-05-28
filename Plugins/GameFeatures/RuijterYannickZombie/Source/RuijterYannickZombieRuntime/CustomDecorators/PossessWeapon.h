#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "PossessWeapon.generated.h"

UCLASS()
class UPossessWeapon : public UBTDecorator
{
	GENERATED_BODY()
public:
	UPossessWeapon();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};