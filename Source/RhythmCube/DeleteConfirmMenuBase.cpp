// Fill out your copyright notice in the Description page of Project Settings.


#include "DeleteConfirmMenuBase.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UDeleteConfirmMenuBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind button delegates
	ConfirmDeleteButton->OnClicked.AddDynamic(this, &UDeleteConfirmMenuBase::ConfirmDeleteButtonClicked);
}

void UDeleteConfirmMenuBase::ConfirmDeleteButtonClicked()
{
	MainGameInstance->CreateSaveFile(); // Override the existed file
	MainGameMode->ExitState();
}