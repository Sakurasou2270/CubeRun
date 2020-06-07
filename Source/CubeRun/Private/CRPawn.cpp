// Fill out your copyright notice in the Description page of Project Settings.

#include "CRPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CRGameMode.h"

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

	ForwardForce = 2000;
	SideForce = 5;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bGameEnded = false;
	Mass = Cube->GetMass();
}

// Called when the game starts or when spawned
void ACRPawn::BeginPlay()
{
	Super::BeginPlay();

	Cube->OnComponentHit.AddDynamic(this, &ACRPawn::OnHit);
	Cube->OnComponentBeginOverlap.AddDynamic(this, &ACRPawn::OnComponentOverlap);
	GameMode = Cast<ACRGameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ACRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGameEnded)
	{
		FVector Impulse = FVector(Mass * ForwardForce * DeltaSeconds, 0.f, 0.f);
		Cube->AddImpulse(Impulse);

		FVector PlayerLocation = GetActorLocation();
		if (PlayerLocation.Z < -100.f)
		{
			GameEnded();
		}
	}

	DeltaSeconds = DeltaTime;
}

// Called to bind functionality to input
void ACRPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ACRPawn::MoveRight);
}

void ACRPawn::OnHit(
	class UPrimitiveComponent *MyComp,
	AActor *Other,
	class UPrimitiveComponent *OtherComp,
	FVector NormalImpulse,
	const FHitResult &Hit)
{
}

void ACRPawn::OnComponentOverlap(
	class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
}

void ACRPawn::GameEnded()
{
}

void ACRPawn::MoveRight(float Val)
{
	if (!bGameEnded)
	{
		FVector Impulse = FVector(0.f, Mass * ForwardForce * DeltaSeconds * Val, 0.f);
		Cube->AddImpulse(Impulse, NAME_Name, true);
	}
}

void ACRPawn::EndGame()
{
}