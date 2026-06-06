#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RotateCharacter.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API URotateCharacter : public UBTService
{
	GENERATED_BODY()

public:
	URotateCharacter();

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationSpeed = 90.f;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	AActor* DummyActor = nullptr;
	float CurrentAngle = 0.f;
};