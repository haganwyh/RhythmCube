// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SongCoverBase.generated.h"

class AMainGameMode;
class UMainGameInstance;
class UStaticMeshComponent;
class UMaterialInterface;
class UMainGameInstance;

UCLASS()
class RHYTHMCUBE_API ASongCoverBase : public AActor
{
	GENERATED_BODY()

protected:
	// Override functions
	virtual void BeginPlay() override;

private:
	// Exposed references
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> SongCoverMaterials;

	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;

	// Delegate functions
	UFUNCTION()
	void UpdateSongName(int ID, FString SongName);
	UFUNCTION()
	void Hide();
	UFUNCTION()
	void MainMenu(bool bNew);
};
