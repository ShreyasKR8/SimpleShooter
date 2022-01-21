// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	int32 PullTrigger(int32 RemainingAmmo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 EmpAmmo = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 ShotgunAmmo = 100;


	float GetMaxRange(); //For use in BTService_MoveToGunRange.cpp
	
private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 20;

	UPROPERTY(VisibleAnywhere) //VisibleAnywhere for pointers
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere) //VisibleAnywhere for pointers
	USkeletalMeshComponent* Mesh; 

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* MagEmptySound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* BulletTrail;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;
};
