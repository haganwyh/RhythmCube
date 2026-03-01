// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayPawn.h"
#include "Kismet/GameplayStatics.h"

void APlayPawn::CameraShake(float Scale)
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(CamShake, Scale);
}