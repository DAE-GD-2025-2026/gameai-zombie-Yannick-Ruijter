// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Shotgun.h"
#include "Perception/AIPerceptionComponent.h"
#include "Village/House/House.h"

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
		{
			UpdateNeededItems();
			GEngine->AddOnScreenDebugMessage(12, 1.f, FColor::Green,
			FString::Printf(TEXT("Item Changed")));
			break;
		}
	}
	
	BlackboardComponent->SetValueAsBool("EnemyClose", false);
	for (int i = CloseZombies.Num() - 1; i >= 0; i--)
	{
		if (!IsValid(CloseZombies[i]))
		{
			CloseZombies.RemoveAt(i);
			continue;
		}
		auto DistanceSquared = (CloseZombies[i]->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
		if (DistanceSquared < 500 * 500)
		{
			BlackboardComponent->SetValueAsBool("EnemyClose", true);
			break;
		}
		CloseZombies.RemoveAt(i);		
	}
	
	if (BlackboardComponent->GetValueAsBool("SetNextHouse"))
	{
		BlackboardComponent->SetValueAsBool("SetNextHouse", false);
		AdvanceHouseIndex();
	}
	
	if (!BlackboardComponent->GetValueAsBool("InPurgeZone"))
	{
		for (int i = PurgeZonesSpotted.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(PurgeZonesSpotted[i]))
			{
				PurgeZonesSpotted.RemoveAt(i);
				continue;
			}
		
			auto DistSquared{(PurgeZonesSpotted[i]->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength()};
			if (DistSquared < 150 * 150)
			{
				BlackboardComponent->SetValueAsBool("InPurgeZone", true);
				BlackboardComponent->SetValueAsVector("PurgeZoneLocation", PurgeZonesSpotted[i]->GetActorLocation());
				break;
			}
		}
	}
}

FVector UStudentPerceptor::GetAverageZombieLocation()
{
	FVector Location = FVector::ZeroVector;
	int32 ValidCount = 0;

	for (int i = CloseZombies.Num() - 1; i >= 0; i--)
	{
		if (!IsValid(CloseZombies[i]))
		{
			CloseZombies.RemoveAt(i);
			continue;
		}
		Location += CloseZombies[i]->GetActorLocation();
		ValidCount++;
	}

	if (ValidCount == 0) return FVector::ZeroVector;

	return Location / ValidCount;
}

ABaseZombie* UStudentPerceptor::GetClosestZombie()
{
	double ClosestDistance = FLT_MAX;
	ABaseZombie* ClosestZombie{nullptr};
	for (int i{ZombiesInRange.Num() - 1}; i >= 0; i--)
	{
		if (!IsValid(ZombiesInRange[i]))
		{
			ZombiesInRange.RemoveAt(i);
			continue;
		}
		double CurrentDist{(ZombiesInRange[i]->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength()};
		if (CurrentDist < ClosestDistance)
		{
			ClosestDistance = CurrentDist;
			ClosestZombie = ZombiesInRange[i];
		}
	}
	
	return ClosestZombie;
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
        if (!IsValid(Inventory[i]))
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
            {
	            return SetSlotAndReturn(Item, LastWeaponIndex);
            }
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
	GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Green,
									 FString::Printf(TEXT("Updating Items...")));
	auto Inv = InventoryComponent->GetInventory();
	
	for (int i = 0; i < Inv.Num(); ++i)
	{
		PreviousInventory[i] = Inv[i];
		if (ItemsSpotted.Contains(Inv[i]))
			ItemsSpotted.Remove(Inv[i]);
	}
	
	auto ItemNeeded = GetNeededItem();
	BlackboardComponent->SetValueAsBool("ItemNeeded", ItemNeeded != nullptr);
	BlackboardComponent->SetValueAsObject("NeededItem", ItemNeeded);
	if (ItemNeeded != nullptr)
	{
		FString ItemTypeStr = UEnum::GetValueAsString(ItemNeeded->GetItemType());

		GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Green,
			FString::Printf(TEXT("Item needed %s"), *ItemTypeStr));
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation ProjectedLocation;
		FVector ItemLocation = ItemNeeded->GetActorLocation();
    
		if (NavSys && NavSys->ProjectPointToNavigation(ItemLocation, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
			BlackboardComponent->SetValueAsVector("TargetLocation", ProjectedLocation.Location);
		else
			BlackboardComponent->SetValueAsVector("TargetLocation", ItemLocation);
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(8, 1.f, FColor::Green,
			FString::Printf(TEXT("No item needed")));
	}
	GEngine->AddOnScreenDebugMessage(11, 1.f, FColor::Green,
			FString::Printf(TEXT("Nr Items In Sight %i"), ItemsSpotted.Num()));
}

void UStudentPerceptor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	FAISenseID DamageSenseID = UAISense::GetSenseID<UAISense_Damage>();
	if (Stimulus.WasSuccessfullySensed() && Stimulus.Type == DamageSenseID)
	{
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red,
		                                 FString::Printf(TEXT("Sensing Damage")));
		BlackboardComponent->SetValueAsBool("GotDamaged", true);
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		if (auto PurgeZone = Cast<APurgeZone>(Actor))
		{
			PurgeZonesSpotted.AddUnique(PurgeZone);
			BlackboardComponent->SetValueAsBool("PurgeZoneSpotted", true);
			
			GEngine->AddOnScreenDebugMessage(55, 1.f, FColor::Red,
											 FString::Printf(TEXT("Spotted a Purge Zone!")));
		}
		if (auto Zombie = Cast<ABaseZombie>(Actor))
		{
			CloseZombies.AddUnique(Zombie);
			if (ZombiesInRange.Contains(Zombie)) return;
			BlackboardComponent->SetValueAsBool("EnemySpotted", true);
			ZombiesInRange.Add(Zombie);
			BlackboardComponent->SetValueAsObject("ClosestEnemy", GetClosestZombie());
			
			GEngine->AddOnScreenDebugMessage(6, 1.f, FColor::Green,
											 FString::Printf(TEXT("Spotted a zombie!")));
		}
		auto Item = Cast<ABaseItem>(Actor);
		if (Item && Item->GetItemType() != EItemType::Garbage)
		{
			if (ItemsSpotted.Contains(Item))
				return;
			ItemsSpotted.Add(Item);
			GEngine->AddOnScreenDebugMessage(7, 1.f, FColor::Green,
											 FString::Printf(TEXT("Spotted an item!")));
			UpdateNeededItems();
		}

		if (auto SensedHouse = Cast<AHouse>(Actor))
		{
			if (HousesSpotted.Contains(SensedHouse)) return;
			GEngine->AddOnScreenDebugMessage(23, 1.f, FColor::Green,
											 FString::Printf(TEXT("Spotted a new house: %i houses remembered spotted in total!"), HousesSpotted.Num()));
			
			HousesSpotted.Add(SensedHouse);

			// If we just crossed the threshold, enable infinite cycling
			if (!CycleHouses && HousesSpotted.Num() >= HouseCycleThreshold)
				CycleHouses = true;
			

			BlackboardComponent->SetValueAsObject("LastSpottedHouse", GetNextHouse());
			BlackboardComponent->SetValueAsBool("HouseSpotted", GetNextHouse() != nullptr);
		}
	}
	else
	{
		if (auto Zombie = Cast<ABaseZombie>(Actor))
		{
			ZombiesInRange.Remove(Zombie);

			if (ZombiesInRange.IsEmpty())
			{
				BlackboardComponent->SetValueAsBool("EnemySpotted", false);
				BlackboardComponent->SetValueAsObject("ClosestEnemy", nullptr);
			}
			else
			{
				BlackboardComponent->SetValueAsObject("ClosestEnemy", GetClosestZombie());
			}
		}
	}
}

AHouse* UStudentPerceptor::GetNextHouse() const
{
	if (HousesSpotted.IsEmpty()) return nullptr;

	// Not enough houses yet and we've exhausted the list
	if (!CycleHouses && CurrentHouseIndex >= HousesSpotted.Num())
		return nullptr;

	int32 Index = CurrentHouseIndex % HousesSpotted.Num();
	return HousesSpotted[Index];
}

void UStudentPerceptor::AdvanceHouseIndex()
{
	if (HousesSpotted.IsEmpty() || CurrentHouseIndex >= HousesSpotted.Num())
	{
		BlackboardComponent->SetValueAsObject("LastSpottedHouse", nullptr);
		return;
	}
	
	CurrentHouseIndex++;
	GEngine->AddOnScreenDebugMessage(33, 1.f, FColor::Green,
									 FString::Printf(TEXT("Increased house index to %i!"), CurrentHouseIndex));

	// Stop advancing if below threshold and list exhausted
	if (!CycleHouses && CurrentHouseIndex >= HousesSpotted.Num())
	{
		BlackboardComponent->SetValueAsBool("HouseSpotted", false);
		return;
	}

	// Wrap around only if cycling is enabled
	if (CycleHouses)
		CurrentHouseIndex = CurrentHouseIndex % HousesSpotted.Num();

	AHouse* Next = GetNextHouse();
	if (Next)
	{
		BlackboardComponent->SetValueAsObject("LastSpottedHouse", Next);
		BlackboardComponent->SetValueAsBool("HouseSpotted", true);
	}
}