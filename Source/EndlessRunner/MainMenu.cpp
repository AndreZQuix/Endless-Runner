// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	UGameplayStatics::PlaySound2D(GetWorld(), BGMSound, VolumeSound);	// play main menu background sound

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenu::OnStartClicked);	// start clicked delegate
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitClicked);	// quit clicked delegate
	}
}

void UMainMenu::OnStartClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, TEXT("MainLevel"));	// open game level
	}
}

void UMainMenu::OnQuitClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("quit"));	// quit game by console command
	}
}