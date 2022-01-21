// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "Gun.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	CollisionBox->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	SetRootComponent(CollisionBox);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
	StaticMesh->SetupAttachment(CollisionBox);

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnBeginOverlap);
	StaticMesh->OnComponentEndOverlap.AddDynamic(this, &AAmmoBox::OnEndOverlap);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AAmmoBox::OnEndOverlap);
}

// Called when the game starts or when spawned
void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAmmoBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmoBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor && (OtherActor != this) && OtherActor->IsA<AMyCharacter>())
	{
		UE_LOG(LogTemp, Warning, TEXT("AmmoBox Overlapped"))
		AddAmmo();
	}
}

void AAmmoBox::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	// return;
}

void AAmmoBox::AddAmmo() 
{
	PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(PlayerCharacter == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Null PlayerCharacter in AmmoBox.cpp"));
		return;
	}

	while(PlayerCharacter->TotalAmmo[WeaponIndex] != PlayerCharacter->Weapons[WeaponIndex]->EmpAmmo && AmmoQuantity > 0)
	{
		PlayerCharacter->TotalAmmo[WeaponIndex] += 1; //Adding 1 unit until the mag is full or the AmmoQuantity is empty.
		AmmoQuantity -= 1;
		// UE_LOG(LogTemp, Warning, TEXT("AmmoInBox = %d"), AmmoQuantity);
	}

	if(AmmoQuantity <= 0) //if no Ammo in AmmoBox, return.
	{
		Destroy();
		return;
	}
}