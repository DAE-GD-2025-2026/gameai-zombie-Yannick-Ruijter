// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"

#include "AIController.h"
#include "IDetailTreeNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Items/Pistol.h"
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
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
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
	for (int i {ZombiesInRange.Num() - 1}; i >= 0 ; i--)
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
	for (int i {ZombiesInRange.Num() - 1}; i >= 0 ; i--)
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
		if ( auto Zombie = Cast<ABaseZombie>(Actor))
		{
			blackBoard->SetValueAsBool("EnemySpotted", true);
			ZombiesInRange.Add(Zombie);
			blackBoard->SetValueAsObject("ClosestEnemy", GetClosestZombie());
		}
		auto Item = Cast<ABaseItem>(Actor);
		if (Item && Item->GetItemType() != EItemType::Garbage)
		{
			ItemsSpotted.AddUnique(Item);
			blackBoard->SetValueAsObject("LastSpottedItem", Item);
			blackBoard->SetValueAsBool("ItemSpotted", true);
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
