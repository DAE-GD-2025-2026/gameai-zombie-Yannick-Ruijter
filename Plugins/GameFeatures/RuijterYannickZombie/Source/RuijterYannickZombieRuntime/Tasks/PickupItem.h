#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PickupItem.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UPickupItem : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPickupItem();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
protected:
};