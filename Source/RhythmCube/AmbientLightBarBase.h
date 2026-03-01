// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmbientLightBarBase.generated.h"

class AMainGameMode;

UCLASS()
class RHYTHMCUBE_API AAmbientLightBarBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmbientLightBarBase();

	// Functions
	void LightBarOn(float Duration);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Exposed references
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> SourceMaterials;

	// Private references
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> BarMaterialInstances;
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;

	// Timers
	FTimerHandle LightBarOnTimer;
	FTimerHandle LightBarStayTimer;
	FTimerHandle LightBarOffTimer;

	// Variables
	float Opacity;
	float Duration;

	// Functions
	void SetOpacity();
	UFUNCTION()
	void PauseState();
	UFUNCTION()
	void UnPauseState();
	UFUNCTION()
	void MainMenuState(bool bNew);
};
