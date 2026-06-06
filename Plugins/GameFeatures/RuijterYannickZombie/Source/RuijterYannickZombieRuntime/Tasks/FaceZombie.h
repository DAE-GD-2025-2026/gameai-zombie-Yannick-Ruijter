#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FaceZombie.generated.h"

UCLASS()
class RUIJTERYANNICKZOMBIERUNTIME_API UFaceZombie : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UFaceZombie();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
protected:
};