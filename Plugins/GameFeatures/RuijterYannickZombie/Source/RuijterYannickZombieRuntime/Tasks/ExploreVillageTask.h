#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RuijterYannickZombieRuntime/StudentPerceptor.h"
#include "ExploreVillageTask.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UExploreVillageTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UExploreVillageTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual FString GetStaticDescription() const override;
protected:
	FVector LastRandomPoint{};
	UPROPERTY()
	TObjectPtr<UStudentPerceptor> StudentPerceptor;
	void CalculateRandomPoint(AAIController* AIController);
};