// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombies/BaseZombie.h"

UEnemyInRange::UEnemyInRange()
{
}

bool UEnemyInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto const AIController = OwnerComp.GetAIOwner();
	auto const BlackBoard = OwnerComp.GetBlackboardComponent();
	float constexpr SightRadius = 1000.f;
	return false;
}