// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AWall::AWall()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = SceneComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();	// base class method

	if (Meshes.Num() > 0)	// if meshes count is larger than 0
	{
		int RandomMeshNumber = FMath::RandRange(0, Meshes.Num() - 1);	// get random number from 0 to meshes count
		StaticMesh->SetStaticMesh(Meshes[RandomMeshNumber]);	// set static mesh	
	}
}

