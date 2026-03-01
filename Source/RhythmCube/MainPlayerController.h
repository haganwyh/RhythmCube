// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class AMainGameMode;
class UInputAction;
class UInputMappingContext;
class UMainGameInstance;
class UEnhancedInputComponent;
class UEnhancedInputLocalPlayerSubsystem;
class APlayPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitCube, int, ID);

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Actions references
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement") // Hitting blue
	TObjectPtr<UInputAction> ActionBlueDown = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionBlueLeft = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionBlueRight = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionBlueUp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement") // Hitting red
	TObjectPtr<UInputAction> ActionRedDown = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionRedLeft = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionRedRight = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionRedUp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement") // Pause game
	TObjectPtr<UInputAction> ActionPause = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement") // Quit tutorial
	TObjectPtr<UInputAction> ActionQuitTutorial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement") // Input Mapping
	TObjectPtr<UInputMappingContext> InputMappingContent = nullptr;
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem;

	// Create delegates
	FHitCube OnHitCube;

	// Functions
	void HandlePause();

	// Variables
	bool bStartingAnimation;

protected:
	// Override functions
	virtual void BeginPlay() override;

private:
	// Exposed references
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> PlayStateActor = nullptr;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<APawn> PlayStatePawn = nullptr;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> MenuStateActor = nullptr;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<APawn> MenuStatePawn = nullptr;
	
	// Private references
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;

	// Delegate functions
	UFUNCTION()
	void ReadyPlayState(bool bNew);
	UFUNCTION()
	void GameOverState();
	UFUNCTION()
	void MainMenu(bool bNew);
	UFUNCTION()
	void PlayerDie();
	UFUNCTION()
	void ReadyMainMenu();
	UFUNCTION()
	void StartState();

	// Functions
	void BindAllActions();

	void HandleBlueDown();
	void HandleBlueLeft();
	void HandleBlueUp();
	void HandleBlueRight();

	void HandleRedDown();
	void HandleRedLeft();
	void HandleRedUp();
	void HandleRedRight();

	void HandleQuitTutorial();

	// Variables
	static constexpr float CamShakeScale = 0.05f;
	bool bIsPaused;
};
