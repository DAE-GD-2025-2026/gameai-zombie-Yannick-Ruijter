// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../BlackBoardClasses/ObjectList.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Zombies/BaseZombie.h"

UEnemyInRange::UEnemyInRange()
{
}

bool UEnemyInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto const AIController = OwnerComp.GetAIOwner();
	auto const BlackBoard = OwnerComp.GetBlackboardComponent();
	auto EnemiesInRange = BlackBoard->GetValue<UObjectList>("EnemiesInRange");
	float constexpr SightRadius = 1000.f;
	
	//doing it backwards so i can remove elements while looping over it
	for (int32 i = EnemiesInRange.Num() - 1; i >= 0; i--)
	{
		ABaseZombie* Zombie = Cast<ABaseZombie>(EnemiesInRange[i]);

		// Remove if null or out of sight range
		if (!Zombie || FVector::DistSquared(Zombie->GetActorLocation(), AIController->GetPawn()->GetActorLocation()) > SightRadius * SightRadius)
		{
			EnemiesInRange.RemoveAt(i);
		}
	}
	
	//set it back since it's just a copy that we're working on
	BlackBoard->SetValue<UObjectList>("EnemiesInRange", EnemiesInRange);
	return EnemiesInRange.Num() > 0;
}