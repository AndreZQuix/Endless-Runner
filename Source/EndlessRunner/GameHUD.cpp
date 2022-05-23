// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "EndlessRunnerGameModeBase.h"

void UGameHUD::NativeConstruct()
{
	if (PauseButton)
	{
		PauseButton->OnClicked.AddDynamic(this, &UGameHUD::OnPauseClicked);	// delegate
	}

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(GetWorld(), 0), this);	// set input mode as ui and game
}

void UGameHUD::InitializeHUD(AEndlessRunnerGameModeBase* RunGameMode)
{
	if (RunGameMode)
	{
		CoinsCount->SetText(FText::AsNumber(0));	// null text variable
		RunGameMode->OnCoinsCountChanged.AddDynamic(this, &UGameHUD::SetCoinsCount);
	}
}

void UGameHUD::SetCoinsCount(const int32 Count)
{
	CoinsCount->SetText(FText::AsNumber(Count));	// set text of coins count widget
}

void UGameHUD::OnPauseClicked()
{
	if (IsValid(PauseMenuWidgetClass))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);	// pause game

		UUserWidget* Widget = CreateWidget(GetWorld(), PauseMenuWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();	// set pause menu
		}
	}
}
