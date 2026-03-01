// Fill out your copyright notice in the Description page of Project Settings.


#include "PreStartingMenuLayoutBase.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UPreStartingMenuLayoutBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	PlayAnimation(FadeIn);
	FTimerHandle FadeInAnimationTimer;
	GetWorld()->GetTimerManager().SetTimer(FadeInAnimationTimer, [this]
	{
		MainGameMode->BenchmarkGraphicsInitialize();

		FTimerHandle LogoAnimationTimer;
		GetWorld()->GetTimerManager().SetTimer(LogoAnimationTimer, [this]
		{
			PlayAnimation(FadeOut);
		}, 0.5f, false);

		FTimerHandle HeadphoneAnimationTimer;
		GetWorld()->GetTimerManager().SetTimer(HeadphoneAnimationTimer, [this]
		{
			PlayAnimation(HeadphoneFadeInOut);

		}, 1.5f, false);

		FTimerHandle FadeOutAnimationTimer;
		GetWorld()->GetTimerManager().SetTimer(FadeOutAnimationTimer, [this]
		{
			PlayAnimation(FadeOutLast);
			MainGameMode->StartState();

		}, 7.f, false);

		FTimerHandle SelfDestoryTimer;
		GetWorld()->GetTimerManager().SetTimer(SelfDestoryTimer, [this]
		{
			RemoveFromParent();

		}, 8.f, false);

	}, (MainGameInstance->bNewPlayer) ? 1.f : 2.5f, false);
}