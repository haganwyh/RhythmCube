// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreStartingMenuLayoutBase.generated.h"

class UWidgetAnimation;
class AMainGameMode;
class UMainGameInstance;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UPreStartingMenuLayoutBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutLast = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HeadphoneFadeInOut = nullptr;

protected:
	// Begin play
	virtual void NativeOnInitialized() override;

private:
	// References
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	// References
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;
};
