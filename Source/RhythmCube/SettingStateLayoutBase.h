// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingStateLayoutBase.generated.h"

class UWidgetAnimation;
class UButton;
class UTextBlock;
class USizeBox;
class USlider;
class AMainGameMode;
class UMainGameInstance;
class UCheckBox;
class UBorder;
class UDeleteConfirmMenuBase;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API USettingStateLayoutBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// Back button
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BackButton = nullptr;
	// Notes offset
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NotesOffsetAddButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NotesOffsetMinusButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> NotesOffsetText = nullptr;
	// Key binds
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> KeyBindsSwapButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> LeftColor = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> RightColor = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> KeyW = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> KeyA = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> KeyS = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> KeyD = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> KeyI = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> KeyJ = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> KeyK = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> KeyL = nullptr;
	// Volume
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> BGMVolumeText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> BGMVolumeSlider = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> HitsoundVolumeText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> HitsoundVolumeSlider = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SFXVolumeText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SFXVolumeSlider = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> UIVolumeText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> UIVolumeSlider = nullptr;
	// Graphics
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> GraphicsLowButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> GraphicsMediumButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> GraphicsHighButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> GraphicsUltraButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> GraphicsMaxButton = nullptr;
	// FPS
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FPSLowButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FPSMediumButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FPSHighButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FPSUltraButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FPSMaxButton = nullptr;
	// Camera shaking effect
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> CameraShakingEffectToggle = nullptr;
	// Transparent cube
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> TransparentCubeToggle = nullptr;
	// Hit Assist
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> HitAssistToggle = nullptr;
	// Account delete
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> AccountDeleteButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UDeleteConfirmMenuBase> DeleteConfirmMenu = nullptr;
	// Tutorial
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> TutorialButton = nullptr;
	// Copyright button
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CopyrightButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CopyrightCloseButton = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BackButtonHover = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DeleteConfirmFadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DeleteConfirmFadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CopyrightFadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CopyrightFadeOut = nullptr;

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	// Exposed references
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> NormalButtonTexture = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> SelectedButtonTexture = nullptr;
	UPROPERTY(EditAnywhere)
	FSlateSound ClickSound;
	UPROPERTY(EditAnywhere)
	FSlateSound HoverSound;

	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;

	// Button delegate functions
	// Back button
	UFUNCTION()
	void BackButtonHovered();
	UFUNCTION()
	void BackButtonUnHovered();
	UFUNCTION()
	void BackButtonClicked();
	// Notes offset
	UFUNCTION()
	void NotesOffsetAddButtonClicked();
	UFUNCTION()
	void NotesOffsetMinusButtonClicked();
	UFUNCTION()
	void NotesOffsetAddButtonPressed();
	UFUNCTION()
	void NotesOffsetMinusButtonPressed();
	// Key binds
	UFUNCTION()
	void KeyBindsSwapButtonClicked();
	// Volume
	UFUNCTION()
	void BGMVolumeSliderValueChanged(float Value);
	UFUNCTION()
	void SFXVolumeSliderValueChanged(float Value);
	UFUNCTION()
	void UIVolumeSliderValueChanged(float Value);
	UFUNCTION()
	void HitsoundVolumeSliderValueChanged(float Value);
	// Graphics
	UFUNCTION()
	void GraphicsLowButtonClicked();
	UFUNCTION()
	void GraphicsMediumButtonClicked();
	UFUNCTION()
	void GraphicsHighButtonClicked();
	UFUNCTION()
	void GraphicsUltraButtonClicked();
	UFUNCTION()
	void GraphicsMaxButtonClicked();
	// FPS
	UFUNCTION()
	void FPSLowButtonClicked();
	UFUNCTION()
	void FPSMediumButtonClicked();
	UFUNCTION()
	void FPSHighButtonClicked();
	UFUNCTION()
	void FPSUltraButtonClicked();
	UFUNCTION()
	void FPSMaxButtonClicked();
	// Camera shaking effect
	UFUNCTION()
	void CameraShakingEffectToggleChanged(bool bIsChecked);
	// Transparent cube
	UFUNCTION()
	void TransparentCubeToggleChanged(bool bIsChecked);
	// Hit Assist
	UFUNCTION()
	void HitAssistToggleChanged(bool bIsChecked);
	// Account delete
	UFUNCTION()
	void AccountDeleteButtonClicked();
	UFUNCTION()
	void CloseButtonClicked();
	// Tutorial button
	UFUNCTION()
	void TutorialButtonClicked();
	// Copyright button
	UFUNCTION()
	void CopyrightButtonClicked();
	UFUNCTION()
	void CopyrightCloseButtonClicked();

	// Functions
	void UpdateNotesOffsetText();
	void UpdateKeyBindsUI();
	void UpdateGraphicsButtonStyle();
	void UpdateFPSButtonStyle();

	// Variables
	FTimerHandle ButtonHoldTimer;
	FTimerHandle ChangeLoopTimer;
	FButtonStyle NormalButtonStyle;
	FButtonStyle SelectedButtonStyle;
	FSlateBrush NormalButtonBrush;
	FSlateBrush SelectedButtonBrush;
};
