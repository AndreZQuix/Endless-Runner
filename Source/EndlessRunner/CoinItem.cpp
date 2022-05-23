// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "Components/SphereComponent.h"
#include "RunCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
ACoinItem::ACoinItem()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));	// root component
	RootComponent = SceneComponent;

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Coin Mesh"));	// static mesh component
	CoinMesh->SetupAttachment(SceneComponent);
	CoinMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));	// collider and it's profile for overlaping only pawn
	SphereCollider->SetupAttachment(SceneComponent);
	SphereCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));	// rotating movement component
	RotatingMovement->RotationRate = FRotator(0, 180, 0);
}

// Called when the game starts or when spawned
void ACoinItem::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ACoinItem::OnSphereOverlap);
}

void ACoinItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter)
	{
		UWorld* World = GetWorld();
		if (OverlapSound && World)
		{
			UGameplayStatics::PlaySoundAtLocation(World, OverlapSound, GetActorLocation());
		}

		RunCharacter->AddCoin();	// add coin to character

		Destroy();	// delete actor from memory
	}
}