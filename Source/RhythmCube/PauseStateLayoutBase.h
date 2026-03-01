// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseStateLayoutBase.generated.h"

class UImage;
class UButton;
class AMainGameMode;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UPauseStateLayoutBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// References
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> HomeButton = nullptr;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> MenuFadeOut = nullptr;

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void Resume();
	UFUNCTION()
	void MainMenu();
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
};
