// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CRPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ACRGameMode;
class UUserWidget;

UCLASS()
class CUBERUN_API ACRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACRPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	UStaticMeshComponent *Cube;

	//! Pointer is not getting all information just making a ref in memory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	UCameraComponent *Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floats")
	float ForwardForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floats")
	float SideForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UUserWidget> DefaultHUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	UUserWidget *HUD;


	ACRGameMode *GameMode;
	bool bGameEnded;
	float Mass;
	float DeltaSeconds;
	FTimerHandle EndGameTimer;

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent *HitComp,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		FVector NormalImpulse,
		const FHitResult &Hit);

	UFUNCTION()
	void OnComponentOverlap(
		class UPrimitiveComponent *OverlappedComp,
		class AActor *OtherActor,
		class UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	void GameEnded();
	void MoveRight(float Val);
	void EndGame();
};
