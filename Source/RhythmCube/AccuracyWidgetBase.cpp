// Fill out your copyright notice in the Description page of Project Settings.


#include "AccuracyWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"

void UAccuracyWidgetBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Bind delegates
	MainGameMode->OnHitResult.AddDynamic(this, &UAccuracyWidgetBase::HitResult);
}

void UAccuracyWidgetBase::HitResult(Accuracy Result, int ID)
{
	if (WidgetPosition == Position::Left && ID % 4 == 1
		|| WidgetPosition == Position::Right && ID % 4 == 3
		|| WidgetPosition == Position::Middle && ID % 4 == 2 || WidgetPosition == Position::Middle && ID % 4 == 0)
	{
		switch (Result)
		{
		case Accuracy::Perfect:
			PlayAnimation(Perfect);
			break;
		case Accuracy::Good:
			PlayAnimation(Good);
			break;
		case Accuracy::Early:
			PlayAnimation(Early);
			break;
		case Accuracy::Miss:
			PlayAnimation(Miss);
			break;
		default:;
		}
	}
}