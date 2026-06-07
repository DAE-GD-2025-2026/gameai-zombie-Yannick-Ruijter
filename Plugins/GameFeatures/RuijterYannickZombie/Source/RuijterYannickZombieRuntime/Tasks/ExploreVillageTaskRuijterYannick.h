#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RuijterYannickZombieRuntime/StudentPerceptorRuijterYannick.h"
#include "ExploreVillageTaskRuijterYannick.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UExploreVillageTaskRuijterYannick : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UExploreVillageTaskRuijterYannick();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual FString GetStaticDescription() const override;
protected:
	FVector LastRandomPoint{};
	UPROPERTY()
	TObjectPtr<UStudentPerceptorRuijterYannick> StudentPerceptor;
	void CalculateRandomPoint(AAIController* AIController);
};