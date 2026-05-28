#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RuijterYannickZombieRuntime/StudentPerceptor.h"
#include "MoveToLocation.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UMoveToLocationTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMoveToLocationTask();
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetKey;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual FString GetStaticDescription() const override;
protected:
	FVector TargetLocation;
	FVector LastRandomPoint{};
	void CalculateRandomPoint(AAIController* AIController);
};