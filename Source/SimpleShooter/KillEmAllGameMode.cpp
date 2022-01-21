// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);

    UE_LOG(LogTemp, Warning, TEXT("A pawn was killed"));
    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if(PlayerController != nullptr)
    {
        EndGame(false);
    }
    //For loop over ShooterAI in World:
        //Is not dead?
            //return
    // End game
    for(AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        if(!AIController->IsDead())
        {
            return;
        }
    } 
    EndGame(true);//True because the player has won
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) 
{
    for(AController* Controller : TActorRange<AController>(GetWorld()))
    {
        /*bool : True if PlayerController and bIsPlayerWinner, True if not PlayerController and not bIsPlayerWinner, 
        False if PlayerController and not IsPlayerWinner, False if not PlayerController and bIsPlayerWinner*/
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
