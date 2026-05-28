#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TakeDistanceFromZombies.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UTakeDistanceFromZombies : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTakeDistanceFromZombies();
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Distance;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual FString GetStaticDescription() const override;
protected:
	FVector TargetLocation;
};