// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunCharacter.generated.h"

UCLASS()
class ENDLESSRUNNER_API ARunCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
		class AEndlessRunnerGameModeBase* RunGameMode;

	UPROPERTY()
		class UCharacterMovementComponent* MovementComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Assets")
		class UParticleSystem* DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		float Acceleration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		float MaxSpeedWithAcceleration = 2200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Assets")
		class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Config")
		class USoundBase* BGMSound;

	UPROPERTY(EditAnywhere, Category = "Config")
		float VolumeSound = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveDownImpulse = -1000.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
		int32 CurrentLane = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
		int32 NextLane = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TouchDistance = 0.1f;

	UPROPERTY(VisibleInstanceOnly)
		FVector2D TouchStart;

	UPROPERTY(VisibleInstanceOnly)
		FVector2D TouchEnd;

	UFUNCTION(BlueprintImplementableEvent, Category = "Lane")
		void ChangeLane();

	UFUNCTION(BlueprintCallable, Category = "Lane")
		void ChangeLaneUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Lane")
		void ChangeLaneFinished();

	UFUNCTION(BlueprintCallable)
		void Death();

	UFUNCTION()
		void AccelerateSpeed();

	// Sets default values for this character's properties
	ARunCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void MoveLeft();

	UFUNCTION(BlueprintCallable)
		void MoveRight();

	UFUNCTION(BlueprintCallable)
		void MoveDown();

	UFUNCTION(BlueprintCallable)
		void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);

	UFUNCTION(BlueprintCallable)
		void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);

	UPROPERTY()
		FTimerHandle RestartTimerHandle;

	UPROPERTY(VisibleInstanceOnly)
		bool bIsDead = false;

	UPROPERTY(VisibleInstanceOnly)
		bool bIsPressed = false;

	UPROPERTY()
		bool DoOnce = true;

	UFUNCTION(BlueprintCallable)
		void OnDeath();

	UFUNCTION()
		void CheckSwiping();

public:	
	UFUNCTION()
		void AddCoin();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
