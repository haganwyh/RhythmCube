// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseSongMenuBase.h"
#include "Components/Button.h"
#include "MainGameInstance.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UChooseSongMenuBase::NativeOnInitialized()
{
	// Set references
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	// Bind button delegates
	LeftButton->OnHovered.AddDynamic(this, &UChooseSongMenuBase::LeftButtonHovered);
	LeftButton->OnUnhovered.AddDynamic(this, &UChooseSongMenuBase::LeftButtonUnHovered);
	RightButton->OnHovered.AddDynamic(this, &UChooseSongMenuBase::RightButtonHovered);
	RightButton->OnUnhovered.AddDynamic(this, &UChooseSongMenuBase::RightButtonUnHovered);

	LeftButton->OnClicked.AddDynamic(this, &UChooseSongMenuBase::LeftButtonClicked);
	RightButton->OnClicked.AddDynamic(this, &UChooseSongMenuBase::RightButtonClicked);

	// Bind delegates
	MainGameMode->OnSettingState.AddDynamic(this, &UChooseSongMenuBase::Hide);
	MainGameMode->OnStoreState.AddDynamic(this, &UChooseSongMenuBase::Hide);
	MainGameMode->OnUpdateSelectedSong.AddDynamic(this, &UChooseSongMenuBase::UpdateSelectedSong);
	MainGameMode->OnMainMenu.AddDynamic(this, &UChooseSongMenuBase::MainMenu);

	UpdateSelectedSong(MainGameInstance->SelectedSongID, MainGameMode->SongNames[MainGameInstance->SelectedSongID]);
}

// Hovering events
void UChooseSongMenuBase::LeftButtonHovered()
{
	PlayAnimation(LeftButtonHover);
}

void UChooseSongMenuBase::LeftButtonUnHovered()
{
	PlayAnimation(LeftButtonUnHover);
}

void UChooseSongMenuBase::RightButtonHovered()
{
	PlayAnimation(RightButtonHover);
}

void UChooseSongMenuBase::RightButtonUnHovered()
{
	PlayAnimation(RightButtonUnHover);
}

// Clicking events
void UChooseSongMenuBase::LeftButtonClicked()
{
	int NewSelectedSongID = (MainGameInstance->SelectedSongID - 1) % MainGameMode->SongNames.Num();
	if (NewSelectedSongID < 0)
	{
		MainGameInstance->SelectedSongID = MainGameMode->SongNames.Num() + NewSelectedSongID;
	}
	else
	{
		MainGameInstance->SelectedSongID = NewSelectedSongID;
	}
	MainGameMode->UpdateSelectedSong();
}

void UChooseSongMenuBase::RightButtonClicked()
{
	MainGameInstance->SelectedSongID = (MainGameInstance->SelectedSongID + 1) % MainGameMode->SongNames.Num();
	MainGameMode->UpdateSelectedSong();
}

void UChooseSongMenuBase::Hide()
{
	PlayAnimation(FadeOut);
}

void UChooseSongMenuBase::UpdateSelectedSong(int ID, FString SongName)
{
	SongNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *SongName)));
	ArtistText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *MainGameMode->SongCredit[ID][0])));
	IllustrationText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *MainGameMode->SongCredit[ID][1])));
	SongCoverImage->SetBrushFromTexture(SongCoverTextures[ID]);
}

void UChooseSongMenuBase::MainMenu(bool bNew)
{
	if (!MainGameInstance->bNewPlayer)
	{
		PlayAnimation(FadeIn);
	}
}