// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialLayoutBase.generated.h"

class UWidgetAnimation;
class UTextBlock;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UTutorialLayoutBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TutorialText = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> TextFadeOutIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> EndTextFadeInOut = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial1In = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial4In = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial7In = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial8In = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial9In = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial10In = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial13In = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial14In = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial15In = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial16In = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial1Out = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial4Out = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial7Out = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial8Out = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial9Out = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial10Out = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial13Out = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial14Out = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial15Out = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial16Out = nullptr;

	// Functions
	void UpdateTutorialText();
	void EndTutorialText();

protected:
	// Override functions
	virtual void NativeOnInitialized() override;

private:
	// Variables
	int TutorialCount;

	TArray<FString> TutorialTextList =
	{
		// Introduction (0-5)
		"Welcome to Rhythm Cube!\nLet's begin the tutorial",
		"The score is at the top right corner",
		"Song information and difficulty are shown at the bottom",
		"You can pause the game by pressing ESC key",
		"The horizontal red line in front of you is the judgement line.\nCubes are going to approach towards the judgement line.",
		"To get the highest score, hit the cubes when they are close to the judgement line.",
		// Blue cubes (6-11)
		"First of all, BLUE cubes",
		"Hit them before they pass the judgement line.\nTo hit BLUE DOWN cube, press \"S\"",
		"To hit BLUE LEFT cube, press \"A\"",
		"To hit BLUE RIGHT cube, press \"D\"",
		"To hit BLUE TOP cube, press \"W\"",
		"",
		// Red cubes (12-17)
		"Next up, RED cubes",
		"Hit them before they pass the judgement line.\nTo hit RED DOWN cube, press \"K\"",
		"To hit RED LEFT cube, press \"J\"",
		"To hit RED RIGHT cube, press \"L\"",
		"To hit RED TOP cube, press \"I\"",
		"",
	};
};
