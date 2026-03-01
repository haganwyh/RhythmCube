// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmBar.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARhythmBar::ARhythmBar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARhythmBar::BeginPlay()
{
	Super::BeginPlay();
	// Initialise variables
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	bpmTime = MainGameMode->BpmTime;

	// Start self destroy
	LightOpacity = 0.5f;
	if (GetActorLocation().Y < 0) {
		deltaRotX = 10.f / 60;
	}
	else {
		deltaRotX = -10.f / 60;
	}
	GetWorldTimerManager().SetTimer(SelfDestroyTimer, [this] {
		if (LightOpacity <= 0.f) {
			GetWorldTimerManager().ClearTimer(SelfDestroyTimer);
			Destroy();
		}
		else {
			LightOpacity -= 0.5f / 60.f;
			BarMaterialInstance->SetScalarParameterValue(FName("Opacity"), LightOpacity);
			SetActorRotation(FRotator(0.f, 0.f, GetActorRotation().Roll + deltaRotX));
		}
		}, bpmTime/60, true);
}

void ARhythmBar::ApplyInitialSettings() {
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);
	if (PrimitiveComponents[0]->GetName().Contains("Bar")) {
		BarMaterialInstance = PrimitiveComponents[0]->CreateDynamicMaterialInstance(0, SourceMaterials[barID]);
	}
}

