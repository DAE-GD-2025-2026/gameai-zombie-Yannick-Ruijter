#include "RotateCharacter.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Survivor/SurvivorPawn.h"

URotateCharacter::URotateCharacter()
{
	NodeName = TEXT("Rotate Character");
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void URotateCharacter::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	DummyActor = AIController->GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FTransform::Identity, Params);
    
	// Give it a root component so SetActorLocation works
	USceneComponent* Root = NewObject<USceneComponent>(DummyActor);
	DummyActor->SetRootComponent(Root);
	Root->RegisterComponent();

	AIController->SetFocus(DummyActor, EAIFocusPriority::Gameplay);
}

void URotateCharacter::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController) AIController->ClearFocus(EAIFocusPriority::Gameplay);

	if (DummyActor)
	{
		DummyActor->Destroy();
		DummyActor = nullptr;
	}
}

void URotateCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto Pawn = Cast<ASurvivorPawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Pawn || !DummyActor) return;
	// Orbit dummy around character at a fixed radius
	CurrentAngle += RotationSpeed * DeltaSeconds;
	const float Radius = 100.f;
	FVector NewPos = Pawn->GetActorLocation() + FVector(
		FMath::Cos(FMath::DegreesToRadians(CurrentAngle)) * Radius,
		FMath::Sin(FMath::DegreesToRadians(CurrentAngle)) * Radius,
		0.f
	);
	DummyActor->SetActorLocation(NewPos);
}