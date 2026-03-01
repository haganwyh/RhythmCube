// Fill out your copyright notice in the Description page of Project Settings.


#include "SongCoverBase.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "Components/StaticMeshComponent.h"

void ASongCoverBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MeshComponent = FindComponentByClass<UStaticMeshComponent>();

	// Bind delegates
	MainGameMode->OnUpdateSelectedSong.AddDynamic(this, &ASongCoverBase::UpdateSongName);
	MainGameMode->OnSettingState.AddDynamic(this, &ASongCoverBase::Hide);
	MainGameMode->OnStoreState.AddDynamic(this, &ASongCoverBase::Hide);
	MainGameMode->OnMainMenu.AddDynamic(this, &ASongCoverBase::MainMenu);

	UpdateSongName(MainGameInstance->SelectedSongID, MainGameMode->SongNames[MainGameInstance->SelectedSongID]);
}

void ASongCoverBase::UpdateSongName(int ID, FString SongName)
{
	MeshComponent->SetMaterial(0, SongCoverMaterials[ID]);
}

void ASongCoverBase::Hide()
{
	SetActorHiddenInGame(true);
}

void ASongCoverBase::MainMenu(bool bNew)
{
	if (!MainGameInstance->bNewPlayer)
	{
		SetActorHiddenInGame(false);
	}
}