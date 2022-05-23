// Fill out your copyright notice in the Description page of Project Settings.

#include "RunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EndlessRunnerGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameHUD.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));	// camera arm component
	CameraArm->TargetArmLength = 350.0f;
	CameraArm->SocketOffset = FVector(0.0f, 0.0f, 100.0f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));	// camera component
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	check(RunGameMode);

	UGameplayStatics::PlaySound2D(GetWorld(), BGMSound, VolumeSound);
}

void ARunCharacter::AddCoin()
{
	RunGameMode->AddCoin();
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector());

	CheckSwiping();
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunCharacter::StopJumping);

	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARunCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARunCharacter::MoveRight);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ARunCharacter::MoveDown);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARunCharacter::BeginTouch);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARunCharacter::EndTouch);
}

void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);	// linear interpolation
	SetActorLocation(Location);
}

void ARunCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ARunCharacter::AccelerateSpeed()
{
	MovementComponent = GetCharacterMovement();
	if (MovementComponent && MovementComponent->MaxWalkSpeed < MaxSpeedWithAcceleration)
	{
		MovementComponent->MaxWalkSpeed += Acceleration;
	}
}

void ARunCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane -1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane +1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveDown()
{
	static FVector Impulse = FVector(0, 0, MoveDownImpulse);
	GetCharacterMovement()->AddImpulse(Impulse, true);
}

void ARunCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsPressed = true;
	FVector2D TempLocation;
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		bool bIsCurrentlyPressed;
		PC->GetInputTouchState(ETouchIndex::Touch1, TempLocation.X, TempLocation.Y, bIsCurrentlyPressed);
	}
	TouchStart = TempLocation;
}

void ARunCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsPressed = false;
	DoOnce = true;
}

void ARunCharacter::Death()
{
	if (!bIsDead)
	{
		const FVector Location = GetActorLocation();

		UWorld* World = GetWorld();

		if (DeathParticleSystem && World && DeathSound)
		{
			bIsDead = true;	// character is dead
			DisableInput(nullptr);	// no input

			UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);	// spawn particles
			UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);	// play death sound
			GetMesh()->SetVisibility(false);	// hide character from scene
			GetCharacterMovement()->Deactivate();	// stop movement
			World->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 1.0f);	// set timer for game over function
		}
	}
}

void ARunCharacter::OnDeath()
{
	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}

	RunGameMode->GameOver();
}

void ARunCharacter::CheckSwiping()
{
	if (bIsPressed)
	{
		FVector2D TempLocation;
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			bool bIsCurrentlyPressed;
			PC->GetInputTouchState(ETouchIndex::Touch1, TempLocation.X, TempLocation.Y, bIsCurrentlyPressed);
		}

		float TouchLength = (TouchStart - TempLocation).Size();	// calculate touch length
		if (TouchLength > TouchDistance)	// if touch length is not just a press...
		{
			if (DoOnce)	// ... do this one
			{
				DoOnce = false;
				TouchEnd = TempLocation;
				FVector2D Swipe = TouchStart - TouchEnd;	// get swipe length
				if (UKismetMathLibrary::Abs(Swipe.X) > UKismetMathLibrary::Abs(Swipe.Y))	// calculate movement
				{
					if (Swipe.X > 0.0f)
					{
						MoveLeft();
					}
					else
					{
						MoveRight();
					}
				}
				else
				{
					if (Swipe.Y > 0.0f)
					{
						Jump();
					}
					else
					{
						MoveDown();
					}
				}
			}
		}
	}
}