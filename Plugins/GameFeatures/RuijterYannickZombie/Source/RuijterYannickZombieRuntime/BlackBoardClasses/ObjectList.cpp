#include "ObjectList.h"

const UObjectList* UObjectList::StaticInstance = nullptr;

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