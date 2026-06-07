#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetRandomLocationInRangeRuijterYannick.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UGetRandomLocationInRangeRuijterYannick : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UGetRandomLocationInRangeRuijterYannick();
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float RangeRadius = 10000.f;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
protected:
	FVector CalculateRandomPoint(AAIController* AIController) const;
};