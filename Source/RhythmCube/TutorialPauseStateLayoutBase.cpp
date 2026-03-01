// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialPauseStateLayoutBase.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UTutorialPauseStateLayoutBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind delegates
	MainGameMode->OnUnPauseState.AddDynamic(this, &UTutorialPauseStateLayoutBase::Hide);
	MainGameMode->OnGameOverState.AddDynamic(this, &UTutorialPauseStateLayoutBase::Hide);

	// Show quit text if suitable
	if (MainGameInstance->bNewPlayer)
	{
		QuitText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		QuitText->SetVisibility(ESlateVisibility::Visible);
	}

	// Play animations
	PlayAnimation(FadeIn);

	PlayAnimation(ContinueTextLoop);
	GetWorld()->GetTimerManager().SetTimer(ContinueTextLoopTimer, [this] {
		PlayAnimation(ContinueTextLoop);
		}, 6.f, true);
}

void UTutorialPauseStateLayoutBase::Hide()
{
	PlayAnimation(FadeOut);
	GetWorld()->GetTimerManager().ClearTimer(ContinueTextLoopTimer);
	
	FTimerHandle SelfDestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(SelfDestroyTimer, this, &UTutorialPauseStateLayoutBase::RemoveFromParent, 0.25f, false);
}