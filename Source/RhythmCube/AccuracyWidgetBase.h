// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AccuracyWidgetBase.generated.h"

class UTextBlock;
class UWidgetAnimation;
class AMainGameMode;

UENUM()
enum class Position : int32
{
	Left,
	Middle,
	Right
};

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UAccuracyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> PerfectText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> GoodText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> EarlyText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MissText = nullptr;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Perfect = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Good = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Early = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Miss = nullptr;

protected:
	//Begin play
	virtual void NativeOnInitialized() override;

private:
	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;

	// Delegate functions
	UFUNCTION()
	void HitResult(Accuracy Result, int ID);

	// Exposed variables
	UPROPERTY(EditAnywhere)
	Position WidgetPosition;
};
