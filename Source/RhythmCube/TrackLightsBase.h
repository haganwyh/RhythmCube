// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainGameInstance.h"
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "TrackLightsBase.generated.h"

class AMainGameMode;
class UMainGameInstance;
class UTimelineComponent;

UCLASS()
class RHYTHMCUBE_API ATrackLightsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrackLightsBase();

	void EnhanceTrackLights(int Mode);
	void UpdateColor(FLinearColor Color);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Private references
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TArray<TObjectPtr<USpotLightComponent>> TrackLights;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;
	UPROPERTY()
	TObjectPtr<UTimelineComponent> ColorTimeline = nullptr;

	// Exposed references
	UPROPERTY(EditAnywhere)
	int TrackID;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> ColorCurve;

	// Timers
	FTimerHandle TrackLightsTimerOn;
	FTimerHandle TrackLightsTimerOff;

	// Variables
	float LightIntensity;
	const float MAX_LIGHT_INTENSITY = 10000.f;
	const float	MIN_LIGHT_INTENSITY = 3000.f;
	const float LIGHT_INTENSITY_DIFFERENCE = MAX_LIGHT_INTENSITY - MIN_LIGHT_INTENSITY;
	FLinearColor StartColor;
	FLinearColor TargetColor;

	// Functions
	void UpdateIntensity(float Intensity);
	UFUNCTION()
	void OnColorTimelineUpdate(float Value);
	void UpdateColorWithAnimation(const FLinearColor& NewTarget, float Rate);
};
