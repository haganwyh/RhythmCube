// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class AMainGameMode;
class UMainGameInstance;
class UPlayStateLayoutBase;
class UGameOverStateLayoutBase;
class UPauseStateLayoutBase;
class APausedTimerManagerTicker;
class UMainMenuLayoutBase;
class UExitStateLayoutBase;
class UGameOverHardcoreLayoutBase;
class USettingStateLayoutBase;
class UStoreStateLayoutBase;
class UStartingMenuLayoutBase;
class UPreStartingMenuLayoutBase;
class UTutorialPauseStateLayoutBase;
class UTutorialLayoutBase;

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	// References
	UPROPERTY()
	TObjectPtr<UPlayStateLayoutBase> PlayStateLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UGameOverStateLayoutBase> GameOverStateLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UPauseStateLayoutBase> PauseStateLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UMainMenuLayoutBase> MainMenuLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UExitStateLayoutBase> ExitStateLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UGameOverHardcoreLayoutBase> GameOverHardcoreLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<USettingStateLayoutBase> SettingStateLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UStoreStateLayoutBase> StoreStateLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UStartingMenuLayoutBase> StartingMenuLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UPreStartingMenuLayoutBase> PreStartingMenuLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UTutorialPauseStateLayoutBase> TutorialPauseStateLayoutWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UTutorialLayoutBase> TutorialLayoutWidget = nullptr;

	// Functions
	void UnPauseCountDown();

protected:
	// Override functions
	virtual void BeginPlay() override;

private:
	// Private references
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;

	// Exposed references
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayStateLayoutBase> PlayStateLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameOverStateLayoutBase> GameOverStateLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseStateLayoutBase> PauseStateLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuLayoutBase> MainMenuLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UExitStateLayoutBase> ExitStateLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameOverHardcoreLayoutBase> GameOverHardcoreLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USettingStateLayoutBase> SettingStateLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UStoreStateLayoutBase> StoreStateLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UStartingMenuLayoutBase> StartingMenuLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPreStartingMenuLayoutBase> PreStartingMenuLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTutorialPauseStateLayoutBase> TutorialPauseStateLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTutorialLayoutBase> TutorialLayoutClass = nullptr;

	// Delegate Functions
	UFUNCTION()
	void PlayStart();
	UFUNCTION()
	void GameOverState();
	UFUNCTION()
	void ReadyPlayState(bool bNew);
	UFUNCTION()
	void PauseState();
	UFUNCTION()
	void MainMenu(bool bNew);
	UFUNCTION()
	void ExitState();
	UFUNCTION()
	void PlayerDie();
	UFUNCTION()
	void SettingState();
	UFUNCTION()
	void StoreState();
	UFUNCTION()
	void StartState();
};
