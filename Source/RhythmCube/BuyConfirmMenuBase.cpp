// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyConfirmMenuBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MainGameInstance.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"

void UBuyConfirmMenuBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind button delegates
	BuyConfirmButton->OnClicked.AddDynamic(this, &UBuyConfirmMenuBase::BuyConfirmButtonClicked);
}

void UBuyConfirmMenuBase::UpdatePriceText(Theme UpdatingTheme)
{
	// Being called when the menu displayed
	// Prevent fast pressing buttons and break the game
	CloseButton->SetIsEnabled(true);
	BuyConfirmButton->SetIsEnabled(true);
	
	// Assign text and theme into variables
	ThemePrice = MainGameMode->ThemePrices[static_cast<int32>(UpdatingTheme)];
	TargetTheme = UpdatingTheme;
	PriceText->SetText(FText::AsNumber(ThemePrice));

	// Hide no money warning first
	NoMoneyText->SetVisibility(ESlateVisibility::Collapsed);
}

void UBuyConfirmMenuBase::BuyConfirmButtonClicked()
{
	if (MainGameInstance->Money >= ThemePrice)
	{
		// Prevent fast pressing buttons and break the game
		CloseButton->SetIsEnabled(false);
		BuyConfirmButton->SetIsEnabled(false);

		// Buy success
		MainGameInstance->Money -= ThemePrice;
		MainGameInstance->OwnedTheme.Add(TargetTheme);
		MainGameMode->OnBuySuccess.Broadcast();
		MainGameMode->PlaySoundEffects(SoundEffects::Positive);
	}
	else
	{
		// Buy failed
		NoMoneyText->SetVisibility(ESlateVisibility::Visible);
		MainGameMode->PlaySoundEffects(SoundEffects::Warning);
	}
}