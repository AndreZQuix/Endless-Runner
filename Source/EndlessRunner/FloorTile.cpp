// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"
#include "Components/BoxComponent.h"
#include "EndlessRunnerGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Obstacle.h"
#include "CoinItem.h"
#include "Wall.h"
#include "RunCharacter.h"

// Sets default values
AFloorTile::AFloorTile()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Mesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(SceneComponent);

	WallRightPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Wall Right Point"));
	WallRightPoint->SetupAttachment(SceneComponent);

	WallLeftPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Wall Left Point"));
	WallLeftPoint->SetupAttachment(SceneComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Center Lane"));
	CenterLane->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Lane"));
	RightLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Lane"));
	LeftLane->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Trigger Box"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.0f, 500.0f, 200.0f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void AFloorTile::OnTriggerBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	
	if (RunCharacter)
	{
		RunGameMode->AddFloorTile(true);

		GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTile, 2.0f, false);

		RunCharacter->AccelerateSpeed();
	}
}

void AFloorTile::DestroyFloorTile()
{
	if (DestroyHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

	for (AObstacle* Obstacle : Obstacles)
	{
		if (Obstacle)
		{
			Obstacle->Destroy();
		}
	}

	for (ACoinItem* Coin : Coins)
	{
		if (Coin)
		{
			Coin->Destroy();
		}
	}

	for (AWall* Wall : Walls)
	{
		if (Wall)
		{
			Wall->Destroy();
		}
	}

	this->Destroy();
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();
	
	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	check(RunGameMode);

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnTriggerBoxOverlapped);
}

void AFloorTile::SpawnItems()
{
	if (IsValid(SmallObstacleClass) && IsValid(BigObstacleClass) && IsValid(CoinItemClass) && IsValid(WallClass))	// check if class is valid
	{
		int32 NumBigs = 0;	// big obstacles count
		SpawnLaneItem(LeftLane, NumBigs);
		SpawnLaneItem(CenterLane, NumBigs);
		SpawnLaneItem(RightLane, NumBigs);
		//SpawnWalls();
	}
}

void AFloorTile::SpawnLaneItem(UArrowComponent* Lane, int32& NumBigs)
{
	const float RandVal = FMath::FRandRange(0.0f, 1.0f);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform& SpawnLocation = Lane->GetComponentTransform();

	UWorld* World = GetWorld();
	if (World)
	{
		AObstacle* Obstacle = nullptr;

		if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent1, SpawnPercent2, true, true))
		{
			Obstacle = World->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParameters);
		}
		else if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent2, SpawnPercent3, false, false))
		{
			if (NumBigs < 2)
			{
				Obstacle = World->SpawnActor<AObstacle>(BigObstacleClass, SpawnLocation, SpawnParameters);

				if (Obstacle)
				{
					NumBigs += 1;
				}
			}
			else
			{
				Obstacle = World->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParameters);
			}
		}
		else if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent3, 1.0f, true, true))
		{
			ACoinItem* CoinItem = World->SpawnActor<ACoinItem>(CoinItemClass, SpawnLocation, SpawnParameters);
			Coins.Add(CoinItem);
		}

		Obstacles.Add(Obstacle);
	}
}

void AFloorTile::SpawnWalls()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	UWorld* World = GetWorld();	// get wall point tile location and spawn the wall
	if (World)
	{
		const FTransform& RightWallSpawnLocation = GetRightWallTransform();
		AWall* Wall = World->SpawnActor<AWall>(WallClass, RightWallSpawnLocation, SpawnParameters);
		Walls.Add(Wall);

		const FTransform& LeftWallSpawnLocation = GetLeftWallTransform();
		Wall = World->SpawnActor<AWall>(WallClass, LeftWallSpawnLocation, SpawnParameters);
		Walls.Add(Wall);
	}
}