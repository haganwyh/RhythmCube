// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayStateLayoutBase.generated.h"

class UTextBlock;
class UImage;
class AMainGameMode;
class UProgressBar;
class UMainGameInstance;
class AMainPlayerController;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UPlayStateLayoutBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// References
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ComboText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ComboImage = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SongNameText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> DifficultyText = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ComboInOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SongCompletedInOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ReadyGoInOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HealthFadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HealthHurt = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HealthWarnInOut = nullptr;

	// Functions
	void UpdateUI(float Score, int Combo);
	void ShowCompletedBanner();

	// Variables
	bool bFirstCreated;

protected:
	// Override functions
	virtual void NativeOnInitialized() override;

private:
	// Exposed references
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UTexture2D>> ComboImagesBase;

	// Delegate functions
	UFUNCTION()
	void ReadyPlayState(bool bNew);
	UFUNCTION()
	void MainMenu(bool bNew);
	UFUNCTION()
	void HealthChanged();
	UFUNCTION()
	void UnPauseState();

	// Variables
	FTimerHandle HealthWarnTimer;
	FVector2D TwoDigitSize = FVector2D(640.869565f, 75.833328f); // X = original / 2.3
	FVector2D ThreeDigitSize = FVector2D(707.826111f, 75.833328f);
	FVector2D FourDigitSize = FVector2D(774.347826f, 75.833328f);
	bool bHealthWarn;

	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;
	UPROPERTY()
	TObjectPtr<AMainPlayerController> MainPlayerController = nullptr;

	// Functions
	void ShowComboBanner(int ID, FVector2D Size);
	void HealthWarn();
};
