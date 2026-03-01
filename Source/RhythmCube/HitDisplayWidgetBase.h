// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitDisplayWidgetBase.generated.h"

class UWidgetAnimation;
class UImage;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UHitDisplayWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Widget references
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> HitArrow = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutFail = nullptr;
};
