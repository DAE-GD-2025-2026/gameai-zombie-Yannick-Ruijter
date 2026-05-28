// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"

#include "AIController.h"


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
}

void UStudentPerceptor::SetOwner(AAIController* owner)
{
	Controller = owner;
}

void UStudentPerceptor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Stimulus.WasSuccessfullySensed()) return;

	auto Blackboard = Controller->GetBlackboardComponent();
	if (!Blackboard) return;
	
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something! %s"), *Actor->GetName()));
}
