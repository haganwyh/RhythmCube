// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreStateLayoutBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "BuyConfirmMenuBase.h"
#include "Kismet/GameplayStatics.h"

void UStoreStateLayoutBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind delegates
	MainGameMode->OnBuySuccess.AddDynamic(this, &UStoreStateLayoutBase::BuySuccess);

	// Bind button delegates
	BackButton->OnHovered.AddDynamic(this, &UStoreStateLayoutBase::BackButtonHovered);
	BackButton->OnUnhovered.AddDynamic(this, &UStoreStateLayoutBase::BackButtonUnHovered);
	BackButton->OnClicked.AddDynamic(this, &UStoreStateLayoutBase::BackButtonClicked);

	CyanNeonpinkBuyButton->OnClicked.AddDynamic(this, &UStoreStateLayoutBase::CyanNeonpinkBuyButtonClicked);
	BlueRedBuyButton->OnClicked.AddDynamic(this, &UStoreStateLayoutBase::BlueRedBuyButtonClicked);
	GreenRedBuyButton->OnClicked.AddDynamic(this, &UStoreStateLayoutBase::GreenRedBuyButtonClicked);
	BlueGoldBuyButton->OnClicked.AddDynamic(this, &UStoreStateLayoutBase::BlueGoldBuyButtonClicked);
	BlueWhiteBuyButton->OnClicked.AddDynamic(this, &UStoreStateLayoutBase::BlueWhiteBuyButtonClicked);

	BuyConfirmMenu->CloseButton->OnClicked.AddDynamic(this, &UStoreStateLayoutBase::CloseButtonClicked);

	// Set up brushes
	UseButtonBrush.SetResourceObject(UseButtonTexture);
	UseButtonBrush.SetImageSize(FVector2D(391.799988f, 85.800003f));
	UsingButtonBrush.SetResourceObject(UsingButtonTexture);
	UsingButtonBrush.SetImageSize(FVector2D(391.799988f, 85.800003f));
	UseButtonStyle.Normal = UseButtonBrush;
	UseButtonStyle.Normal.TintColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	UseButtonStyle.Hovered = UseButtonBrush;
	UseButtonStyle.Hovered.TintColor = FLinearColor(1.f, 1.f, 1.f, 0.75f);
	UseButtonStyle.Pressed = UseButtonBrush;
	UseButtonStyle.Pressed.TintColor = FLinearColor(1.f, 1.f, 1.f, 0.5f);
	UseButtonStyle.Disabled = UsingButtonBrush;
	UseButtonStyle.Disabled.TintColor = FLinearColor(1.f, 1.f, 1.f, 0.3f);
	UseButtonStyle.SetPressedSound(ClickSound);
	UseButtonStyle.SetHoveredSound(HoverSound);

	// Update texts
	UpdateMoneyText();

	// Update buttons
	UpdateAllBuyButtons();

	// Update prices
	UpdateOwnedState();

	PlayAnimation(FadeIn);

	// Background animation
	PlayAnimation(BackgroundAnimation);
	GetWorld()->GetTimerManager().SetTimer(BackgroundAnimationTimer, [this] {
		PlayAnimation(BackgroundAnimation);
		}, 4.f, true);
}

// Back button
void UStoreStateLayoutBase::BackButtonHovered()
{
	PlayAnimation(BackButtonHover);
}

void UStoreStateLayoutBase::BackButtonUnHovered()
{
	PlayAnimationReverse(BackButtonHover);
}

void UStoreStateLayoutBase::BackButtonClicked()
{
	// Back to main menu
	BackButton->SetIsEnabled(false);
	MainGameMode->MainMenu(true);
	PlayAnimationReverse(FadeIn);
	FTimerHandle AnimationTimer;
	GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this] {
		GetWorld()->GetTimerManager().ClearTimer(BackgroundAnimationTimer);
		RemoveFromParent();
		}, 0.5f, false);
}

void UStoreStateLayoutBase::UpdateMoneyText()
{
	MoneyText->SetText(FText::AsNumber(MainGameInstance->Money));
}

void UStoreStateLayoutBase::UpdateBuyButtons(UButton* TargetButton, Theme TargetTheme)
{
	if (MainGameInstance->UsingTheme == TargetTheme)
	{
		// Button "using", shouldn't be clicked
		TargetButton->SetStyle(UseButtonStyle);
		TargetButton->SetIsEnabled(false);
	}
	else if (MainGameInstance->OwnedTheme.Contains(TargetTheme))
	{
		// Button for players to use
		TargetButton->SetStyle(UseButtonStyle);
		TargetButton->SetIsEnabled(true);
	}
}

void UStoreStateLayoutBase::CyanNeonpinkBuyButtonClicked()
{
	BuyButtonClickedEvent(Theme::CyanNeonpink);
}

void UStoreStateLayoutBase::BlueRedBuyButtonClicked()
{
	BuyButtonClickedEvent(Theme::BlueRed);
}

void UStoreStateLayoutBase::GreenRedBuyButtonClicked()
{
	BuyButtonClickedEvent(Theme::GreenRed);
}

void UStoreStateLayoutBase::BlueGoldBuyButtonClicked()
{
	BuyButtonClickedEvent(Theme::BlueGold);
}

void UStoreStateLayoutBase::BlueWhiteBuyButtonClicked()
{
	BuyButtonClickedEvent(Theme::BlueWhite);
}

void UStoreStateLayoutBase::CloseButtonClicked()
{
	BuyConfirmMenu->CloseButton->SetIsEnabled(false);
	PlayAnimation(BuyConfirmFadeOut);
}

void UStoreStateLayoutBase::UpdateAllBuyButtons()
{
	// Update each button
	UpdateBuyButtons(CyanNeonpinkBuyButton, Theme::CyanNeonpink);
	UpdateBuyButtons(BlueRedBuyButton, Theme::BlueRed);
	UpdateBuyButtons(GreenRedBuyButton, Theme::GreenRed);
	UpdateBuyButtons(BlueGoldBuyButton, Theme::BlueGold);
	UpdateBuyButtons(BlueWhiteBuyButton, Theme::BlueWhite);
}

void UStoreStateLayoutBase::BuySuccess()
{
	PlayAnimation(BuyConfirmFadeOut);
	UpdateAllBuyButtons();
	UpdateMoneyText();
	UpdateOwnedState();
	MainGameInstance->SaveGame();
}

void UStoreStateLayoutBase::UpdateOwnedState()
{
	FSlateColor OwnedColor = FLinearColor(0.f, 1.f, 0.f, 1.f); // Green color
	TArray<UTextBlock*> DollarSigns = { CyanNeonpinkDollarSign, GreenRedDollarSign , BlueGoldDollarSign , BlueWhiteDollarSign };
	TArray<UTextBlock*> MoneyTexts = { CyanNeonpinkMoneyText , GreenRedMoneyText , BlueGoldMoneyText , BlueWhiteMoneyText };
	TArray<Theme> Themes = { Theme::CyanNeonpink, Theme::GreenRed, Theme::BlueGold, Theme::BlueWhite };
	// Loop through each theme, to check does the price needed to set to "Owned"
	for (int i = 0; i < DollarSigns.Num(); i++)
	{
		if (MainGameInstance->OwnedTheme.Contains(Themes[i]))
		{
			DollarSigns[i]->SetVisibility(ESlateVisibility::Collapsed);
			MoneyTexts[i]->SetText(FText::FromString("Owned"));
			MoneyTexts[i]->SetColorAndOpacity(OwnedColor);
		}
	}
}

void UStoreStateLayoutBase::BuyButtonClickedEvent(Theme TargetTheme)
{
	if (MainGameInstance->OwnedTheme.Contains(TargetTheme))
	{
		MainGameInstance->UsingTheme = TargetTheme;
		UpdateAllBuyButtons();
		MainGameMode->UpdateColorTheme();
		MainGameInstance->SaveGame();
	}
	else
	{
		BuyConfirmMenu->UpdatePriceText(TargetTheme);
		PlayAnimation(BuyConfirmFadeIn);
	}
}