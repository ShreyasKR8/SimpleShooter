// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"
#include "AmmoBox.h"

void AShooterAIController::BeginPlay() 
{
    Super::BeginPlay();
    if(AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
    }
    APawn* AIPawn = AController::GetPawn();  //GetPlayerPawn returns the pointer to a playerpawn.
    GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), AIPawn->GetActorLocation());
}

void AShooterAIController::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);
}

bool AShooterAIController::IsDead() const
{
    AMyCharacter* ControlledCharacter = Cast<AMyCharacter>(GetPawn());
    if(ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }
    return true;//If controller doesn't exist, it's obviously dead.
}