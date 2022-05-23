// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOver.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOver::NativeConstruct()
{
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UGameOver::OnMainMenuClicked); // main menu clicked delegate
	}

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UGameOver::OnRestartClicked);	// restart clicked delegate
	}
}

void UGameOver::OnMainMenuClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, TEXT("MainMenu"));	// open main menu as level
	}
}

void UGameOver::OnRestartClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));	// restart level by console command
	}
}