// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DifficultiesMenuBase.generated.h"

class AMainGameMode;
class UMainGameInstance;
class UButton;
class UCheckBox;
class UBorder;

/**
 *
 */
UCLASS()
class RHYTHMCUBE_API UDifficultiesMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NormalButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NormalBackground = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> HardButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> HardBackground = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ExtremeButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ExtremeBackground = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> HardcoreToggle = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> HardcoreBackground = nullptr;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;

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
	TObjectPtr<UTexture2D> NormalBackgroundImage = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> HardBackgroundImage = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ExtremeBackgroundImage = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> NormalBackgroundImageSelected = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> HardBackgroundImageSelected = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ExtremeBackgroundImageSelected = nullptr;

	// Delegate functions
	UFUNCTION()
	void ReadyPlayState(bool bNew);
	UFUNCTION()
	void MainMenu(bool bNew);
	UFUNCTION()
	void NormalButtonHovered();
	UFUNCTION()
	void NormalButtonUnHovered();
	UFUNCTION()
	void NormalButtonClicked();
	UFUNCTION()
	void HardButtonHovered();
	UFUNCTION()
	void HardButtonUnHovered();
	UFUNCTION()
	void HardButtonClicked();
	UFUNCTION()
	void ExtremeButtonHovered();
	UFUNCTION()
	void ExtremeButtonUnHovered();
	UFUNCTION()
	void ExtremeButtonClicked();
	UFUNCTION()
	void HardcoreToggleChanged(bool bIsChecked);
	UFUNCTION()
	void Hide();
	UFUNCTION()
	void LoadGame();

	// Functions
	void UpdateButton();

	// Variables
	FButtonStyle NormalButtonStyle;
	FButtonStyle NormalSelectedButtonStyle;
	FButtonStyle HardButtonStyle;
	FButtonStyle HardSelectedButtonStyle;
	FButtonStyle ExtremeButtonStyle;
	FButtonStyle ExtremeSelectedButtonStyle;
	FSlateBrush NormalButtonBrush;
	FSlateBrush NormalSelectedButtonBrush;
	FSlateBrush HardButtonBrush;
	FSlateBrush HardSelectedButtonBrush;
	FSlateBrush ExtremeButtonBrush;
	FSlateBrush ExtremeSelectedButtonBrush;
};
