// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialPauseStateLayoutBase.generated.h"

class UWidgetAnimation;
class AMainGameMode;
class UMainGameInstance;
class UTextBlock;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UTutorialPauseStateLayoutBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuitText = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ContinueTextLoop = nullptr;

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;

	// Delegate functions
	UFUNCTION()
	void Hide();
	
	// Variables
	FTimerHandle ContinueTextLoopTimer;
};
