// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverStateLayoutBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "Components/Border.h"

void UGameOverStateLayoutBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind button delegates
	RetryButton->OnClicked.AddDynamic(this, &UGameOverStateLayoutBase::Retry);
	HomeButton->OnClicked.AddDynamic(this, &UGameOverStateLayoutBase::MainMenu);

	// Set image and size of grade
	GradeImage->SetBrushFromTexture(GradeImagesBase[static_cast<int32>(MainGameMode->PlayerGrade)]);
	FSlateBrush GradeBrush = GradeImage->GetBrush(); // Create brush then set brush must work, set desired size is not working by some reason
	UHorizontalBoxSlot* GradeSlot = Cast<UHorizontalBoxSlot>(GradeImage->Slot);
	FMargin GradeSlotPadding;

	// X+
	if (MainGameMode->PlayerGrade == Grade::SSS || MainGameMode->PlayerGrade == Grade::AAA || MainGameMode->PlayerGrade == Grade::BBB
		|| MainGameMode->PlayerGrade == Grade::CCC)
	{
		GradeBrush.SetImageSize(FVector2D(317.f, 175.f));
		// Adjust padding, because it is in a horizontal slot
		GradeSlotPadding.Left = 175.f;
		GradeSlotPadding.Right = 175.f;
	}
	// X
	else if (MainGameMode->PlayerGrade == Grade::SS || MainGameMode->PlayerGrade == Grade::AA || MainGameMode->PlayerGrade == Grade::BB
		|| MainGameMode->PlayerGrade == Grade::CC || MainGameMode->PlayerGrade == Grade::FF)
	{
		GradeBrush.SetImageSize(FVector2D(177.f, 175.f));
		// Adjust padding, because it is in a horizontal slot
		GradeSlotPadding.Left = 245.f;
		GradeSlotPadding.Right = 245.f;
	}
	// X-
	else if (MainGameMode->PlayerGrade == Grade::S || MainGameMode->PlayerGrade == Grade::A || MainGameMode->PlayerGrade == Grade::B
		|| MainGameMode->PlayerGrade == Grade::C)
	{
		GradeBrush.SetImageSize(FVector2D(270, 175.f));
		// Adjust padding, because it is in a horizontal slot
		GradeSlotPadding.Left = 199.f;
		GradeSlotPadding.Right = 199.f;
	}

	GradeImage->SetBrush(GradeBrush);
	GradeSlot->SetPadding(GradeSlotPadding);

	// Set player status
	ScoreText->SetText(FText::AsNumber(static_cast<int>(MainGameMode->Score)));
	MaxComboText->SetText(FText::AsNumber(MainGameMode->MaxCombo));
	MoneyGainText->SetText(FText::FromString(FString::Printf(TEXT("+ $%d"), MainGameMode->MoneyGain)));

	
	if (MainGameMode->bTutorial)
	{
		// Set difficulty display
		DifficultyText->SetText(FText::FromString(TEXT("Easy")));

		// Set song name text
		SongNameText->SetText(FText::FromString(TEXT("Tutorial")));

		// Set background image
		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(BackgroundImagesBase.Last());
		Background->SetBrush(BackgroundBrush);

		// Always hide "Hardcore Mode" in tutorial
		HardcoreModeText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		// Set difficulty display
		switch (MainGameInstance->SelectedDifficulty)
		{
		case 0:
			DifficultyText->SetText(FText::FromString(TEXT("Normal")));
			break;
		case 1:
			DifficultyText->SetText(FText::FromString(TEXT("Hard")));
			break;
		case 2:
			DifficultyText->SetText(FText::FromString(TEXT("Extreme")));
			break;
		default:;
		}

		// Set song name text
		SongNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *MainGameMode->SongNames[MainGameInstance->SelectedSongID])));

		// Set background image
		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(BackgroundImagesBase[MainGameInstance->SelectedSongID]);
		Background->SetBrush(BackgroundBrush);

		// Show "Hardcore Mode" only when in hardcore mode
		if (MainGameInstance->bHardcoreMode)
		{
			HardcoreModeText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			HardcoreModeText->SetVisibility(ESlateVisibility::Collapsed);
		}

		// Highest score banner
		if (MainGameMode->bHighestScore)
		{
			FTimerHandle AnimationTimer;
			GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this] {
				PlayAnimation(HighestScoreFadeIn);
				}, 1.5f, false);
		}
	}

	// Set completed icon image
	if (MainGameMode->Score == 1000000.f)
	{
		FSlateBrush CompletedIconBrush;
		CompletedIconBrush.SetResourceObject(CompletedIconFullScore);
		CompletedIconBrush.SetImageSize(FVector2D(550.f, 550.f));
		CompletedIcon->SetBrush(CompletedIconBrush);
	}

	// All prepared, start animation
	if (MainGameMode->bTutorial)
	{
		PlayAnimation(FadeInLimited);
	}
	else
	{
		PlayAnimation(FadeIn);
	}
}

void UGameOverStateLayoutBase::Retry()
{
	RetryButton->SetIsEnabled(false);

	// Retry and back to ready play state
	MainGameMode->ReadyPlayState(false);
	PlayAnimation(FadeOut);
	
	// Self destroy after fade out effect
	FTimerHandle SelfDestroyTimer;
	MainGameMode->GetWorldTimerManager().SetTimer(SelfDestroyTimer, this, &UGameOverStateLayoutBase::RemoveFromParent, 0.5f, false);
}

void UGameOverStateLayoutBase::MainMenu()
{
	HomeButton->SetIsEnabled(false);
	RetryButton->SetIsEnabled(false);

	if (MainGameMode->bTutorial)
	{
		MainGameMode->bTutorial = false;
	}

	// Retry and back to ready play state
	MainGameMode->MainMenu(false);
	PlayAnimation(FadeOut);

	// Self destroy after fade out effect
	FTimerHandle SelfDestroyTimer;
	MainGameMode->GetWorldTimerManager().SetTimer(SelfDestroyTimer, this, &UGameOverStateLayoutBase::RemoveFromParent, 0.5f, false);
}