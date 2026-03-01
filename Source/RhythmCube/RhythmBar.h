// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RhythmBar.generated.h"

class AMainGameMode;

UCLASS()
class RHYTHMCUBE_API ARhythmBar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARhythmBar();
	
	// Exposed references
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> SourceMaterials;

	void ApplyInitialSettings();
	int barID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Private references
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BarMaterialInstance = nullptr;
	FTimerHandle SelfDestroyTimer;
	float bpmTime;
	float LightOpacity;
	float deltaRotX;
};
