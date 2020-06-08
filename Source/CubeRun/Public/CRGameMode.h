// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CRGameMode.generated.h"

class UUserWidget;

UCLASS()
class CUBERUN_API ACRGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACRGameMode();

	void EndGame();
	void LevelComplete();
	void LoadNextLevel();

	FString CleanLevelString(UObject *WorldContextObject);

protected:
	UPROPERTY(EditAnywhere, Category = "UMG")
	TSubclassOf<UUserWidget> DefaultLevelCompleteWidget;

	UPROPERTY(EditAnywhere, Category = "UMG")
	TSubclassOf<UUserWidget> DefaultGameCompleteWidget;

	UUserWidget *LevelCompleteWidget;
	UUserWidget *GameCompleteWidget;
	FTimerHandle LevelSwapTimer;

private:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels", meta = (AllowPrivateAccess = "true"))
	TArray<FString> Levels;

	APlayerController *Controller;
	int32 CurrentLevelIndex;
	FString NextLevel;
	FString GetCurrentLevelName;

	void CheckLevel();
};
