// BTTask_AvoidPurgeZones.h
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AvoidPurgeZone.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UAvoidPurgeZones : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UAvoidPurgeZones();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

private:
	// How far to detour around a purge zone (should be > 100 units danger radius)
	UPROPERTY(EditAnywhere, Category = "Purge Zone Avoidance")
	float AvoidanceRadius = 150.f;

	// The danger radius — if within this, you die
	UPROPERTY(EditAnywhere, Category = "Purge Zone Avoidance")
	float DangerRadius = 100.f;
	
	FVector ZoneToAvoidLocation = FVector::ZeroVector;
	
	FVector SafeDestination = FVector::ZeroVector;

	bool IsSegmentIntersectingZone(const FVector& Start, const FVector& End,
								   const FVector& ZoneCenter, float Radius) const;

	FVector ComputeDetourPoint(const FVector& AgentLocation, const FVector& TargetLocation,
							   const FVector& ZoneCenter, float Radius) const;
};