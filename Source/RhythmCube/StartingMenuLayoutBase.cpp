// Fill out your copyright notice in the Description page of Project Settings.


#include "StartingMenuLayoutBase.h"
#include "Components/Button.h"
#include "MainGameMode.h"
#include "StartingPawn.h"
#include "Kismet/GameplayStatics.h"

void UStartingMenuLayoutBase::NativeOnInitialized()
{	
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Bind button delegates
	TapButton->OnClicked.AddDynamic(this, &UStartingMenuLayoutBase::TapButtonClicked);

	// Play animation
	PlayAnimation(FadeIn);

	GetWorld()->GetTimerManager().SetTimer(MusicBeatTimer, [this] {
		StartingPawn->CameraShake(0.05f);
		}, 60.f / 151.6376f * 2.f, true);

	PlayAnimation(TapTextLoop);
	GetWorld()->GetTimerManager().SetTimer(TapTextLoopTimer, [this] {
		PlayAnimation(TapTextLoop);
		}, 6.f, true);

}

void UStartingMenuLayoutBase::TapButtonClicked()
{
	TapButton->SetIsEnabled(false);
	MainGameMode->ReadyMainMenu();
	GetWorld()->GetTimerManager().ClearTimer(MusicBeatTimer);
	GetWorld()->GetTimerManager().ClearTimer(TapTextLoopTimer);

	PlayAnimation(FadeOut);
	FTimerHandle SelfDestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(SelfDestroyTimer, [this] {
		RemoveFromParent();
		}, 0.5f, false);
}