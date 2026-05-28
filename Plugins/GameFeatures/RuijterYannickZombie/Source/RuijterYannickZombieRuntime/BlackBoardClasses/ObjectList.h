#pragma once
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "ObjectList.generated.h"

UCLASS()
class UObjectList : public UBlackboardKeyType
{
	GENERATED_BODY()
public:
	typedef TArray<UObject*> FDataType;
	static const UObjectList* StaticInstance;

	virtual FString DescribeValue(const UBlackboardComponent& OwnerComp, const uint8* RawData) const override;
};