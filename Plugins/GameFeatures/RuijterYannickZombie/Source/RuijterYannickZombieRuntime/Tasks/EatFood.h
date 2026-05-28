#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EatFood.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UEatFood : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UEatFood();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
protected:
};