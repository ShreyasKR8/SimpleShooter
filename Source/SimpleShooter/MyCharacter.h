// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class AGun;
class AAmmoBox;

UCLASS()
class SIMPLESHOOTER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	float Health = 100;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintReadWrite)
	bool Shooting = false; //For Recoil animation

	UFUNCTION(BlueprintPure) //BlueprintPure removes the need for an execution pin in blueprints.
	bool IsDead() const;

	UFUNCTION(BlueprintPure) //BlueprintPure removes the need for an execution pin in blueprints.
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure) //BlueprintPure removes the need for an execution pin in blueprints.
	AGun* GetCurrentWeapon() const;

	UFUNCTION(BlueprintPure) //BlueprintPure removes the need for an execution pin in blueprints.
	int32 GetRemainingAmmo() const; //For BP_HUD.

	UFUNCTION(BlueprintCallable) //BlueprintPure removes the need for an execution pin in blueprints.
	bool IsRifle(); //For Recoil animation
	
	void Shoot();

	void StopFiring();

	void ChangeWeapon();

	void AddAmmo(int32 AmmoBox, int32 ShotgunAmmoBox, AActor* OtherActor);

	void AddAmmo(int32 AmmoQuantity);

	AGun* GetGun();

	void SpawnLoot();

	// float* GetHealth(); //For HealthBox.cpp
	float GetHealth(); //For HealthBox.cpp

	UPROPERTY()
	TArray<AGun*> Weapons;

	UPROPERTY()
	TArray<int32> TotalAmmo;

	UPROPERTY()
	int32 WeaponIndex;

	// UPROPERTY()
	// int32 Kills;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Actors")
	TSubclassOf<AActor> LootMesh;

	UPROPERTY(VisibleAnywhere) //VisibleAnywhere for pointers
	USkeletalMeshComponent* SkelMesh;

private:

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	// UPROPERTY(VisibleAnywhere)
	// float Health;

	void MoveForwardBackward(float AxisValue);

	void MoveLeftRight(float AxisValue);

	void LookUpDownRate(float AxisValue);

	void LookLeftRightRate(float AxisValue);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> RifleClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> ShotgunClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AGun>>WeaponClass;

	UPROPERTY()
	AGun* Gun;
	
	UPROPERTY()
	AGun* CurrentWeapon;

	UPROPERTY()
	AGun* Shotgun;
};