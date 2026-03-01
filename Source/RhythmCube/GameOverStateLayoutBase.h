// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverStateLayoutBase.generated.h"

class AMainGameMode;
class UImage;
class UTextBlock;
class UButton;
class UBorder;
class UWidgetAnimation;
class UMainGameInstance;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UGameOverStateLayoutBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// References
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> GradeImage = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxComboText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyGainText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> RetryButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> HomeButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SongNameText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> HardcoreModeText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> DifficultyText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> Background = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> CompletedIcon = nullptr;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInLimited = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HighestScoreFadeIn = nullptr;

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	// Exposed references
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UTexture2D>> GradeImagesBase;
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UTexture2D>> BackgroundImagesBase;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> CompletedIconFullScore;

	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;

	// Delegate functions
	UFUNCTION()
	void Retry();
	UFUNCTION()
	void MainMenu();
};
