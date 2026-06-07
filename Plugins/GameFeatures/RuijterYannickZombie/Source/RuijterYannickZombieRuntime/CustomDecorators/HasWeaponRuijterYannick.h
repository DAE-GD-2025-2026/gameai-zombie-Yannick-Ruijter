#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "HasWeaponRuijterYannick.generated.h"

UCLASS()
class UHasWeaponRuijterYannick : public UBTDecorator
{
	GENERATED_BODY()
public:
	UHasWeaponRuijterYannick();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};