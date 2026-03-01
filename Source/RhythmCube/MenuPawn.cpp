// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPawn.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"

void AMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	// Bind delegates
	MainGameMode->OnMainMenu.AddDynamic(this, &AMenuPawn::ScreenMotion);
	MainGameMode->OnReadyPlayState.AddDynamic(this, &AMenuPawn::ReadyPlayState);
	MainGameMode->OnExitState.AddDynamic(this, &AMenuPawn::ExitState);
}

void AMenuPawn::ScreenMotion(bool bNew)
{
	// Main menu camera offset depends on the mouse position.
	GetWorldTimerManager().SetTimer(ScreenMotionTimer, [this] {
		ScreenSizeX = GEngine->GameViewport->Viewport->GetSizeXY().X;
		ScreenSizeY = GEngine->GameViewport->Viewport->GetSizeXY().Y;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MouseX, MouseY);
		SetActorRotation(FRotator(-9.2 + (ScreenSizeY / 2 - MouseY) / ScreenSizeY, -115.599999, (ScreenSizeX / 2 - MouseX) / ScreenSizeX));
		}, 0.01f, true);
}

void AMenuPawn::ReadyPlayState(bool bNew)
{
	GetWorldTimerManager().ClearTimer(ScreenMotionTimer);
}

void AMenuPawn::ExitState()
{
	GetWorldTimerManager().ClearTimer(ScreenMotionTimer);
}