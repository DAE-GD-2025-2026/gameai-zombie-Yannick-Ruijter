#include "ObjectList.h"

const UObjectList* UObjectList::StaticInstance = nullptr;
const TArray<UObject*> UObjectList::InvalidValue = {};

UObjectList::FDataType UObjectList::GetValue(const UObjectList* KeyOb, const uint8* RawData)
{
	if (!RawData) return InvalidValue;
	return *reinterpret_cast<const FDataType*>(RawData);
}

bool UObjectList::SetValue(UObjectList* KeyOb, uint8* RawData, const FDataType& Value)
{
	*reinterpret_cast<FDataType*>(RawData) = Value;
	return true;
}

FString UObjectList::DescribeValue(const UBlackboardComponent& OwnerComp, const uint8* RawData) const
{
	FString Result = FString::Printf(TEXT("List of pointers to objects"));
	return Result;
}