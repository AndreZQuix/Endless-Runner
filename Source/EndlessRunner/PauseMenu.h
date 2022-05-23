// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* ContinueButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* RestartButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnContinueClicked();

	UFUNCTION()
		void OnRestartClicked();
};
