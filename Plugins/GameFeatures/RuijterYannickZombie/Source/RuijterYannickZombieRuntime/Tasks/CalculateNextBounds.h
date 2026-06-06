#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Village/House/House.h"
#include "CalculateNextBounds.generated.h"


UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UCalculateNextBounds : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCalculateNextBounds();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName LastHouseSpottedKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

private:
	int32 CurrentStep = 0; // 0 = center, 1-4 = corners
	AHouse* PreviousHouse = nullptr;
};