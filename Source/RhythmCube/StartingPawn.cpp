// Fill out your copyright notice in the Description page of Project Settings.


#include "StartingPawn.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"

void AStartingPawn::BeginPlay()
{
	Super::BeginPlay();

	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Bind delegates
	MainGameMode->OnStartState.AddDynamic(this, &AStartingPawn::ScreenMotion);
	MainGameMode->OnReadyMainMenu.AddDynamic(this, &AStartingPawn::ReadyMainMenu);
}

void AStartingPawn::ScreenMotion()
{
	// Main menu camera offset depends on the mouse position.
	GetWorldTimerManager().SetTimer(ScreenMotionTimer, [this] {
		ScreenSizeX = GEngine->GameViewport->Viewport->GetSizeXY().X;
		ScreenSizeY = GEngine->GameViewport->Viewport->GetSizeXY().Y;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MouseX, MouseY);
		SetActorRotation(FRotator(-23.2 + (ScreenSizeY / 2 - MouseY) * 2.f / ScreenSizeY, -108.199999, (ScreenSizeX / 2 - MouseX) * 2.f / ScreenSizeX)); // Y, Z, X
		}, 0.01f, true);
}

void AStartingPawn::CameraShake(float Scale)
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(CamShake, Scale);
}

void AStartingPawn::ReadyMainMenu()
{
	GetWorldTimerManager().ClearTimer(ScreenMotionTimer);
}