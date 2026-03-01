// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverHardcoreLayoutBase.h"
#include "Components/Button.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"

void UGameOverHardcoreLayoutBase::NativeOnInitialized()
{
	// Set reference
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Bind button delegates
	RetryButton->OnClicked.AddDynamic(this, &UGameOverHardcoreLayoutBase::Retry);
	HomeButton->OnClicked.AddDynamic(this, &UGameOverHardcoreLayoutBase::MainMenu);

	// Animation
	PlayAnimation(FadeIn);
}

void UGameOverHardcoreLayoutBase::Retry()
{
	RetryButton->SetIsEnabled(false);

	// Retry and back to ready play state
	MainGameMode->ReadyPlayState(false);
	PlayAnimation(FadeOut);

	// Self destroy after fade out effect
	FTimerHandle SelfDestroyTimer;
	MainGameMode->GetWorldTimerManager().SetTimer(SelfDestroyTimer, this, &UGameOverHardcoreLayoutBase::RemoveFromParent, 0.5f, false);
}

void UGameOverHardcoreLayoutBase::MainMenu()
{
	HomeButton->SetIsEnabled(false);
	RetryButton->SetIsEnabled(false);

	// Retry and back to ready play state
	MainGameMode->MainMenu(false);
	PlayAnimation(FadeOut);

	// Self destroy after fade out effect
	FTimerHandle SelfDestroyTimer;
	MainGameMode->GetWorldTimerManager().SetTimer(SelfDestroyTimer, this, &UGameOverHardcoreLayoutBase::RemoveFromParent, 0.5f, false);
}