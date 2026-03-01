// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitStateLayoutBase.h"
#include "Kismet/GameplayStatics.h"

void UExitStateLayoutBase::NativeOnInitialized()
{
	PlayAnimation(FadeInOut);
	FTimerHandle ExitTimer;
	GetWorld()->GetTimerManager().SetTimer(ExitTimer, [this] {
		// Exit the game
		UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
		}, 4.f, false);
}