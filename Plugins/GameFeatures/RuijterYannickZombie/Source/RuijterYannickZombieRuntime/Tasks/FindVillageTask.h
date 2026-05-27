#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindVillageTask.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UFindVillageTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UFindVillageTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual FString GetStaticDescription() const override;

protected:
	FVector LastRandomPoint{};
	
	void CalculateRandomPoint(AAIController* AIController);
};