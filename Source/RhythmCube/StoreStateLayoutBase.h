// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameInstance.h"
#include "StoreStateLayoutBase.generated.h"

class UButton;
class UTextBlock;
class AMainGameMode;
class UWidgetAnimation;
class UBuyConfirmMenuBase;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UStoreStateLayoutBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// Back button
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BackButton = nullptr;
	// Money Text
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyText = nullptr;
	// Buy buttons
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CyanNeonpinkBuyButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BlueRedBuyButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> GreenRedBuyButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BlueGoldBuyButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BlueWhiteBuyButton = nullptr;
	// Prices
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CyanNeonpinkDollarSign = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CyanNeonpinkMoneyText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> GreenRedDollarSign = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> GreenRedMoneyText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> BlueGoldDollarSign = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> BlueGoldMoneyText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> BlueWhiteDollarSign = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> BlueWhiteMoneyText = nullptr;
	// Buy confirm menu
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBuyConfirmMenuBase> BuyConfirmMenu = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BackButtonHover = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BackgroundAnimation = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BuyConfirmFadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BuyConfirmFadeOut = nullptr;

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;

	// Exposed references
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> UseButtonTexture = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> UsingButtonTexture = nullptr;
	UPROPERTY(EditAnywhere)
	FSlateSound ClickSound;
	UPROPERTY(EditAnywhere)
	FSlateSound HoverSound;

	// Button delegate functions
	// Back button
	UFUNCTION()
	void BackButtonHovered();
	UFUNCTION()
	void BackButtonUnHovered();
	UFUNCTION()
	void BackButtonClicked();
	// Buy buttons
	UFUNCTION()
	void CyanNeonpinkBuyButtonClicked();
	UFUNCTION()
	void BlueRedBuyButtonClicked();
	UFUNCTION()
	void GreenRedBuyButtonClicked();
	UFUNCTION()
	void BlueGoldBuyButtonClicked();
	UFUNCTION()
	void BlueWhiteBuyButtonClicked();
	// Close button
	UFUNCTION()
	void CloseButtonClicked();

	// Delegate functions
	UFUNCTION()
	void BuySuccess();

	// Functions
	void UpdateMoneyText();
	void UpdateBuyButtons(UButton* TargetButton, Theme TargetTheme);
	void UpdateAllBuyButtons();
	void UpdateOwnedState();
	void BuyButtonClickedEvent(Theme TargetTheme);

	// Variables
	FTimerHandle BackgroundAnimationTimer;
	FButtonStyle UseButtonStyle;
	FSlateBrush UseButtonBrush;
	FSlateBrush UsingButtonBrush;
};
