// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

// Sets default values
AHealthBox::AHealthBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	CollisionTrigger->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	SetRootComponent(CollisionTrigger);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
	StaticMesh->SetupAttachment(CollisionTrigger);
}

// Called when the game starts or when spawned
void AHealthBox::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AHealthBox::OnOverlapBegin);
	StaticMesh->OnComponentEndOverlap.AddDynamic(this, &AHealthBox::OnOverlapEnd);

	CollisionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHealthBox::OnOverlapBegin);
	CollisionTrigger->OnComponentEndOverlap.AddDynamic(this, &AHealthBox::OnOverlapEnd);
}

// Called every frame
void AHealthBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealthBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthBox Overlapped"))
		AddHealth();
	}
}

void AHealthBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{

}

void AHealthBox::AddHealth() 
{
	PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(PlayerCharacter == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("NULL PlayerCharacter in HealthBox.cpp"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Health = %f"), PlayerCharacter->Health)
	if(PlayerCharacter->Health <= 250)
	{
		PlayerCharacter->Health += HealingFactor;
		HealingFactor = 0;
		UE_LOG(LogTemp, Warning, TEXT("Health Added"))
	}

	Destroy();
}
