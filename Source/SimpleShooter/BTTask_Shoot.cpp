// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Shoot::UBTTask_Shoot() 
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    AMyCharacter* AICharacter = Cast<AMyCharacter> (OwnerComp.GetAIOwner()->GetPawn());
    // AMyCharacter* PlayerCharacter = Cast<AMyCharacter> (UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if(AICharacter == nullptr)   
    {
        UE_LOG(LogTemp, Warning, TEXT("Bruh.."))
        return EBTNodeResult::Failed;
    }

    AICharacter->Shoot();
    UE_LOG(LogTemp, Warning, TEXT("AIShot"));

    return EBTNodeResult::Succeeded;
}