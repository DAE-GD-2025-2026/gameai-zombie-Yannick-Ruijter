// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/BaseItem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Zombies/BaseZombie.h"
#include "Village/House/House.h"
#include "StudentPerceptor.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUIJTERYANNICKZOMBIERUNTIME_API UStudentPerceptor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptor();
	TArray<ABaseZombie*> ZombiesInRange{};
	TArray<ABaseItem*> ItemsSpotted{};
	TArray<AHouse*> HousesSpotted{};
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    FVector GetAverageZombieLocation();
	ABaseZombie* GetClosestZombie();
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
private:
	UBlackboardComponent* BlackboardComponent;
	int CurrentHouseIndex{};
};
