#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CalculateNextRingPos.generated.h"

USTRUCT()
struct FRingSearchMemory
{
	GENERATED_BODY()

	int32 RingIndex = 1;   // Current ring (1 = first ring around start)
	int32 Side = 0;        // 0=North, 1=East, 2=South, 3=West
	int32 StepOnSide = 0;  // How far along the current side we are
	FVector Origin = FVector::ZeroVector;
	bool bOriginSet = false;
};

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UCalculateNextRingPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCalculateNextRingPos();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetKey;

	UPROPERTY(EditAnywhere, Category = "Ring Search")
	float StepSize = 1000.f;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

private:
	float SideLength = 0.f;
	float SideIncrease = 1000.f;
	int32 SidesWalked = 0;
	FVector2D CurrentDirection = FVector2D(1, 0);
};