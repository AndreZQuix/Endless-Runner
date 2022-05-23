// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessRunnerGameModeBase.h"
#include "FloorTile.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.h"
#include "Obstacle.h"

void AEndlessRunnerGameModeBase::BeginPlay()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	GameHUD = Cast<UGameHUD>(CreateWidget(GetWorld(), GameHUDClass));
	check(GameHUD);

	GameHUD->InitializeHUD(this);
	GameHUD->AddToViewport();

	CreateInitialFloorTiles();
}

void AEndlessRunnerGameModeBase::CreateInitialFloorTiles()
{
	AFloorTile* InitialTile = AddFloorTile(false);	// spawn initial tile without items (coins or obstacles)

	if (InitialTile)	// add lines Y location
	{
		LaneSwitchValues.Add(InitialTile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(InitialTile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(InitialTile->RightLane->GetComponentLocation().Y);
	}

	//AddFloorTile(false);	// uncomment for small tiles
	//AddFloorTile(false);

	for (int i = 0; i < NumInitialFloorTiles; i++)	// spawn initial tiles with items
		AddFloorTile(true);
}

AFloorTile* AEndlessRunnerGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	UWorld* World = GetWorld();

	if (World)
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);	// spawn tile at next spawn point

		if (Tile)	// if spawn successed...
		{
			if (bSpawnItems)	// ...and there is a need to spawn items...
			{
				Tile->SpawnItems();		// ...spawn obstacles, walls (environment models) and coins
				Tile->SpawnWalls();
			}

			NextSpawnPoint = Tile->GetAttachTransform();	// get next spawn point

			//------- Delete this for small (500.0f on X coordinate) tiles
			FVector MeshSize = Tile->FloorMesh->GetStaticMesh()->GetBoundingBox().GetSize();	// calculate length of tile
			float LocationX = NextSpawnPoint.GetLocation().X - 500.0f + (MeshSize * Tile->FloorMesh->GetRelativeScale3D()).X;	//500.0f BP_FloorTile mesh size
			NextSpawnPoint.SetLocation(FVector(LocationX, NextSpawnPoint.GetLocation().Y, NextSpawnPoint.GetLocation().Z));	// set next spawn point
			//-------
		}

		return Tile;
	}

	return nullptr;
}

void AEndlessRunnerGameModeBase::AddCoin()
{
	OnCoinsCountChanged.Broadcast(++TotalCoinNumber);	// total coin number changed: broadcast this information
}

void AEndlessRunnerGameModeBase::GameOver()
{
	if (IsValid(GameOverScreenClass))	// if game over menu class exists..
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), GameOverScreenClass);
		if (Widget)
		{
			Widget->AddToViewport();	// ..add it to the viewport 
		}
	}
}
