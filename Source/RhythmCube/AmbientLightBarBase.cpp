// Fill out your copyright notice in the Description page of Project Settings.


#include "AmbientLightBarBase.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAmbientLightBarBase::AAmbientLightBarBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAmbientLightBarBase::LightBarOn(float Dur)
{
	Opacity = 0.f;
	Duration = FMath::Abs(Dur);
	GetWorldTimerManager().PauseTimer(LightBarStayTimer);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	GetWorldTimerManager().SetTimer(LightBarOnTimer, [this] {
		if (Opacity < 1.f) {
			if (Opacity < 0.2f) {
				Opacity += 0.2f / 30.f; // Slow begin
			}
			else {
				Opacity += 0.8f / 20.f; // Fast end
			}
			SetOpacity();
		}
		else {
			GetWorldTimerManager().ClearTimer(LightBarOnTimer);
		}
	}, 0.01f, true);
	GetWorldTimerManager().SetTimer(LightBarStayTimer, [this] {
		GetWorldTimerManager().ClearTimer(LightBarOnTimer);
		Opacity = 1.f;
		SetOpacity();
		GetWorldTimerManager().SetTimer(LightBarOffTimer, [this] {
			if (Opacity > 0.f) {
				if (Opacity > 0.2f) {
					Opacity -= 0.8f / 20.f; // Fast begin
				}
				else {
					Opacity -= 0.2f / 30.f; // Slow end
				}
				SetOpacity();
			}
			else {
				GetWorldTimerManager().ClearTimer(LightBarOffTimer);
			}
			}, 0.01f, true);
	}, Duration, false);
}

// Called when the game starts or when spawned
void AAmbientLightBarBase::BeginPlay()
{
	Super::BeginPlay();
	// Initialise variables
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Apply dynamic material instances & Set HitDisplayWidget
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);
	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		BarMaterialInstances.Add(PrimitiveComponent->CreateDynamicMaterialInstance(0, SourceMaterials[0]));
	}

	// Bind delegates
	MainGameMode->OnPauseState.AddDynamic(this, &AAmbientLightBarBase::PauseState);
	MainGameMode->OnUnPauseState.AddDynamic(this, &AAmbientLightBarBase::UnPauseState);
	MainGameMode->OnMainMenu.AddDynamic(this, &AAmbientLightBarBase::MainMenuState);
}

void AAmbientLightBarBase::SetOpacity()
{
	for (UMaterialInstanceDynamic* MaterialInstance : BarMaterialInstances) {
		MaterialInstance->SetScalarParameterValue(FName("Opacity"), Opacity);
	}
}

void AAmbientLightBarBase::PauseState()
{
	GetWorldTimerManager().PauseTimer(LightBarOnTimer);
	GetWorldTimerManager().PauseTimer(LightBarStayTimer);
	GetWorldTimerManager().PauseTimer(LightBarOffTimer);
	
}

void AAmbientLightBarBase::UnPauseState()
{
	GetWorldTimerManager().UnPauseTimer(LightBarOnTimer);
	GetWorldTimerManager().UnPauseTimer(LightBarStayTimer);
	GetWorldTimerManager().UnPauseTimer(LightBarOffTimer);
	
}

void AAmbientLightBarBase::MainMenuState(bool bNew)
{
	UnPauseState();
}
