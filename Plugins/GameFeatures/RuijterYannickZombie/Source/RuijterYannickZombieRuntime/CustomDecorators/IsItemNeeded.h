#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "Items/BaseItem.h"
#include "IsItemNeeded.generated.h"

UCLASS()
class UIsItemNeeded : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsItemNeeded();

protected:
	enum class ItemType
	{
		Weapon,
		Medkit,
		Food,
		Garbage,
	};
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	static ItemType GetItemType(ABaseItem* Item);
};