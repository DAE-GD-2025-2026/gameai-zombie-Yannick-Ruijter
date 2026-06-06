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
    UPROPERTY()
	TArray<ABaseZombie*> ZombiesInRange{};
    UPROPERTY()
	TArray<ABaseZombie*> CloseZombies{};
    UPROPERTY()
	TArray<ABaseItem*> ItemsSpotted{};
    UPROPERTY()
	TArray<AHouse*> HousesSpotted{};
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    FVector GetAverageZombieLocation();
	ABaseZombie* GetClosestZombie();
	ABaseItem* GetNeededItem();
	void UpdateNeededItems();
	
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	AHouse* GetNextHouse() const;
	void AdvanceHouseIndex();

private:
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY()
	ASurvivorPawn* SurvivorPawn;
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	UPROPERTY()
	TArray<ABaseItem*> PreviousInventory;
	int CurrentHouseIndex{};
	bool CycleHouses{false};
	static constexpr int32 HouseCycleThreshold = 10;
};
