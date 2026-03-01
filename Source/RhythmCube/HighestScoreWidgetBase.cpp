// Fill out your copyright notice in the Description page of Project Settings.


#include "HighestScoreWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UHighestScoreWidgetBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind delegates
	MainGameMode->OnMainMenu.AddDynamic(this, &UHighestScoreWidgetBase::MainMenu);
	MainGameMode->OnReadyPlayState.AddDynamic(this, &UHighestScoreWidgetBase::ReadyPlayState);
	MainGameMode->OnSettingState.AddDynamic(this, &UHighestScoreWidgetBase::Hide);
	MainGameMode->OnStoreState.AddDynamic(this, &UHighestScoreWidgetBase::Hide);
	MainGameMode->OnUpdateSelectedSong.AddDynamic(this, &UHighestScoreWidgetBase::UpdateSongInfo);
	MainGameMode->OnUpdateSongInfo.AddDynamic(this, &UHighestScoreWidgetBase::UpdateSongInfo);
}

void UHighestScoreWidgetBase::MainMenu(bool bNew)
{
	if (!MainGameInstance->bNewPlayer)
	{
		// Appear
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

void UHighestScoreWidgetBase::ReadyPlayState(bool bNew)
{
	if (bNew && !MainGameMode->bTutorial)
	{
		// Only need to fade out when it is visible, i.e. from main menu
		PlayAnimation(FadeOut);
	}
}

void UHighestScoreWidgetBase::Hide()
{
	PlayAnimation(FadeOut, 0.5f);
}

void UHighestScoreWidgetBase::UpdateSongInfo(int SongID, FString SongName)
{
	// Highest score display set up
	int* SelectedHighestScore = MainGameInstance->GetHighestScore();
	if (SelectedHighestScore == nullptr)
	{
		// Set text to 0
		HighestScoreText->SetText(FText::FromString(TEXT("0")));

		// Set grade image to NEW!
		GradeImage->SetBrushFromTexture(GradeImagesBase.Last());
		FSlateBrush GradeBrush = GradeImage->GetBrush();
		GradeBrush.SetImageSize(FVector2D(375.f, 85.f));
		GradeImage->SetBrush(GradeBrush);
	}
	else
	{
		// Set text to highest score
		HighestScoreText->SetText(FText::AsNumber(*SelectedHighestScore));

		// Set image and size of grade
		Grade CurrentPlayerGrade = MainGameMode->GetPlayerGrade(*SelectedHighestScore);
		GradeImage->SetBrushFromTexture(GradeImagesBase[static_cast<int32>(CurrentPlayerGrade)]);
		FSlateBrush GradeBrush = GradeImage->GetBrush();

		// X+
		if (CurrentPlayerGrade == Grade::SSS || CurrentPlayerGrade == Grade::AAA || CurrentPlayerGrade == Grade::BBB
			|| CurrentPlayerGrade == Grade::CCC)
		{
			GradeBrush.SetImageSize(FVector2D(317.f, 175.f));
		}
		// X
		else if (CurrentPlayerGrade == Grade::SS || CurrentPlayerGrade == Grade::AA || CurrentPlayerGrade == Grade::BB
			|| CurrentPlayerGrade == Grade::CC || CurrentPlayerGrade == Grade::FF)
		{
			GradeBrush.SetImageSize(FVector2D(177.f, 175.f));
		}
		// X-
		else if (CurrentPlayerGrade == Grade::S || CurrentPlayerGrade == Grade::A || CurrentPlayerGrade == Grade::B
			|| CurrentPlayerGrade == Grade::C)
		{
			GradeBrush.SetImageSize(FVector2D(270, 175.f));
		}

		// Set grade to highest grade
		GradeImage->SetBrush(GradeBrush);
	}
}