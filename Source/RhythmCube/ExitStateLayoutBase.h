// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExitStateLayoutBase.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UExitStateLayoutBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Begin play
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInOut = nullptr;
};
