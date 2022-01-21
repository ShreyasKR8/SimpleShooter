// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

AShooterPlayerController::AShooterPlayerController()
{
	bPaused = false;
}

void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if(HUD != nullptr)
    {
        HUD->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    FTimerHandle WLTimerHandle;
    HUD->RemoveFromViewport();
    bShowMouseCursor = false;
    if(bIsWinner)
    {
        GetWorld()->GetTimerManager().SetTimer(WLTimerHandle, [&]()
        {
            UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
            if(WinScreen != nullptr)
            {
                bShowMouseCursor = true;
                WinScreen->AddToViewport();
                UGameplayStatics::SetGamePaused(GetWorld(), true);
                UWidgetBlueprintLibrary::SetFocusToGameViewport();
                UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, WinScreen); //Without which, you have to double click to click on the buttons.
            }
        }, 0.8, false);

        // GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
    }
    else
    {
        GetWorld()->GetTimerManager().SetTimer(WLTimerHandle, [&]()
        {
            UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);

            if(LoseScreen != nullptr)
            {
                bShowMouseCursor = true;
                LoseScreen->AddToViewport();
                UGameplayStatics::SetGamePaused(GetWorld(), true);
                UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, LoseScreen); //Without which, you have to double click to click on the buttons.
    
            }
        }, 0.8, false);
    }
}

void AShooterPlayerController::SetupInputComponent() 
{
    Super::SetupInputComponent();

    FInputActionBinding& PauseToggle = InputComponent->BindAction(TEXT("Pause"), EInputEvent::IE_Pressed, this, &AShooterPlayerController::PauseMenuScreen);
    PauseToggle.bExecuteWhenPaused = true;
}

void AShooterPlayerController::PauseMenuScreen() 
{
    // if(bPaused)
    // {
    //     bPaused = false;
    //     UE_LOG(LogTemp, Warning, TEXT("Removed Widget"))
    //     bShowMouseCursor = false;
    //     UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
    //     PauseMenu->RemoveFromViewport();
    //     UWidgetBlueprintLibrary::SetFocusToGameViewport();
    // }
    // else
    // {
    //     // SetTickableWhenPaused(true);
    //     bPaused = true;

    //     if(!PauseMenu)
    //     {
    //         PauseMenu = CreateWidget(this, PauseScreenClass);
    //     }
    //     PauseMenu->AddToViewport();
    //     UGameplayStatics::SetGamePaused(GetWorld(), true);
    //     UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, PauseMenu);
    //     UE_LOG(LogTemp, Warning, TEXT("Added Widget"))
    //     bShowMouseCursor = true;
    // }
    if(!bPaused)
    {
        if(!PauseMenu)
        {
            PauseMenu = CreateWidget(this, PauseScreenClass);
        }
        PauseMenu->AddToViewport();
        // SetTickableWhenPaused(true);

        UGameplayStatics::SetGamePaused(GetWorld(), true);
        bPaused = true;
        bShowMouseCursor = true;

        UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, PauseMenu);
        UE_LOG(LogTemp, Warning, TEXT("Added Widget"))
    }
    else
    {
        bPaused = false;
        UE_LOG(LogTemp, Warning, TEXT("Removed Widget"))
        bShowMouseCursor = false;
        PauseMenu->RemoveFromViewport();
        UGameplayStatics::SetGamePaused(GetWorld(), false);
        UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
        // UWidgetBlueprintLibrary::SetFocusToGameViewport();
    }
}
