#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CalculateNextRingPosRuijterYannick.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UCalculateNextRingPosRuijterYannick : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCalculateNextRingPosRuijterYannick();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetKey;

	UPROPERTY(EditAnywhere, Category = "Ring Search")
	float StepSize = 1000.f;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

private:
	float SideLength = 0.f;
	int32 SidesWalked = 0;
	FVector2D CurrentDirection = FVector2D(1, 0);
	FVector CurrentTargetLocation = FVector(0, 0, 0);
};