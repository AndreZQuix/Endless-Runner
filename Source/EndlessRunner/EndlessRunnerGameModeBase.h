// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameModeBase.generated.h"

class AFloorTile;
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsCountChanged, int32, CoinsCount); // delegate declaration

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Config")
		TSubclassOf<AFloorTile> FloorTileClass;	// tile class

	UPROPERTY(EditAnywhere, Category = "Config")
		TSubclassOf<UUserWidget> GameHUDClass;	// hud class

	UPROPERTY(EditAnywhere, Category = "Config")
		TSubclassOf<UUserWidget> GameOverScreenClass;	// game over menu class

	UPROPERTY(VisibleAnywhere)
		int32 TotalCoinNumber = 0;

	UPROPERTY(EditAnywhere, Category = "Config")
		int32 NumInitialFloorTiles = 10;	// initial tiles count

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		TArray<float> LaneSwitchValues;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
		FOnCoinsCountChanged OnCoinsCountChanged;	// delegate

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UGameHUD* GameHUD;

	UFUNCTION(BlueprintCallable)
		void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable)
		AFloorTile* AddFloorTile(const bool bSpawnItems);

	UFUNCTION(BlueprintCallable)
		void AddCoin();		// spawn coin

	UFUNCTION(BlueprintCallable)
		void GameOver();
	
protected:
	virtual void BeginPlay() override;
};
