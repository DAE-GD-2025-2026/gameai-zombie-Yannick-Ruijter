// BTTask_AvoidPurgeZones.cpp
#include "AvoidPurgeZone.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "RuijterYannickZombieRuntime/StudentPerceptor.h"

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

    auto Perceptor = Cast<UStudentPerceptor>(Blackboard->GetValueAsObject("Perceptor"));
    if (!Perceptor) return EBTNodeResult::Succeeded;
    
    ZoneToAvoidLocation = Blackboard->GetValueAsVector("PurgeZoneLocation");

    SafeDestination = SurvivorLocation + (SurvivorLocation - ZoneToAvoidLocation).Normalize() * AvoidanceRadius * 1.1f;
    
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

bool UAvoidPurgeZones::IsSegmentIntersectingZone(
    const FVector& Start,
    const FVector& End,
    const FVector& ZoneCenter,
    float Radius) const
{
    //more efficient to work with 2D stuff
    FVector2D Start2D(Start.X, Start.Y);
    FVector2D End2D(End.X, End.Y);
    FVector2D Center2D(ZoneCenter.X, ZoneCenter.Y);

    //direction from current location to target location
    FVector2D SegmentDir = End2D - Start2D;
    float SegmentLen = SegmentDir.Size();

    if (SegmentLen < KINDA_SMALL_NUMBER)
    {
        return FVector2D::Distance(Start2D, Center2D) < Radius;
    }

    FVector2D SegmentDirNorm = SegmentDir / SegmentLen;
    FVector2D ToCenter = Center2D - Start2D;

    // Project zone center onto segment
    float T = FVector2D::DotProduct(ToCenter, SegmentDirNorm);
    T = FMath::Clamp(T, 0.f, SegmentLen);

    FVector2D ClosestPoint = Start2D + SegmentDirNorm * T;
    float DistToSegment = FVector2D::Distance(ClosestPoint, Center2D);

    return DistToSegment < Radius;
}

FVector UAvoidPurgeZones::ComputeDetourPoint(
    const FVector& AgentLocation,
    const FVector& TargetLocation,
    const FVector& ZoneCenter,
    float Radius) const
{
    // Find the closest point on our path to the zone center
    FVector PathDir = (TargetLocation - AgentLocation).GetSafeNormal2D();
    FVector ToCenter = ZoneCenter - AgentLocation;

    float ProjectedDist = FVector::DotProduct(ToCenter, PathDir);
    FVector ClosestOnPath = AgentLocation + PathDir * ProjectedDist;

    // Build a perpendicular direction to step around the zone
    FVector PerpDir = FVector::CrossProduct(PathDir, FVector::UpVector).GetSafeNormal();

    // Try both sides, pick the one closer to the target
    FVector DetourLeft  = ZoneCenter + PerpDir  * Radius;
    FVector DetourRight = ZoneCenter - PerpDir  * Radius;

    float DistLeft  = FVector::Dist(DetourLeft,  TargetLocation);
    float DistRight = FVector::Dist(DetourRight, TargetLocation);

    return (DistLeft < DistRight) ? DetourLeft : DetourRight;
}

FString UAvoidPurgeZones::GetStaticDescription() const
{
    return FString::Printf(TEXT("Checks PurgeZonesSpotted and detours around any zone\nwithin %.0f units of the path to TargetLocation.\nDanger radius: %.0f units."),
        AvoidanceRadius, DangerRadius);
}