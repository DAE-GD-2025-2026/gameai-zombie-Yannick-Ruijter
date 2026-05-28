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
	if (!RawData) return false;
	*reinterpret_cast<FDataType*>(RawData) = Value;
	return true;
}

FString UObjectList::DescribeValue(const UBlackboardComponent& OwnerComp, const uint8* RawData) const
{
	const FDataType* ObjectList = reinterpret_cast<const FDataType*>(RawData);
	if (!ObjectList || ObjectList->Num() == 0)
	{
		return TEXT("Empty List");
	}

	FString Result = FString::Printf(TEXT("List[%d]: "), ObjectList->Num());
	for (const UObject* Obj : *ObjectList)
	{
		if (Obj == nullptr) continue;
		Result += Obj->GetName() + TEXT(", ");
	}
	return Result;
}