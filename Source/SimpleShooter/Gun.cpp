// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" //For Debug cameras to help in raytracing required for aiming

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	// RootComponent = Root;
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh Component"));
	Mesh->SetupAttachment(Root);
}

// Called every frame.
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

int32 AGun::PullTrigger(int32 RemainingAmmo) 
{
	if(RemainingAmmo == 0) //No ammo check.
	{
		UGameplayStatics::SpawnSoundAttached(MagEmptySound, Mesh, TEXT("RifleMagSocket"));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("No Ammo"));
		return RemainingAmmo;
	}
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnEmitterAttached(BulletTrail, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bCheckHit = GunTrace(Hit, ShotDirection);
	if(bCheckHit)
	{
		// DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		AActor* HitActor = Hit.GetActor();
		if(HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController *OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
	RemainingAmmo = (RemainingAmmo > 0)?(--RemainingAmmo):0;
	return RemainingAmmo;
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection) 
{
	FVector Location;
	FRotator Rotation;
	
	AController* OwnerController = GetOwnerController();
	if(OwnerController == nullptr)
	{
		return false;
	}
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) 
		return nullptr;
	return OwnerPawn->GetController();	
}

float AGun::GetMaxRange() 
{
	return MaxRange;
}