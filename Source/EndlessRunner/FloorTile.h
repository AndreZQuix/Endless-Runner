// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "FloorTile.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;
class AObstacle;
class ACoinItem;
class AWall;

UCLASS()
class ENDLESSRUNNER_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		TSubclassOf<AObstacle> SmallObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		TSubclassOf<AObstacle> BigObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		TSubclassOf<ACoinItem> CoinItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		TSubclassOf<AWall> WallClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UArrowComponent* CenterLane;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UArrowComponent* RightLane;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UArrowComponent* LeftLane;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UArrowComponent* AttachPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UArrowComponent* WallLeftPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UArrowComponent* WallRightPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* FloorTriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		float SpawnPercent1 = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		float SpawnPercent2 = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		float SpawnPercent3 = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<AObstacle*> Obstacles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<ACoinItem*> Coins;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<AWall*> Walls;

	UFUNCTION(BlueprintCallable)
		void SpawnItems();

	UFUNCTION()
		void SpawnWalls();

	// Sets default values for this actor's properties
	AFloorTile();

	FORCEINLINE const FTransform& GetAttachTransform() const
	{
		return AttachPoint->GetComponentTransform();
	}

	FORCEINLINE const FTransform& GetLeftWallTransform() const
	{
		return WallLeftPoint->GetComponentTransform();
	}

	FORCEINLINE const FTransform& GetRightWallTransform() const
	{
		return WallRightPoint->GetComponentTransform();
	}

protected:
	UPROPERTY(VisibleInstanceOnly)
		class AEndlessRunnerGameModeBase* RunGameMode;

	UPROPERTY()
		FTimerHandle DestroyHandle;

	UFUNCTION()
		void OnTriggerBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SpawnLaneItem(UArrowComponent* Lane, int32& NumBigs);

	UFUNCTION()
		void DestroyFloorTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
