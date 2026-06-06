// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Shotgun.h"
#include "Perception/AIPerceptionComponent.h"
#include "Village/House/House.h"


//TODO make village spotted -> house spotted
//save the last 5 houses that were spotted and compare them to the current one spotted
//implement better searching for villages
//implement healing when weak
//implement fleeing when seeing zombie but have no weapon
//fix multiple items in sight
UStudentPerceptor::UStudentPerceptor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor::BeginPlay()
{
	Super::BeginPlay();

	SurvivorPawn = Cast<ASurvivorPawn>(GetOwner());
	InventoryComponent = SurvivorPawn->FindComponentByClass<UInventoryComponent>();
	PreviousInventory = InventoryComponent->GetInventory();
	if (auto PerceptionComp = SurvivorPawn->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor::OnPerceptionUpdated);
	}
	BlackboardComponent = Cast<AAIController>(Cast<APawn>(GetOwner())->GetController())->GetBlackboardComponent();
	BlackboardComponent->SetValueAsObject("Perceptor", this);
}

void UStudentPerceptor::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto CurrentInventory = InventoryComponent->GetInventory();
	for (int i = 0; i < CurrentInventory.Num(); i++)
	{
		if (CurrentInventory[i] != PreviousInventory[i])
			UpdateNeededItems();	
	}

	bool ItemSpotted = BlackboardComponent->GetValueAsBool("ItemSpotted");
	if (!ItemSpotted && ItemsSpotted.Num() > 0)
	{
		BlackboardComponent->SetValueAsObject("LastSpottedItem", ItemsSpotted[0]);
		BlackboardComponent->SetValueAsBool("ItemSpotted", true);
	}

	bool EnemySpotted = BlackboardComponent->GetValueAsBool("EnemySpotted");
	if (!EnemySpotted && ZombiesInRange.Num() > 0)
	{
		BlackboardComponent->SetValueAsBool("EnemySpotted", GetClosestZombie() != nullptr);
	}
}

FVector UStudentPerceptor::GetAverageZombieLocation()
{
	FVector Location{};
	for (int i{ZombiesInRange.Num() - 1}; i >= 0; i--)
	{
		if (ZombiesInRange[i] == nullptr) ZombiesInRange.RemoveAt(i);
		Location += ZombiesInRange[i]->GetActorLocation();
	}
	Location /= ZombiesInRange.Num();
	return Location;
}

ABaseZombie* UStudentPerceptor::GetClosestZombie()
{
	double ClosestDistance = FLT_MAX;
	int ClosestIndex = -1;
	for (int i{ZombiesInRange.Num() - 1}; i >= 0; i--)
	{
		if (ZombiesInRange[i] == nullptr) ZombiesInRange.RemoveAt(i);
		double CurrentDist{(ZombiesInRange[i]->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength()};
		if (CurrentDist < ClosestDistance)
		{
			ClosestDistance = CurrentDist;
			ClosestIndex = i;
		}
	}
	if (ClosestIndex == -1) return nullptr;
	return ZombiesInRange[ClosestIndex];
}

ABaseItem* UStudentPerceptor::GetNeededItem()
{
    auto Inventory = InventoryComponent->GetInventory();

    int WeaponCount = 0, FewestAmmo = 100000, LastWeaponIndex = -1;
    int FoodCount = 0, LastFoodIndex = -1;
    int MedkitCount = 0, LastMedkitIndex = -1;
    int FreeSlot = -1;

    for (int i = 0; i < Inventory.Num(); i++)
    {
        if (Inventory[i] == nullptr)
        {
            FreeSlot = i;
            break;
        }

        switch (Inventory[i]->GetItemType())
        {
        case EItemType::Food:
            FoodCount++;
            LastFoodIndex = i;
            break;
        case EItemType::Medkit:
            MedkitCount++;
            LastMedkitIndex = i;
            break;
        case EItemType::Shotgun:
        case EItemType::Pistol:
            WeaponCount++;
            if (Inventory[i]->GetValue() < FewestAmmo)
            {
                FewestAmmo = Inventory[i]->GetValue();
                LastWeaponIndex = i;
            }
            break;
        case EItemType::Garbage:
        	InventoryComponent->RemoveItem(i);
        	FreeSlot = i;
        	break;
        default:
            break;
        }
    }

    auto SetSlotAndReturn = [&](ABaseItem* Item, int SlotIndex) -> ABaseItem*
    {
        BlackboardComponent->SetValueAsInt("FreeItemSlot", SlotIndex);
        return Item;
    };

    for (ABaseItem* Item : ItemsSpotted)
    {
        if (!Item) continue;
        EItemType ItemType = Item->GetItemType();
        if (ItemType == EItemType::Garbage) continue;

        if (FreeSlot != -1)
            return SetSlotAndReturn(Item, FreeSlot);

        switch (ItemType)
        {
        case EItemType::Pistol:
        case EItemType::Shotgun:
        {
            auto Weapon = Cast<AWeapon>(Item);
            if (WeaponCount <= 1)
            {
                int SlotToReplace = MedkitCount >= FoodCount ? LastMedkitIndex : LastFoodIndex;
                return SetSlotAndReturn(Item, SlotToReplace);
            }
            if (Weapon->GetValue() > FewestAmmo)
                return SetSlotAndReturn(Item, LastWeaponIndex);
            break;
        }
        case EItemType::Medkit:
            if (MedkitCount < 1)
            {
                int SlotToReplace = WeaponCount > 2 ? LastWeaponIndex : LastFoodIndex;
                return SetSlotAndReturn(Item, SlotToReplace);
            }
            break;
        case EItemType::Food:
            if (FoodCount < 2)
            {
                int SlotToReplace = WeaponCount > 2 ? LastWeaponIndex : LastMedkitIndex;
                return SetSlotAndReturn(Item, SlotToReplace);
            }
            break;
        default:
            break;
        }
    }

    return nullptr;
}

void UStudentPerceptor::UpdateNeededItems()
{
	auto Inv = InventoryComponent->GetInventory();
	for (int i = 0; i < Inv.Num(); ++i)
	{
		PreviousInventory[i] = Inv[i];
	}
	
	auto ItemNeeded = GetNeededItem();
	BlackboardComponent->SetValueAsBool("ItemNeeded", ItemNeeded != nullptr);
	BlackboardComponent->SetValueAsObject("NeededItem", ItemNeeded);
	if (ItemNeeded != nullptr)
	{
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation ProjectedLocation;
		FVector ItemLocation = ItemNeeded->GetActorLocation();
    
		if (NavSys && NavSys->ProjectPointToNavigation(ItemLocation, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
			BlackboardComponent->SetValueAsVector("TargetLocation", ProjectedLocation.Location);
		else
			BlackboardComponent->SetValueAsVector("TargetLocation", ItemLocation);
	}
}

void UStudentPerceptor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green,
	                                 FString::Printf(TEXT("Saw Something!")));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!AIController) return;

	UBlackboardComponent* blackBoard = AIController->GetBlackboardComponent();
	if (!blackBoard) return;

	FAISenseID DamageSenseID = UAISense::GetSenseID<UAISense_Damage>();
	if (Stimulus.WasSuccessfullySensed() && Stimulus.Type == DamageSenseID)
	{
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red,
		                                 FString::Printf(TEXT("Sensing Damage")));
		blackBoard->SetValueAsBool("GotDamaged", true);
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		if (auto Zombie = Cast<ABaseZombie>(Actor))
		{
			blackBoard->SetValueAsBool("EnemySpotted", true);
			ZombiesInRange.Add(Zombie);
			blackBoard->SetValueAsObject("ClosestEnemy", GetClosestZombie());
		}
		auto Item = Cast<ABaseItem>(Actor);
		if (Item && Item->GetItemType() != EItemType::Garbage)
		{
			ItemsSpotted.AddUnique(Item);
			UpdateNeededItems();
		}

		if (auto SensedHouse = Cast<AHouse>(Actor))
		{
			if (HousesSpotted.Contains(SensedHouse)) return;
			HousesSpotted.Insert(SensedHouse, CurrentHouseIndex);
			blackBoard->SetValueAsObject("LastSpottedHouse", SensedHouse);
			blackBoard->SetValueAsBool("HouseSpotted", true);
		}
	}
	else
	{
		//if we lost sight of item
		if (auto Item = Cast<ABaseItem>(Actor))
		{
			ItemsSpotted.Remove(Item);
		}
	}
}
