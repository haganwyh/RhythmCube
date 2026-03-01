// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuLayoutBase.generated.h"

class UButton;
class UTextBlock;
class AMainGameMode;
class UMainGameInstance;
class AMainPlayerController;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UMainMenuLayoutBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Widget references
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> PlayButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ExitButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> SettingButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> StoreButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> WarnText = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PlayButtonHover = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PlayButtonUnHover = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutLimited = nullptr;

	// Functions
	void RemoveWithAnimation();

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	// Button delegate functions
	UFUNCTION()
	void PlayButtonHovered();
	UFUNCTION()
	void PlayButtonUnHovered();
	UFUNCTION()
	void PlayButtonClicked();
	UFUNCTION()
	void ExitButtonClicked();
	UFUNCTION()
	void SettingButtonClicked();
	UFUNCTION()
	void StoreButtonClicked();

	// Delegate functions
	UFUNCTION()
	void CheckSongDisable(int SongID, FString SongName);

	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;
	UPROPERTY()
	TObjectPtr<AMainPlayerController> MainPlayerController = nullptr;
};
