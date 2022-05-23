// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* StartButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* QuitButton;

	UPROPERTY(EditAnywhere, Category = "Config")
		class USoundBase* BGMSound;

	UPROPERTY(EditAnywhere, Category = "Config")
		float VolumeSound = 0.1f;

	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnStartClicked();

	UFUNCTION()
		void OnQuitClicked();
};
