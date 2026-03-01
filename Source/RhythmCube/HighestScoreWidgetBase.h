// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HighestScoreWidgetBase.generated.h"

class AMainGameMode;
class UTextBlock;
class UImage;
class UMainGameInstance;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UHighestScoreWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> HighestScoreText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> GradeImage = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;

protected:
	//Begin play
	virtual void NativeOnInitialized() override;
	
private:
	// Exposed references
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UTexture2D>> GradeImagesBase;

	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;

	// Delegate functions
	UFUNCTION()
	void ReadyPlayState(bool bNew);
	UFUNCTION()
	void MainMenu(bool bNew);
	UFUNCTION()
	void Hide();
	UFUNCTION()
	void UpdateSongInfo(int SongID, FString SongName);
};
