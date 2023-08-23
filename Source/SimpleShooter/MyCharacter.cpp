// Fill out your copyright notice in the Description page of Project Settings.

//Perhaps make a different class for enemy character with different characterstics...?
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Gun.h"
#include "AmmoBox.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = true;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	WeaponIndex = 0;

	WeaponClass.SetNum(2);
	WeaponClass[0] = RifleClass;
	WeaponClass[1] = ShotgunClass;

	Weapons.SetNum(2);
	Weapons[WeaponIndex] = Gun = GetWorld()->SpawnActor<AGun>(RifleClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Weapons[WeaponIndex]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Weapons[WeaponIndex]->SetOwner(this);

	TotalAmmo.SetNum(2);
	TotalAmmo[0] = Gun->EmpAmmo;
	TotalAmmo[1] = Gun->ShotgunAmmo;
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward/Backward"), this, &AMyCharacter::MoveForwardBackward);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft/Right"), this, &AMyCharacter::MoveLeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp/Down"), this, &APawn::AddControllerPitchInput); //APawn because AddControllerPitchInput() belongs to class APawn
	PlayerInputComponent->BindAxis(TEXT("LookLeft/Right"), this, &APawn::AddControllerYawInput); //APawn because AddControllerYawInput() belongs to class APawn
	PlayerInputComponent->BindAxis(TEXT("LookUp/DownRate"), this, &AMyCharacter::LookUpDownRate); //APawn because AddControllerYawInput() belongs to class APawn
	PlayerInputComponent->BindAxis(TEXT("LookLeft/RightRate"), this, &AMyCharacter::LookLeftRightRate); //APawn because AddControllerYawInput() belongs to class APawn
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AMyCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("ChangeWeapon"), EInputEvent::IE_Pressed, this, &AMyCharacter::ChangeWeapon);
}

void AMyCharacter::MoveForwardBackward(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AMyCharacter::MoveLeftRight(float Axisvalue) 
{
	AddMovementInput(GetActorRightVector() * Axisvalue);
}

void AMyCharacter::LookUpDownRate(float AxisValue) //For controller
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds()); //Using GetDeltaSeconds() to damp controller movement speed.
}

void AMyCharacter::LookLeftRightRate(float AxisValue) 
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds()); //Using GetDeltaSeconds() to damp controller movement speed.
}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	FTimerHandle TimerHandle;
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float DamageApplied = FMath::Min(Health, DamageToApply);
	Health -= DamageApplied;

	if (IsDead())
	{
		UE_LOG(LogTemp, Warning, TEXT("Killed"))
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        /*To Spawn loot after death*/
		GetWorldTimerManager().SetTimer(TimerHandle, [&](){ 
			SpawnLoot();
		}, 0.5, false);
	}
	return DamageToApply;
}

bool AMyCharacter::IsDead() const
{
	return Health <= 0;
}

float AMyCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

AGun* AMyCharacter::GetCurrentWeapon() const
{
	return Weapons[WeaponIndex];
}

int32 AMyCharacter::GetRemainingAmmo() const
{
	return TotalAmmo[WeaponIndex];
}

bool AMyCharacter::IsRifle() 
{
	if(WeaponIndex == 0)
	{
		return true; //Rifle it is!
	}
	return false; //Shotgun it is!
}

void AMyCharacter::Shoot() 
{
	FTimerHandle TimerHandle;
	if(TotalAmmo[WeaponIndex] == 0)
	{
		Shooting = false;
	}
	else
	{
		Shooting = true;
	}

	TotalAmmo[WeaponIndex] = Weapons[WeaponIndex]->PullTrigger(TotalAmmo[WeaponIndex]);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		Shooting = false;
	}, 0.5, false);
}

void AMyCharacter::ChangeWeapon()
{
	if(WeaponIndex == 0)
	{
		Weapons[WeaponIndex]->Destroy();
		WeaponIndex = 1;
	}
	else
	{
		Weapons[WeaponIndex]->Destroy();
		WeaponIndex = 0;
	}

	Weapons[WeaponIndex] = GetWorld()->SpawnActor<AGun>(WeaponClass[WeaponIndex]);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Weapons[WeaponIndex]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Weapons[WeaponIndex]->SetOwner(this);
}

AGun* AMyCharacter::GetGun()
{
	return Weapons[0];
}

void AMyCharacter::SpawnLoot() 
{
	FActorSpawnParameters SpawnParam;
	AActor* LootMeshRef = GetWorld()->SpawnActor<AActor>(LootMesh, this->GetActorLocation(), this->GetActorRotation(), SpawnParam);
}

void AMyCharacter::UpdateHealth(float HealthChange)
{
    Health = FMath::Clamp(Health += HealthChange, 0.0f, MaxHealth);
}