#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UseMedkit.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UUseMedkit : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UUseMedkit();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
protected:
};