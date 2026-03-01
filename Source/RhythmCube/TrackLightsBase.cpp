// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackLightsBase.h"
#include "Components/TimelineComponent.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrackLightsBase::ATrackLightsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create timeline
	ColorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}

void ATrackLightsBase::EnhanceTrackLights(int Mode)
{
	UpdateColorWithAnimation(MainGameMode->ThemeColors[static_cast<int32>(MainGameInstance->UsingTheme)][Mode ? 1 - TrackID : TrackID], 1.f);
	LightIntensity = TrackLights[0]->Intensity;
	UpdateIntensity(LightIntensity);
	GetWorldTimerManager().ClearTimer(TrackLightsTimerOn);
	GetWorldTimerManager().ClearTimer(TrackLightsTimerOff);
	GetWorldTimerManager().SetTimer(TrackLightsTimerOn, [this] {
		// Increase intensity
		if (LightIntensity < MAX_LIGHT_INTENSITY) {
			LightIntensity += LIGHT_INTENSITY_DIFFERENCE / 60.f;
			UpdateIntensity(LightIntensity);
		}
		else {
			GetWorldTimerManager().ClearTimer(TrackLightsTimerOn);
			GetWorldTimerManager().SetTimer(TrackLightsTimerOff, [this] {
				// Decrease intensity
				if (LightIntensity > MIN_LIGHT_INTENSITY) {
					LightIntensity -= LIGHT_INTENSITY_DIFFERENCE / 60.f;
					UpdateIntensity(LightIntensity);
				}
				else {
					UpdateColorWithAnimation(MainGameMode->ThemeColors[static_cast<int32>(MainGameInstance->UsingTheme)][TrackID], 1.f);
					GetWorldTimerManager().ClearTimer(TrackLightsTimerOff);
				}
				}, MainGameMode->BpmTime / 60.f * 0.5f, true);
		}
		}, MainGameMode->BpmTime / 60.f * 0.2f, true);
}

void ATrackLightsBase::UpdateColor(FLinearColor Color)
{
	for (USpotLightComponent* Light : TrackLights) {
		Light->SetLightColor(Color);
	}
}

void ATrackLightsBase::UpdateIntensity(float Intensity)
{
	for (USpotLightComponent* Light : TrackLights) {
		Light->SetIntensity(Intensity);
	}
}

// Called when the game starts or when spawned
void ATrackLightsBase::BeginPlay()
{
	Super::BeginPlay();
	// Initialise variables
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind components
	TArray<USpotLightComponent*> SpotComponents;
	GetComponents<USpotLightComponent>(SpotComponents);
	for (USpotLightComponent* Spot : SpotComponents) {
		TrackLights.Add(Spot);
	}
	
	// Bind timeline
	FOnTimelineFloat MovementValue;
	MovementValue.BindUFunction(this, FName("OnColorTimelineUpdate"));
	if (ColorCurve) // Validate if movement curve provided properly
	{
		ColorTimeline->AddInterpFloat(ColorCurve, MovementValue);
	}
}

void ATrackLightsBase::UpdateColorWithAnimation(const FLinearColor& NewTarget, float Rate)
{
	StartColor = TrackLights[0]->GetLightColor();
	TargetColor = NewTarget;

	ColorTimeline->SetPlayRate(Rate);
	ColorTimeline->PlayFromStart();
}

void ATrackLightsBase::OnColorTimelineUpdate(float Value)
{
	FLinearColor NewColor = FMath::Lerp(StartColor, TargetColor, Value);
	for (USpotLightComponent* Light : TrackLights) {
		Light->SetLightColor(NewColor);
	}
}
