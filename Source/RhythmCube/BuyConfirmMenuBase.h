// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "BuyConfirmMenuBase.generated.h"

class UButton;
class UTextBlock;
class AMainGameMode;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UBuyConfirmMenuBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> PriceText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BuyConfirmButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> NoMoneyText = nullptr;

	// Functions
	void UpdatePriceText(Theme UpdatingTheme);

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;

	// Button delegate functions
	UFUNCTION()
	void BuyConfirmButtonClicked();

	// Variables
	int ThemePrice;
	Theme TargetTheme;
};
