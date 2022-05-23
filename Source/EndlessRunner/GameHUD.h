// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

class UTextBlock;
class AEndlessRunnerGameModeBase;

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* CoinsCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* PauseButton;

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
		void InitializeHUD(AEndlessRunnerGameModeBase* RunGameMode);

	UFUNCTION(BlueprintCallable)
		void SetCoinsCount(int32 Count);

	UFUNCTION()
		void OnPauseClicked();
};
