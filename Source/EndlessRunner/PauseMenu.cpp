// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenu::NativeConstruct()
{
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UPauseMenu::OnContinueClicked);	// continue clicked delegate
	}

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UPauseMenu::OnRestartClicked);	// restart clicked delegate
	}
}

void UPauseMenu::OnContinueClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::SetGamePaused(World, false);	// unpause game
		RemoveFromParent();	// remove pause menu
	}
}

void UPauseMenu::OnRestartClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));	// restart level by console command
	}
}