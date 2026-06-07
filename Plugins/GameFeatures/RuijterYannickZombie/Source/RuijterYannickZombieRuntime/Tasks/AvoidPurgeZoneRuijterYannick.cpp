// BTTask_AvoidPurgeZones.cpp
#include "AvoidPurgeZoneRuijterYannick.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "RuijterYannickZombieRuntime/StudentPerceptorRuijterYannick.h"

// You'll need to include your Perceptor class header
// #include "SurvivorPerceptor.h"

UAvoidPurgeZones::UAvoidPurgeZones()
{
    NodeName = TEXT("Avoid Purge Zones");
    bNotifyTick = true;
}

EBTNodeResult::Type UAvoidPurgeZones::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    FVector TargetLocation = Blackboard->GetValueAsVector("TargetLocation");
    FVector SurvivorLocation = Pawn->GetActorLocation();

    auto Perceptor = Cast<UStudentPerceptorRuijterYannick>(Blackboard->GetValueAsObject("Perceptor"));
    if (!Perceptor) return EBTNodeResult::Succeeded;
    
    ZoneToAvoidLocation = Blackboard->GetValueAsVector("PurgeZoneLocation");

    SafeDestination = SurvivorLocation + (SurvivorLocation - ZoneToAvoidLocation).GetSafeNormal() * AvoidanceRadius * 1.1f;
    
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
    if (NavSys)
    {
        FNavLocation ProjectedLocation;
        if (NavSys->ProjectPointToNavigation(SafeDestination, ProjectedLocation, FVector(200.f, 200.f, 200.f)))
        {
            SafeDestination = ProjectedLocation.Location;
        }
    }
    
    GEngine->AddOnScreenDebugMessage(99, 1.f, FColor::Red,
                                     FString::Printf(TEXT("Dodging zone")));

    AIController->MoveToLocation(SafeDestination);
    return EBTNodeResult::InProgress;
}

void UAvoidPurgeZones::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    
    auto CurrentLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
    auto DistSquared{(ZoneToAvoidLocation - CurrentLocation).SquaredLength()};
    
    if (DistSquared > AvoidanceRadius * AvoidanceRadius)
    {
        auto AIController = OwnerComp.GetAIOwner();
        auto BlackBoard = AIController->GetBlackboardComponent();
        BlackBoard->SetValueAsBool("InPurgeZone", false);
        AIController->MoveToLocation(BlackBoard->GetValueAsVector("TargetLocation"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

FString UAvoidPurgeZones::GetStaticDescription() const
{
    return FString::Printf(TEXT("Checks PurgeZonesSpotted and detours around any zone\nwithin %.0f units of the path to TargetLocation.\nDanger radius: %.0f units."),
        AvoidanceRadius, DangerRadius);
}