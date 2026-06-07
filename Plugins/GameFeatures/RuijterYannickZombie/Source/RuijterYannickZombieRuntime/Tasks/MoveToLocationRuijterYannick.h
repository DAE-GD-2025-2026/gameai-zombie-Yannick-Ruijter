#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RuijterYannickZombieRuntime/StudentPerceptorRuijterYannick.h"
#include "MoveToLocationRuijterYannick.generated.h"

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
	UPROPERTY()
	ABaseItem* ItemToPickUp{nullptr};
	UPROPERTY()
	UInventoryComponent* Inventory{nullptr};
	FVector TargetLocation;
	FVector LastRandomPoint{};
	void CalculateRandomPoint(AAIController* AIController);
};