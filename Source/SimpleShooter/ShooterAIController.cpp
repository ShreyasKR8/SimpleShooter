// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"
#include "AmmoBox.h"

AShooterAIController::AShooterAIController()
{
    // PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component")); 
    //https://forums.unrealengine.com/t/quick-ai-perception-jumpstart-c/27452
    // SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISight Config"));
    // PerceptionComp->ConfigureSense(*SightConfig); //void ConfigureSense(UAISenseConfig& SenseConfig);
}

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

