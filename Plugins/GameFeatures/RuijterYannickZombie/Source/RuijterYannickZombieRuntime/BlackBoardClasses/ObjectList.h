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
	static const FDataType InvalidValue;

	static FDataType GetValue(const UObjectList* KeyOb, const uint8* RawData);
	static bool SetValue(UObjectList* KeyOb, uint8* RawData, const FDataType& Value);

	virtual FString DescribeValue(const UBlackboardComponent& OwnerComp, const uint8* RawData) const override;
};