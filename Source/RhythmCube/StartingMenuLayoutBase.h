// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartingMenuLayoutBase.generated.h"

class UWidgetAnimation;
class UButton;
class AMainGameMode;
class AStartingPawn;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UStartingMenuLayoutBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> TapButton = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> TapTextLoop = nullptr;

protected:
	// Begin play
	virtual void NativeOnInitialized() override;

private:
	// Exposed references
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AStartingPawn> StartingPawn = nullptr;

	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;

	// Button delegate function
	UFUNCTION()
	void TapButtonClicked();

	// Variables
	FTimerHandle MusicBeatTimer;
	FTimerHandle TapTextLoopTimer;
};
