// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseStateLayoutBase.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "MainPlayerController.h"
#include "Components/Button.h"

void UPauseStateLayoutBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Bind button delegates
	ResumeButton->OnClicked.AddDynamic(this, &UPauseStateLayoutBase::Resume);
	HomeButton->OnClicked.AddDynamic(this, &UPauseStateLayoutBase::MainMenu);
}

void UPauseStateLayoutBase::Resume()
{
	Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->HandlePause();
}

void UPauseStateLayoutBase::MainMenu()
{
	HomeButton->SetIsEnabled(false);
	ResumeButton->SetIsEnabled(false);

	MainGameMode->MainMenu(false);
	PlayAnimation(MenuFadeOut);
	FTimerHandle AnimationTimer;
	GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this] {
		RemoveFromParent();
		}, 0.25f, false);
}