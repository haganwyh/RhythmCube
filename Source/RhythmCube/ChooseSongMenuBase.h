// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChooseSongMenuBase.generated.h"

class UWidgetAnimation;
class UButton;
class UTextBlock;
class UImage;
class UMainGameInstance;
class AMainGameMode;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UChooseSongMenuBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> LeftButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> RightButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SongNameText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> SongCoverImage = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ArtistText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> IllustrationText = nullptr;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LeftButtonHover = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LeftButtonUnHover = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RightButtonHover = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RightButtonUnHover = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	// Button delegate functions
	UFUNCTION()
	void LeftButtonHovered();
	UFUNCTION()
	void LeftButtonUnHovered();
	UFUNCTION()
	void RightButtonHovered();
	UFUNCTION()
	void RightButtonUnHovered();
	UFUNCTION()
	void LeftButtonClicked();
	UFUNCTION()
	void RightButtonClicked();

	// Exposed references
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UTexture2D>> SongCoverTextures;

	// References
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;

	// Delegate functions
	UFUNCTION()
	void Hide();
	UFUNCTION()
	void MainMenu(bool bNew);
	UFUNCTION()
	void UpdateSelectedSong(int ID, FString SongName);
};
