// Fill out your copyright notice in the Description page of Project Settings.

#include "CRPawn.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CRGameMode.h"
#include "CRObstacle.h"
#include "CREndPoint.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ACRPawn::ACRPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->SetSimulatePhysics(true);
	RootComponent = Cube;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Cube);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	ForwardForce = 2000.f;
	SideForce = 25.f;
	// AutoPossessPlayer = EAutoReceiveInput::Player0;
	bGameEnded = false;
	Mass = 100.f;
}

// Called when the game starts or when spawned
void ACRPawn::BeginPlay()
{
	Super::BeginPlay();

	Cube->OnComponentHit.AddDynamic(this, &ACRPawn::OnHit);
	Cube->OnComponentBeginOverlap.AddDynamic(this, &ACRPawn::OnComponentOverlap);
	GameMode = Cast<ACRGameMode>(GetWorld()->GetAuthGameMode());

	if (DefaultHUD)
	{
		HUD = CreateWidget<UUserWidget>(GetWorld(), DefaultHUD);
		HUD->AddToViewport();
	}
}

// Called every frame
void ACRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaSeconds = DeltaTime;
	FVector Impulse = FVector(Mass * ForwardForce * DeltaTime, 0.f, 0.f);
	Cube->AddImpulse(Impulse);

	FVector PlayerLocation = GetActorLocation();
	if (PlayerLocation.Z < -100.f)
	{
		GameEnded();
	}
}

// Called to bind functionality to input
void ACRPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ACRPawn::MoveRight);
}

void ACRPawn::OnHit(
	UPrimitiveComponent *HitComp,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	FVector NormalImpulse,
	const FHitResult &Hit)
{
	ACRObstacle *Obstacle = Cast<ACRObstacle>(OtherActor);

	if (Obstacle)
	{
		GameEnded();
	}
}

void ACRPawn::OnComponentOverlap(
	class UPrimitiveComponent *OverlappedComp,
	class AActor *OtherActor,
	class UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	//! The actors thats passed through
	ACREndPoint *LevelEnd = Cast<ACREndPoint>(OtherActor);
	
	if (LevelEnd)
	{
		GameMode->LevelComplete();
	}
}

void ACRPawn::GameEnded()
{
	if (bGameEnded)
	{
		return;
	}

	bGameEnded = true;

	GetWorldTimerManager().SetTimer(EndGameTimer, this, &ACRPawn::EndGame, 2.f, false);
}

void ACRPawn::MoveRight(float Val)
{
	if (!bGameEnded)
	{
		// FVector Impulse = FVector(0.f, ForwardForce * DeltaSeconds * Val, 0.f);
		Cube->AddImpulse(FVector(0.f, Mass * SideForce * GetWorld()->GetDeltaSeconds() * Val, 0.f), NAME_Name, true);
	}
}

void ACRPawn::EndGame()
{
	GameMode->EndGame();
}