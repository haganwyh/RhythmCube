// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingStateLayoutBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridSlot.h"
#include "Components/CheckBox.h"
#include "Components/Border.h"
#include "Kismet/KismetMathLibrary.h"
#include "DeleteConfirmMenuBase.h"

void USettingStateLayoutBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind button delegates
	BackButton->OnHovered.AddDynamic(this, &USettingStateLayoutBase::BackButtonHovered);
	BackButton->OnUnhovered.AddDynamic(this, &USettingStateLayoutBase::BackButtonUnHovered);
	BackButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::BackButtonClicked);

	NotesOffsetAddButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::NotesOffsetAddButtonClicked);
	NotesOffsetAddButton->OnPressed.AddDynamic(this, &USettingStateLayoutBase::NotesOffsetAddButtonPressed);
	NotesOffsetMinusButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::NotesOffsetMinusButtonClicked);
	NotesOffsetMinusButton->OnPressed.AddDynamic(this, &USettingStateLayoutBase::NotesOffsetMinusButtonPressed);

	KeyBindsSwapButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::KeyBindsSwapButtonClicked);

	BGMVolumeSlider->OnValueChanged.AddDynamic(this, &USettingStateLayoutBase::BGMVolumeSliderValueChanged);
	HitsoundVolumeSlider->OnValueChanged.AddDynamic(this, &USettingStateLayoutBase::HitsoundVolumeSliderValueChanged);
	SFXVolumeSlider->OnValueChanged.AddDynamic(this, &USettingStateLayoutBase::SFXVolumeSliderValueChanged);
	UIVolumeSlider->OnValueChanged.AddDynamic(this, &USettingStateLayoutBase::UIVolumeSliderValueChanged);

	GraphicsLowButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::GraphicsLowButtonClicked);
	GraphicsMediumButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::GraphicsMediumButtonClicked);
	GraphicsHighButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::GraphicsHighButtonClicked);
	GraphicsUltraButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::GraphicsUltraButtonClicked);
	GraphicsMaxButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::GraphicsMaxButtonClicked);

	FPSLowButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::FPSLowButtonClicked);
	FPSMediumButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::FPSMediumButtonClicked);
	FPSHighButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::FPSHighButtonClicked);
	FPSUltraButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::FPSUltraButtonClicked);
	FPSMaxButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::FPSMaxButtonClicked);

	CameraShakingEffectToggle->OnCheckStateChanged.AddDynamic(this, &USettingStateLayoutBase::CameraShakingEffectToggleChanged);
	TransparentCubeToggle->OnCheckStateChanged.AddDynamic(this, &USettingStateLayoutBase::TransparentCubeToggleChanged);
	HitAssistToggle->OnCheckStateChanged.AddDynamic(this, &USettingStateLayoutBase::HitAssistToggleChanged);

	AccountDeleteButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::AccountDeleteButtonClicked);
	DeleteConfirmMenu->CloseButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::CloseButtonClicked);

	TutorialButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::TutorialButtonClicked);

	CopyrightButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::CopyrightButtonClicked);
	CopyrightCloseButton->OnClicked.AddDynamic(this, &USettingStateLayoutBase::CopyrightCloseButtonClicked);

	// Set brushes
	NormalButtonBrush.SetResourceObject(NormalButtonTexture);
	NormalButtonBrush.SetImageSize(FVector2D(214.326004f, 59.975998f));
	NormalButtonStyle.Normal = NormalButtonBrush;
	NormalButtonStyle.Normal.TintColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	NormalButtonStyle.Hovered = NormalButtonBrush;
	NormalButtonStyle.Hovered.TintColor = FLinearColor(1.f, 1.f, 1.f, 0.75f);
	NormalButtonStyle.Pressed = NormalButtonBrush;
	NormalButtonStyle.Pressed.TintColor = FLinearColor(1.f, 1.f, 1.f, 0.5f);
	NormalButtonStyle.SetPressedSound(ClickSound);
	NormalButtonStyle.SetHoveredSound(HoverSound);
	SelectedButtonBrush.SetResourceObject(SelectedButtonTexture);
	SelectedButtonBrush.SetImageSize(FVector2D(214.326004f, 59.975998f));
	SelectedButtonStyle.Normal = SelectedButtonBrush;
	SelectedButtonStyle.Normal.TintColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	SelectedButtonStyle.Hovered = SelectedButtonBrush;
	SelectedButtonStyle.Hovered.TintColor = FLinearColor(1.f, 1.f, 1.f, 0.75f);
	SelectedButtonStyle.Pressed = SelectedButtonBrush;
	SelectedButtonStyle.Pressed.TintColor = FLinearColor(1.f, 1.f, 1.f, 0.5f);
	SelectedButtonStyle.SetPressedSound(ClickSound);
	SelectedButtonStyle.SetHoveredSound(HoverSound);

	// Set left & right color
	FLinearColor UsingThemeColorLeft = MainGameMode->ThemeColors[static_cast<int32>(MainGameInstance->UsingTheme)][0];
	FLinearColor UsingThemeColorRight = MainGameMode->ThemeColors[static_cast<int32>(MainGameInstance->UsingTheme)][1];
	float LeftMaxValue = FMath::Max3(UsingThemeColorLeft.R, UsingThemeColorLeft.G, UsingThemeColorLeft.B);
	float RightMaxValue = FMath::Max3(UsingThemeColorRight.R, UsingThemeColorRight.G, UsingThemeColorRight.B);
	// Normalize the color values so that the maximum is 1 while maintaining the ratio
	UsingThemeColorLeft /= LeftMaxValue;
	UsingThemeColorRight /= RightMaxValue;
	LeftColor->SetBrushColor(UsingThemeColorLeft);
	RightColor->SetBrushColor(UsingThemeColorRight);

	// Set text
	UpdateNotesOffsetText();

	// Set key binds display
	UpdateKeyBindsUI();

	// Set volume sliders & texts
	BGMVolumeSlider->SetValue(MainGameInstance->BGMVolume);
	HitsoundVolumeSlider->SetValue(MainGameInstance->HitsoundVolume);
	SFXVolumeSlider->SetValue(MainGameInstance->SFXVolume);
	UIVolumeSlider->SetValue(MainGameInstance->UIVolume);
	BGMVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int>(roundf(MainGameInstance->BGMVolume * 100.f)))));
	HitsoundVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int>(roundf(MainGameInstance->HitsoundVolume * 100.f)))));
	SFXVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int>(roundf(MainGameInstance->SFXVolume * 100.f)))));
	UIVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int>(roundf(MainGameInstance->UIVolume * 100.f)))));

	// Highlight current graphics & setup graphics
	UpdateGraphicsButtonStyle();
	UpdateFPSButtonStyle();

	// Set camera shaking effect toggle button
	if (MainGameInstance->bCameraShake)
	{
		CameraShakingEffectToggle->SetCheckedState(ECheckBoxState::Checked);
	}
	else
	{
		CameraShakingEffectToggle->SetCheckedState(ECheckBoxState::Unchecked);
	}

	// Set transparent cube toggle button
	if (MainGameInstance->bTransparentCube)
	{
		TransparentCubeToggle->SetCheckedState(ECheckBoxState::Checked);
	}
	else
	{
		TransparentCubeToggle->SetCheckedState(ECheckBoxState::Unchecked);
	}

	// Set hit assist toggle button
	if (MainGameInstance->bHitAssist)
	{
		HitAssistToggle->SetCheckedState(ECheckBoxState::Checked);
	}
	else
	{
		HitAssistToggle->SetCheckedState(ECheckBoxState::Unchecked);
	}

	PlayAnimation(FadeIn);
}

// Back button
void USettingStateLayoutBase::BackButtonHovered()
{
	PlayAnimation(BackButtonHover);
}

void USettingStateLayoutBase::BackButtonUnHovered()
{
	PlayAnimationReverse(BackButtonHover);
}

void USettingStateLayoutBase::BackButtonClicked()
{
	// Back to main menu
	BackButton->SetIsEnabled(false);
	MainGameMode->MainMenu(true);
	PlayAnimationReverse(FadeIn);
	FTimerHandle AnimationTimer;
	GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this] {
		RemoveFromParent();
		}, 0.5f, false);
}

// Notes offset
void USettingStateLayoutBase::NotesOffsetAddButtonClicked()
{
	if (MainGameInstance->Offset <= 0.59f)
	{
		// + offset
		MainGameInstance->Offset += 0.01f;
		MainGameInstance->SaveGame();
	}
	UpdateNotesOffsetText();
}

void USettingStateLayoutBase::NotesOffsetMinusButtonClicked()
{
	if (MainGameInstance->Offset >= -0.59f)
	{
		// - offset
		MainGameInstance->Offset -= 0.01f;
		MainGameInstance->SaveGame();
	}
	UpdateNotesOffsetText();
}

void USettingStateLayoutBase::NotesOffsetAddButtonPressed()
{
	GetWorld()->GetTimerManager().ClearTimer(ButtonHoldTimer);
	GetWorld()->GetTimerManager().ClearTimer(ChangeLoopTimer);
	GetWorld()->GetTimerManager().SetTimer(ButtonHoldTimer, [this] {
		GetWorld()->GetTimerManager().SetTimer(ChangeLoopTimer, [this] {
			if (NotesOffsetAddButton->IsPressed())
			{
				// Continuously increase
				NotesOffsetAddButtonClicked();
			}
			else
			{
				// Player not pressing anymore
				GetWorld()->GetTimerManager().ClearTimer(ButtonHoldTimer);
				GetWorld()->GetTimerManager().ClearTimer(ChangeLoopTimer);
				MainGameInstance->SaveGame();
			}
			}, 0.1f, true);
		}, 0.5f, false);
}

void USettingStateLayoutBase::NotesOffsetMinusButtonPressed()
{
	GetWorld()->GetTimerManager().ClearTimer(ButtonHoldTimer);
	GetWorld()->GetTimerManager().ClearTimer(ChangeLoopTimer);
	GetWorld()->GetTimerManager().SetTimer(ButtonHoldTimer, [this] {
		GetWorld()->GetTimerManager().SetTimer(ChangeLoopTimer, [this] {
			if (NotesOffsetMinusButton->IsPressed())
			{
				// Continuously decrease
				NotesOffsetMinusButtonClicked();
			}
			else
			{
				// Player not pressing anymore
				GetWorld()->GetTimerManager().ClearTimer(ButtonHoldTimer);
				GetWorld()->GetTimerManager().ClearTimer(ChangeLoopTimer);
				MainGameInstance->SaveGame();
			}
			}, 0.1f, true);
		}, 0.5f, false);
}

void USettingStateLayoutBase::UpdateNotesOffsetText()
{
	if (MainGameInstance->Offset > -0.005f)
	{
		// Positive value
		NotesOffsetText->SetText(FText::FromString(FString::Printf(TEXT("+ %dms"), static_cast<int>(abs(roundf(MainGameInstance->Offset * 1000.f))))));
	}
	else
	{
		// Negative value
		NotesOffsetText->SetText(FText::FromString(FString::Printf(TEXT("- %dms"), static_cast<int>(abs(roundf(MainGameInstance->Offset * 1000.f))))));
	}
}

// Keybinds
void USettingStateLayoutBase::KeyBindsSwapButtonClicked()
{
	// Swap bool
	MainGameInstance->bKeySwap = !MainGameInstance->bKeySwap;
	UpdateKeyBindsUI();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::UpdateKeyBindsUI()
{
	// Swap display
	Cast<UUniformGridSlot>(KeyW->Slot)->SetRow(MainGameInstance->bKeySwap);
	Cast<UUniformGridSlot>(KeyA->Slot)->SetRow(MainGameInstance->bKeySwap);
	Cast<UUniformGridSlot>(KeyS->Slot)->SetRow(MainGameInstance->bKeySwap);
	Cast<UUniformGridSlot>(KeyD->Slot)->SetRow(MainGameInstance->bKeySwap);
	Cast<UUniformGridSlot>(KeyI->Slot)->SetRow(!MainGameInstance->bKeySwap);
	Cast<UUniformGridSlot>(KeyJ->Slot)->SetRow(!MainGameInstance->bKeySwap);
	Cast<UUniformGridSlot>(KeyK->Slot)->SetRow(!MainGameInstance->bKeySwap);
	Cast<UUniformGridSlot>(KeyL->Slot)->SetRow(!MainGameInstance->bKeySwap);
}

// Volumes
void USettingStateLayoutBase::BGMVolumeSliderValueChanged(float Value)
{
	MainGameInstance->BGMVolume = Value;
	BGMVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int>(roundf(MainGameInstance->BGMVolume * 100.f)))));

	MainGameMode->UpdateBGMVolume();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::SFXVolumeSliderValueChanged(float Value)
{
	MainGameInstance->SFXVolume = Value;
	SFXVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int>(roundf(MainGameInstance->SFXVolume * 100.f)))));

	MainGameMode->UpdateSFXVolume();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::UIVolumeSliderValueChanged(float Value)
{
	MainGameInstance->UIVolume = Value;
	UIVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int>(roundf(MainGameInstance->UIVolume * 100.f)))));

	MainGameMode->UpdateUIVolume();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::HitsoundVolumeSliderValueChanged(float Value)
{
	MainGameInstance->HitsoundVolume = Value;
	HitsoundVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int>(roundf(MainGameInstance->HitsoundVolume * 100.f)))));

	MainGameMode->UpdateHitsoundVolume();
	MainGameInstance->SaveGame();
}

// Graphics
void USettingStateLayoutBase::GraphicsLowButtonClicked()
{
	MainGameInstance->Graphics = 0;
	UpdateGraphicsButtonStyle();
	MainGameMode->UpdateGraphics();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::GraphicsMediumButtonClicked()
{
	MainGameInstance->Graphics = 1;
	UpdateGraphicsButtonStyle();
	MainGameMode->UpdateGraphics();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::GraphicsHighButtonClicked()
{
	MainGameInstance->Graphics = 2;
	UpdateGraphicsButtonStyle();
	MainGameMode->UpdateGraphics();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::GraphicsUltraButtonClicked()
{
	MainGameInstance->Graphics = 3;
	UpdateGraphicsButtonStyle();
	MainGameMode->UpdateGraphics();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::GraphicsMaxButtonClicked()
{
	MainGameInstance->Graphics = 4;
	UpdateGraphicsButtonStyle();
	MainGameMode->UpdateGraphics();
	MainGameInstance->SaveGame();
}

// FPS
void USettingStateLayoutBase::FPSLowButtonClicked()
{
	MainGameInstance->MaxFPS = 30;
	UpdateFPSButtonStyle();
	MainGameMode->UpdateFPS();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::FPSMediumButtonClicked()
{
	MainGameInstance->MaxFPS = 60;
	UpdateFPSButtonStyle();
	MainGameMode->UpdateFPS();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::FPSHighButtonClicked()
{
	MainGameInstance->MaxFPS = 90;
	UpdateFPSButtonStyle();
	MainGameMode->UpdateFPS();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::FPSUltraButtonClicked()
{
	MainGameInstance->MaxFPS = 120;
	UpdateFPSButtonStyle();
	MainGameMode->UpdateFPS();
	MainGameInstance->SaveGame();
}

void USettingStateLayoutBase::FPSMaxButtonClicked()
{
	MainGameInstance->MaxFPS = 1000; // 0 = Unlimited? Not quite, limited to 60
	UpdateFPSButtonStyle();
	MainGameMode->UpdateFPS();
	MainGameInstance->SaveGame();
}

// Camera shaking effect
void USettingStateLayoutBase::CameraShakingEffectToggleChanged(bool bIsChecked)
{
	MainGameInstance->bCameraShake = bIsChecked;
	MainGameInstance->SaveGame();
}

// Transparent cube
void USettingStateLayoutBase::TransparentCubeToggleChanged(bool bIsChecked)
{
	MainGameInstance->bTransparentCube = bIsChecked;
	MainGameInstance->SaveGame();
}

// Hit Assist
void USettingStateLayoutBase::HitAssistToggleChanged(bool bIsChecked)
{
	MainGameInstance->bHitAssist = bIsChecked;
	MainGameInstance->SaveGame();
}

// Account delete
void USettingStateLayoutBase::AccountDeleteButtonClicked()
{
	DeleteConfirmMenu->CloseButton->SetIsEnabled(true);
	PlayAnimation(DeleteConfirmFadeIn);
}

void USettingStateLayoutBase::CloseButtonClicked()
{
	DeleteConfirmMenu->CloseButton->SetIsEnabled(false);
	PlayAnimation(DeleteConfirmFadeOut);
}

void USettingStateLayoutBase::TutorialButtonClicked()
{
	// Enter tutorial
	MainGameMode->bTutorial = true;
	BackButton->SetIsEnabled(false);
	MainGameMode->ReadyPlayState(true);
	PlayAnimationReverse(FadeIn);
	FTimerHandle AnimationTimer;
	GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this] {
		RemoveFromParent();
		}, 0.5f, false);
}

// Copyright button
void USettingStateLayoutBase::CopyrightButtonClicked()
{
	CopyrightCloseButton->SetIsEnabled(true);
	PlayAnimation(CopyrightFadeIn);
}

void USettingStateLayoutBase::CopyrightCloseButtonClicked()
{
	CopyrightCloseButton->SetIsEnabled(false);
	PlayAnimation(CopyrightFadeOut);
}

void USettingStateLayoutBase::UpdateGraphicsButtonStyle()
{
	// Set default style
	GraphicsLowButton->SetStyle(NormalButtonStyle);
	GraphicsMediumButton->SetStyle(NormalButtonStyle);
	GraphicsHighButton->SetStyle(NormalButtonStyle);
	GraphicsUltraButton->SetStyle(NormalButtonStyle);
	GraphicsMaxButton->SetStyle(NormalButtonStyle);
	// Pick one to set selected style
	switch (MainGameInstance->Graphics)
	{
	case 0:
		GraphicsLowButton->SetStyle(SelectedButtonStyle);
		break;
	case 1:
		GraphicsMediumButton->SetStyle(SelectedButtonStyle);
		break;
	case 2:
		GraphicsHighButton->SetStyle(SelectedButtonStyle);
		break;
	case 3:
		GraphicsUltraButton->SetStyle(SelectedButtonStyle);
		break;
	case 4:
		GraphicsMaxButton->SetStyle(SelectedButtonStyle);
		break;
	default:;
	}
}

void USettingStateLayoutBase::UpdateFPSButtonStyle()
{
	// Set default style
	FPSLowButton->SetStyle(NormalButtonStyle);
	FPSMediumButton->SetStyle(NormalButtonStyle);
	FPSHighButton->SetStyle(NormalButtonStyle);
	FPSUltraButton->SetStyle(NormalButtonStyle);
	FPSMaxButton->SetStyle(NormalButtonStyle);
	// Pick one to set selected style
	switch (MainGameInstance->MaxFPS)
	{
	case 30:
		FPSLowButton->SetStyle(SelectedButtonStyle);
		break;
	case 60:
		FPSMediumButton->SetStyle(SelectedButtonStyle);
		break;
	case 90:
		FPSHighButton->SetStyle(SelectedButtonStyle);
		break;
	case 120:
		FPSUltraButton->SetStyle(SelectedButtonStyle);
		break;
	case 1000:
		FPSMaxButton->SetStyle(SelectedButtonStyle);
		break;
	default:;
	}
}