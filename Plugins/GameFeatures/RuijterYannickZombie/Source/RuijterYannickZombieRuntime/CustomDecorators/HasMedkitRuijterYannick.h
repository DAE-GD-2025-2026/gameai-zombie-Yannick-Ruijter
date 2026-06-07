#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "HasMedkitRuijterYannick.generated.h"

UCLASS()
class UHasMedkitRuijterYannick : public UBTDecorator
{
	GENERATED_BODY()
public:
	UHasMedkitRuijterYannick();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};