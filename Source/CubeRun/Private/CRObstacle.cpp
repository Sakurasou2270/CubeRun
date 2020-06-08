// Fill out your copyright notice in the Description page of Project Settings.

#include "CRObstacle.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACRObstacle::ACRObstacle()
{
	PrimaryActorTick.bCanEverTick = false;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = Cube;
}

// Called when the game starts or when spawned
void ACRObstacle::BeginPlay()
{
	Super::BeginPlay();
}
