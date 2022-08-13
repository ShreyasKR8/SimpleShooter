// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MoveToGunRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Gun.h"
#include "MyCharacter.h"
#include "ShooterAIController.h"

UBTService_MoveToGunRange::UBTService_MoveToGunRange() 
{
    NodeName = TEXT("Move To Relevant Gun Range");
}

void UBTService_MoveToGunRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); //GetPlayerPawn returns the pointer to a playerpawn.
    AMyCharacter* AICharacter = Cast<AMyCharacter> (OwnerComp.GetAIOwner()->GetPawn());
    AGun* Gun = AICharacter->GetGun();

    if(AICharacter == nullptr || PlayerCharacter == nullptr || Gun == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Null pointer in BTService_MoveToGunRange.cpp"))
        return;
    }
    
    float Distance = (PlayerCharacter->GetActorLocation() - AICharacter->GetActorLocation()).Size();
    UE_LOG(LogTemp, Warning, TEXT("Distance = %f"), Distance)
    if(Distance > Gun->GetMaxRange())
    {

        // OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), );
        UE_LOG(LogTemp, Warning, TEXT("it is greater!"))
    }
    // AICharacter->MoveToLocation(PlayerCharacter->GetActorLocation(), )
}
