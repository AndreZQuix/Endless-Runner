// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API UGameOver : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* MainMenuButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* RestartButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnMainMenuClicked();

	UFUNCTION()
		void OnRestartClicked();
};
