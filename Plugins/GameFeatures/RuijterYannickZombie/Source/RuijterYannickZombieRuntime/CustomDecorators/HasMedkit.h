#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "HasMedkit.generated.h"

UCLASS()
class UHasMedkit : public UBTDecorator
{
	GENERATED_BODY()
public:
	UHasMedkit();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};