// Fill out your copyright notice in the Description page of Project Settings.

#include "CRGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ACRGameMode::ACRGameMode()
{
    CurrentLevelIndex = 0;
}

void ACRGameMode::BeginPlay()
{
    Super::BeginPlay();

    Controller = GetWorld()->GetFirstPlayerController();

    //! Setting input mode
    FInputModeGameOnly InputMode;
    Controller->SetInputMode(InputMode);
    Controller->bShowMouseCursor = false;

    GetCurrentLevelName = CleanLevelString(GetWorld());
    CheckLevel();
    GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, TEXT("") + GetCurrentLevelName);
}

void ACRGameMode::CheckLevel()
{
    Levels.Find(GetCurrentLevelName, CurrentLevelIndex);

    if (CurrentLevelIndex < Levels.Num() - 1)
    {
        NextLevel = Levels[CurrentLevelIndex + 1];
    }
    else
    {
        NextLevel = "End";
    }
}

void ACRGameMode::EndGame()
{
    // FString LevelString = GetWorld()->GetMapName();
    FName LevelToLoad = FName(*GetCurrentLevelName);

    UGameplayStatics::OpenLevel(this, LevelToLoad, true);
}

void ACRGameMode::LevelComplete()
{
    if (DefaultLevelCompleteWidget)
    {
        LevelCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultLevelCompleteWidget);
        LevelCompleteWidget->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Default UserDefaultWidget Set"));
    }

    GetWorldTimerManager().SetTimer(LevelSwapTimer, this, &ACRGameMode::LoadNextLevel, 2.f, false);
}

void ACRGameMode::LoadNextLevel()
{
    if (Levels.Contains(NextLevel))
    {
        FName LevelToLoad = FName(*NextLevel);
        UGameplayStatics::OpenLevel(this, LevelToLoad, true);
    }
    else
    {
        if (LevelCompleteWidget)
        {
            LevelCompleteWidget->RemoveFromParent();

            if (DefaultGameCompleteWidget)
            {
                GameCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultGameCompleteWidget);
                GameCompleteWidget->AddToViewport();

                Controller->bShowMouseCursor = true;
                FInputModeUIOnly InputMode;
                Controller->SetInputMode(InputMode);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("GameCompleteWidget Does Not Exist"));
            }
        }
    }
}

FString ACRGameMode::CleanLevelString(UObject *WorldContextObject)
{
    if (GEngine)
    {
        // Removing level prefix
        FString Prefix = GEngine->GetWorldFromContextObject(WorldContextObject)->StreamingLevelsPrefix;
        FString LevelName = GetWorld()->GetMapName();

        return LevelName.RightChop(Prefix.Len());
    }
    return "No Map Exists";
}