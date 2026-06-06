// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"

#include "AIController.h"
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
}

FVector UStudentPerceptor::GetAverageZombieLocation()
{
	FVector Location{};
	for (auto& Zombie : ZombiesInRange) 
		Location += Zombie->GetActorLocation();
	Location /= ZombiesInRange.Num();
	return Location;
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
		blackBoard->SetValueAsBool(FName("GotDamaged"), true);
	}
	
	if (Stimulus.WasSuccessfullySensed())
	{
		if ( auto Zombie = Cast<ABaseZombie>(Actor))
		{
			blackBoard->SetValueAsBool(FName("EnemySpotted"), true);
			ZombiesInRange.Add(Zombie);
		}
		auto Item = Cast<ABaseItem>(Actor);
		if (Item && Item->GetItemType() != EItemType::Garbage)
		{
			ItemsSpotted.AddUnique(Item);
			blackBoard->SetValueAsObject(FName("LastSpottedItem"), Item);
			blackBoard->SetValueAsBool(FName("ItemSpotted"), true);
		}
		
		if (auto SensedHouse = Cast<AHouse>(Actor))
		{
			blackBoard->SetValueAsVector(FName("TargetLocation"), SensedHouse->GetActorLocation());
			blackBoard->SetValueAsBool(FName("VillageSpotted"), true);
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
