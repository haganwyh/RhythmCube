// Fill out your copyright notice in the Description page of Project Settings.


#include "PausedTimerManagerTicker.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "GameSystem.h"

// Sets default values
APausedTimerManagerTicker::APausedTimerManagerTicker()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

// Called when the game starts or when spawned
void APausedTimerManagerTicker::BeginPlay()
{
	Super::BeginPlay();

	// Set up real timer
	UGameInstance* GameInstance = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	UGameSystem* GameSystem = GameInstance->GetSubsystem<UGameSystem>();
	GameSystem->RealTimeActor = this;
}

// Called every frame
void APausedTimerManagerTicker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld() && GetWorld()->IsPaused())
	{
		GetWorld()->GetTimerManager().Tick(DeltaTime);
	}
}