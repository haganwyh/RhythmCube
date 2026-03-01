// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverHardcoreLayoutBase.generated.h"

class UButton;
class AMainGameMode;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UGameOverHardcoreLayoutBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// References
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> RetryButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> HomeButton = nullptr;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;

	// Button delegate functions
	UFUNCTION()
	void Retry();
	UFUNCTION()
	void MainMenu();
};
