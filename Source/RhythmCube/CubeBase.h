// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeBase.generated.h"

class AMainGameMode;
class AMainPlayerController;
class UTimelineComponent;
class APlayPawn;
class ARhythmBar;
class UMainGameInstance;
class UWidgetComponent;
class UHitDisplayWidgetBase;

UENUM()
enum class CubeEffectType : int32
{
	Dissolve,
	Fade
};

UCLASS()
class RHYTHMCUBE_API ACubeBase : public AActor
{
	ACubeBase();
	GENERATED_BODY()

public:
	// Override functions
	virtual void Tick(float DeltaTime) override;

	// Variables
	int CubeID;

	// Functions
	UFUNCTION()
	void OnHitCube(int ID);
	void ApplyInitialSettings();

protected:
	virtual void BeginPlay() override;

private:
	// Exposed references
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> SourceMaterials;
	UPROPERTY(EditAnywhere, Category = "Timeline Actor|Curves")
	TObjectPtr<UCurveFloat> SelfDestroyCurve = nullptr;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<APlayPawn>PlayPawn = nullptr;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ActorsToSpawn;

	// Private references
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<AMainPlayerController> MainPlayerController = nullptr;
	UPROPERTY()
	TObjectPtr<UTimelineComponent> SelfDestroyTimeline = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;
	UPROPERTY()
	TObjectPtr<UHitDisplayWidgetBase> HitDisplayWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UWidgetComponent> HitDisplayWidgetComponent = nullptr;
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> CubeMaterialInstances;

	// Timeline functions
	UFUNCTION()
	void TimelineProgress(float Value);
	UFUNCTION()
	void TimelineFinished();

	// Delegate functions
	UFUNCTION()
	void ForceDestroyCubes(bool bNew);

	// Functions
	void AutoHit();
	void ConstructWithAnimation(float Rate, CubeEffectType Type);
	void DestroyWithAnimation(float Rate, CubeEffectType Type);
	int GenerateRhythmBar(bool bFollowCubeColor);

	// Variables
	float CubeSpawnX;
	float CubeEndX;
	float BpmTime;
	float Deviation;
	float DelayAmount;
	float SlowDownFactor;
	bool bIsHit;
	bool bIsDisplay;
	CubeEffectType CubeEffect;
};
