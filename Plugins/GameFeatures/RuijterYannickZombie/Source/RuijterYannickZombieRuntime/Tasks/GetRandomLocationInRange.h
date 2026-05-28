#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetRandomLocationInRange.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UGetRandomLocationInRange : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UGetRandomLocationInRange();
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float RangeRadius = 10000.f;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
protected:
	FVector CalculateRandomPoint(AAIController* AIController) const;
};