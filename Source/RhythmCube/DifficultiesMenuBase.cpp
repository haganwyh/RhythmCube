// Fill out your copyright notice in the Description page of Project Settings.


#include "DifficultiesMenuBase.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "Components/Button.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CheckBox.h"
#include "Components/Border.h"

void UDifficultiesMenuBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind delegates
	MainGameMode->OnMainMenu.AddDynamic(this, &UDifficultiesMenuBase::MainMenu);
	MainGameMode->OnReadyPlayState.AddDynamic(this, &UDifficultiesMenuBase::ReadyPlayState);
	MainGameMode->OnSettingState.AddDynamic(this, &UDifficultiesMenuBase::Hide);
	MainGameMode->OnStoreState.AddDynamic(this, &UDifficultiesMenuBase::Hide);
	MainGameMode->OnLoadGame.AddDynamic(this, &UDifficultiesMenuBase::LoadGame);
	 
	// Bind button delegates
	NormalButton->OnHovered.AddDynamic(this, &UDifficultiesMenuBase::NormalButtonHovered);
	NormalButton->OnUnhovered.AddDynamic(this, &UDifficultiesMenuBase::NormalButtonUnHovered);
	NormalButton->OnClicked.AddDynamic(this, &UDifficultiesMenuBase::NormalButtonClicked);
	HardButton->OnHovered.AddDynamic(this, &UDifficultiesMenuBase::HardButtonHovered);
	HardButton->OnUnhovered.AddDynamic(this, &UDifficultiesMenuBase::HardButtonUnHovered);
	HardButton->OnClicked.AddDynamic(this, &UDifficultiesMenuBase::HardButtonClicked);
	ExtremeButton->OnHovered.AddDynamic(this, &UDifficultiesMenuBase::ExtremeButtonHovered);
	ExtremeButton->OnUnhovered.AddDynamic(this, &UDifficultiesMenuBase::ExtremeButtonUnHovered);
	ExtremeButton->OnClicked.AddDynamic(this, &UDifficultiesMenuBase::ExtremeButtonClicked);
	HardcoreToggle->OnCheckStateChanged.AddDynamic(this, &UDifficultiesMenuBase::HardcoreToggleChanged);

	// Set variables
	NormalButtonBrush.SetResourceObject(NormalBackgroundImage); // Normal button style
	NormalButtonBrush.SetImageSize(FVector2D(1490.f, 250.f));
	NormalButtonStyle.Normal = NormalButtonBrush;
	NormalSelectedButtonBrush.SetResourceObject(NormalBackgroundImageSelected);
	NormalSelectedButtonBrush.SetImageSize(FVector2D(1490.f, 250.f));
	NormalSelectedButtonStyle.Normal = NormalSelectedButtonBrush;

	HardButtonBrush.SetResourceObject(HardBackgroundImage); // Hard button style
	HardButtonBrush.SetImageSize(FVector2D(1490.f, 250.f));
	HardButtonStyle.Normal = HardButtonBrush;
	HardSelectedButtonBrush.SetResourceObject(HardBackgroundImageSelected);
	HardSelectedButtonBrush.SetImageSize(FVector2D(1490.f, 250.f));
	HardSelectedButtonStyle.Normal = HardSelectedButtonBrush;

	ExtremeButtonBrush.SetResourceObject(ExtremeBackgroundImage); // Extreme button style
	ExtremeButtonBrush.SetImageSize(FVector2D(1490.f, 250.f));
	ExtremeButtonStyle.Normal = ExtremeButtonBrush;
	ExtremeSelectedButtonBrush.SetResourceObject(ExtremeBackgroundImageSelected);
	ExtremeSelectedButtonBrush.SetImageSize(FVector2D(1490.f, 250.f));
	ExtremeSelectedButtonStyle.Normal = ExtremeSelectedButtonBrush;
}

void UDifficultiesMenuBase::MainMenu(bool bNew)
{
	if (!MainGameInstance->bNewPlayer)
	{
		if (bNew)
		{
			PlayAnimation(FadeIn);
		}
		else
		{
			FTimerHandle AnimationTimer;
			GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this] {
				PlayAnimation(FadeIn);
				}, 2.f, false);
		}
	}
}

void UDifficultiesMenuBase::ReadyPlayState(bool bNew)
{
	if (bNew && !MainGameMode->bTutorial)
	{
		// Only need to fade out when it is visible, i.e. from main menu
		PlayAnimation(FadeOut);
	}
}

void UDifficultiesMenuBase::NormalButtonHovered()
{
	NormalButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
}

void UDifficultiesMenuBase::NormalButtonUnHovered()
{
	NormalButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
}

void UDifficultiesMenuBase::NormalButtonClicked()
{
	MainGameInstance->SelectedDifficulty = 0;
	UpdateButton();
}

void UDifficultiesMenuBase::HardButtonHovered()
{
	HardButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
}

void UDifficultiesMenuBase::HardButtonUnHovered()
{
	HardButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
}

void UDifficultiesMenuBase::HardButtonClicked()
{
	MainGameInstance->SelectedDifficulty = 1;
	UpdateButton();
}

void UDifficultiesMenuBase::ExtremeButtonHovered()
{
	ExtremeButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
}

void UDifficultiesMenuBase::ExtremeButtonUnHovered()
{
	ExtremeButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
}

void UDifficultiesMenuBase::ExtremeButtonClicked()
{
	MainGameInstance->SelectedDifficulty = 2;
	UpdateButton();
}

void UDifficultiesMenuBase::UpdateButton()
{
	// Set all button back to normal state
	NormalBackground->SetStyle(NormalButtonStyle);
	HardBackground->SetStyle(HardButtonStyle);
	ExtremeBackground->SetStyle(ExtremeButtonStyle);

	// Determine which button needs to be in selected state
	switch (MainGameInstance->SelectedDifficulty)
	{
	case 0:
		NormalBackground->SetStyle(NormalSelectedButtonStyle);
		break;
	case 1:
		HardBackground->SetStyle(HardSelectedButtonStyle);
		break;
	case 2:
		ExtremeBackground->SetStyle(ExtremeSelectedButtonStyle);
		break;
	default:;
	}

	MainGameMode->OnUpdateSongInfo.Broadcast(MainGameInstance->SelectedSongID, MainGameMode->SongNames[MainGameInstance->SelectedSongID]);
	MainGameInstance->SaveGame();
}

void UDifficultiesMenuBase::HardcoreToggleChanged(bool bIsChecked)
{
	MainGameInstance->bHardcoreMode = bIsChecked;
	if (bIsChecked)
	{
		HardcoreBackground->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HardcoreBackground->SetVisibility(ESlateVisibility::Collapsed);
	}

	MainGameMode->OnUpdateSongInfo.Broadcast(MainGameInstance->SelectedSongID, MainGameMode->SongNames[MainGameInstance->SelectedSongID]);
	MainGameInstance->SaveGame();
}

void UDifficultiesMenuBase::Hide()
{
	PlayAnimation(FadeOut, 0.5f);
}

void UDifficultiesMenuBase::LoadGame()
{
	// Initialize buttons
	UpdateButton();
	if (MainGameInstance->bHardcoreMode)
	{
		HardcoreToggle->SetCheckedState(ECheckBoxState::Checked);
		HardcoreBackground->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HardcoreToggle->SetCheckedState(ECheckBoxState::Unchecked);
		HardcoreBackground->SetVisibility(ESlateVisibility::Collapsed);
	}
}