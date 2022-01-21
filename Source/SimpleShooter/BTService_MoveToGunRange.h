// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_MoveToGunRange.generated.h"

/**
 * 
 */
class AGun;
UCLASS()
class SIMPLESHOOTER_API UBTService_MoveToGunRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_MoveToGunRange();

protected: 
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


	
};
