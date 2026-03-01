// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeBase.h"
#include "RhythmBar.h"
#include "MainGameMode.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"
#include "PlayPawn.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "HitDisplayWidgetBase.h"

//// Initialise ////
ACubeBase::ACubeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create timeline
	SelfDestroyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}

void ACubeBase::BeginPlay()
{
	Super::BeginPlay();

	// Initialise variables
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CubeSpawnX = MainGameMode->CubeSpawnX;
	CubeEndX = MainGameMode->CubeEndX;
	BpmTime = MainGameMode->BpmTime;
	Deviation = MainGameMode->Deviation;
	DelayAmount = 0.f;
	SlowDownFactor = 0.f;
	bIsHit = false;
	bIsDisplay = false;

	// Bind delegate
	MainGameMode->OnMainMenu.AddDynamic(this, &ACubeBase::ForceDestroyCubes);

	// Bind timeline
	FOnTimelineFloat MovementValue;
	FOnTimelineEvent TimelineFinishedEvent;
	MovementValue.BindUFunction(this, FName("TimelineProgress"));
	TimelineFinishedEvent.BindUFunction(this, FName("TimelineFinished"));
	if (SelfDestroyCurve) // Validate if movement curve provided properly
	{
		SelfDestroyTimeline->AddInterpFloat(SelfDestroyCurve, MovementValue);
	}
	SelfDestroyTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);

	// Construct animation
	ConstructWithAnimation(0.5f, CubeEffectType::Dissolve);
}

//// Main loop ////
void ACubeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Detect if the cube enter accept range
	if ((GetGameTimeSinceCreation() + MainGameInstance->Offset) > BpmTime * MainGameMode->PreBeat - Deviation * 1.5f && !bIsDisplay && MainGameInstance->bHitAssist)
	{
		if (HitDisplayWidget)
		{
			if (!bIsHit)
			{
				bIsDisplay = true;
				HitDisplayWidget->PlayAnimation(HitDisplayWidget->FadeIn);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fail to reference HitDisplayWidget!"));
		}
	}

	AutoHit(); // Just for testing, auto hit all the cubes !AUTO HIT HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
	if (MainGameMode->Health <= 0)
	{
		bIsHit = true;

		// Player die
		if (SlowDownFactor != 1.f)
		{
			// Add slow down factor each frame, until reach 1
			SlowDownFactor += DeltaTime * 0.5f;
			if (SlowDownFactor >= 1.f)
			{
				SlowDownFactor = 1.f;
				FTimerHandle SelfDestroyTimer;
				GetWorldTimerManager().SetTimer(SelfDestroyTimer, [this] {
					if (bIsDisplay && MainGameInstance->bHitAssist)
					{
						HitDisplayWidget->StopAnimation(HitDisplayWidget->FadeIn);
						HitDisplayWidget->PlayAnimation(HitDisplayWidget->FadeOutFail);
					}
					DestroyWithAnimation(0.25f, CubeEffectType::Dissolve);
					}, 1.f, false);
			}
		}
		// SlowDownFactor:0 = Normal | SlowDownFactor:1 = Stop
		DelayAmount += DeltaTime * SlowDownFactor;
	}
	else if ((GetGameTimeSinceCreation() + MainGameInstance->Offset) > BpmTime * MainGameMode->PreBeat + MainGameMode->Deviation && !bIsHit)
	{
		bIsHit = true;

		// Self destroy when pass through border line
		UE_LOG(LogTemp, Warning, TEXT("Miss!"));
		MainGameMode->DeregisterCube(this, CubeID);
		MainGameMode->ResetCombo();
		MainGameMode->Hurt();
		MainGameMode->OnHitResult.Broadcast(Accuracy::Miss, CubeID);
		if (MainGameInstance->bHitAssist)
		{
			HitDisplayWidget->StopAnimation(HitDisplayWidget->FadeIn);
			HitDisplayWidget->PlayAnimation(HitDisplayWidget->FadeOutFail);
		}
		DestroyWithAnimation(1.f, CubeEffectType::Dissolve);
	}
	// Move toward player
	SetActorLocation(FVector(CubeSpawnX - (CubeSpawnX - CubeEndX) * ((GetGameTimeSinceCreation() - DelayAmount + MainGameInstance->Offset) / (BpmTime * MainGameMode->PreBeat)),
		GetActorLocation().Y, GetActorLocation().Z));
}

//// Functions ////
void ACubeBase::OnHitCube(int ID)
{
	// Confirm the data exists, and continue if the instance is the first front cube
	// Also ensure the cube won't be too far when being hit
	if (MainGameMode->GeneratedCubes[ID % 4].Num() > 0 && MainGameMode->GeneratedCubes[ID % 4][0] == this && !bIsHit
		&& GetGameTimeSinceCreation() + MainGameInstance->Offset > BpmTime * MainGameMode->PreBeat - Deviation * 1.0f) // <- This is the maximum distance for player to hit cube, out of this distance won't response to hit
	{
		// Hit me
		bIsHit = true;
		if ((GetGameTimeSinceCreation() + MainGameInstance->Offset) > BpmTime * MainGameMode->PreBeat - Deviation && ID == CubeID)
		{
			// Hit me with correct 
			if (CubeID < 4)
			{
				// Hit sound of blue cube
				MainGameMode->PlaySoundEffects(SoundEffects::Blue);
			}
			else
			{
				// Hit sound of red cube
				MainGameMode->PlaySoundEffects(SoundEffects::Red);
			}

			// Camera shaking effect
			if (MainGameInstance->bCameraShake)
			{
				PlayPawn->CameraShake(0.05f);
			}

			// Track lights & Rhythm bar effect
			int GenTrackID;
			if (GenerateRhythmBar(true) == 0) GenTrackID = 0; else GenTrackID = 1;
			MainGameMode->ShowTrackLights(GenTrackID, (GenTrackID != (CubeID >= 4))); // Track light match = 0, not match = 1

			UE_LOG(LogTemp, Warning, TEXT("Perfect! Hit time: %f"), (GetGameTimeSinceCreation() + MainGameInstance->Offset) - BpmTime * MainGameMode->PreBeat);
			MainGameMode->HitSuccessCount++;
			if ((GetGameTimeSinceCreation() + MainGameInstance->Offset) > BpmTime * MainGameMode->PreBeat - MainGameMode->PerfectDeviation
				&& (GetGameTimeSinceCreation() + MainGameInstance->Offset) < BpmTime * MainGameMode->PreBeat + MainGameMode->PerfectDeviation)
			{
				// Perfect hit
				MainGameMode->OnHitResult.Broadcast(Accuracy::Perfect, CubeID);
				MainGameMode->GainScore(Accuracy::Perfect);
			}
			else
			{
				// Good hit
				MainGameMode->OnHitResult.Broadcast(Accuracy::Good, CubeID);
				MainGameMode->GainScore(Accuracy::Good);
			}
			if (MainGameInstance->bHitAssist)
			{
				HitDisplayWidget->StopAnimation(HitDisplayWidget->FadeIn);
				HitDisplayWidget->PlayAnimation(HitDisplayWidget->FadeOut);
			}
			DestroyWithAnimation(1.f, CubeEffectType::Dissolve);
		}
		else
		{
			// Hit me with wrong key or too early
			UE_LOG(LogTemp, Warning, TEXT("Bad!"));
			MainGameMode->ResetCombo();
			MainGameMode->Hurt();
			if (ID == CubeID)
			{
				MainGameMode->OnHitResult.Broadcast(Accuracy::Early, CubeID);
			}
			else
			{
				MainGameMode->OnHitResult.Broadcast(Accuracy::Miss, CubeID);
			}
			if (MainGameInstance->bHitAssist)
			{
				HitDisplayWidget->StopAnimation(HitDisplayWidget->FadeIn);
				HitDisplayWidget->PlayAnimation(HitDisplayWidget->FadeOutFail);
			}
			DestroyWithAnimation(1.f, CubeEffectType::Fade);
		}
		MainGameMode->DeregisterCube(this, CubeID);
	}
}

void ACubeBase::TimelineProgress(float Value)
{
	// Increase dissolve effects on dynamic material
	if (CubeEffect == CubeEffectType::Dissolve)
	{
		for (int i = 0; i < CubeMaterialInstances.Num(); i++)
		{
			CubeMaterialInstances[i]->SetScalarParameterValue(FName("Dissolve"), Value); // 0.6 = No dissolve effect | -0.4 = Full dissolve effect
		}
	}
	else
	{
		for (int i = 0; i < CubeMaterialInstances.Num(); i++)
		{
			CubeMaterialInstances[i]->SetScalarParameterValue(FName("Opacity"), Value + 0.4f); // 1.0 = No fade effect | 0.0 = Full fade effect
		}
	}
}

void ACubeBase::TimelineFinished()
{
	// Animation ended, self destroy
	if (SelfDestroyTimeline->GetPlaybackPosition() > 0.f)
	{
		Destroy();
	}
}

void ACubeBase::ForceDestroyCubes(bool bNew)
{
	bIsHit = true; // Avoid "MISS" show
	if (bIsDisplay && MainGameInstance->bHitAssist)
	{
		HitDisplayWidget->StopAnimation(HitDisplayWidget->FadeIn);
		HitDisplayWidget->PlayAnimation(HitDisplayWidget->FadeOut);
	}
	DestroyWithAnimation(1.f, CubeEffectType::Dissolve);
}

void ACubeBase::DestroyWithAnimation(float Rate, CubeEffectType Type)
{
	CubeEffect = Type;
	SelfDestroyTimeline->SetPlayRate(Rate);
	SelfDestroyTimeline->PlayFromStart(); // Destroy animation
}

void ACubeBase::ConstructWithAnimation(float Rate, CubeEffectType Type)
{
	CubeEffect = Type;
	SelfDestroyTimeline->SetPlayRate(Rate);
	SelfDestroyTimeline->ReverseFromEnd(); // Create animation
}

// Just for testing, auto hit all the cubes
void ACubeBase::AutoHit()
{
	if ((GetGameTimeSinceCreation() + MainGameInstance->Offset) > BpmTime * MainGameMode->PreBeat && !bIsHit)
	{
		bIsHit = true;
		// Hit me with correct 
		if (CubeID < 4)
		{
			// Hit sound of blue cube
			MainGameMode->PlaySoundEffects(SoundEffects::Blue);
		}
		else
		{
			// Hit sound of red cube
			MainGameMode->PlaySoundEffects(SoundEffects::Red);
		}

		// Camera shaking effect
		if (MainGameInstance->bCameraShake)
		{
			PlayPawn->CameraShake(0.05f);
		}

		// Track lights & Rhythm bar effect
		int GenTrackID;
		if (GenerateRhythmBar(true) == 0) GenTrackID = 0; else GenTrackID = 1;
		MainGameMode->ShowTrackLights(GenTrackID, (GenTrackID != (CubeID >= 4))); // Track light match = 0, not match = 1

		UE_LOG(LogTemp, Warning, TEXT("Perfect! Hit time: %f"), (GetGameTimeSinceCreation() + MainGameInstance->Offset) - BpmTime * MainGameMode->PreBeat);
		MainGameMode->GainScore(Accuracy::Perfect);
		MainGameMode->HitSuccessCount++;
		MainGameMode->OnHitResult.Broadcast(Accuracy::Perfect, CubeID);
		MainGameMode->DeregisterCube(this, CubeID);
		DestroyWithAnimation(1.f, CubeEffectType::Dissolve);
		if (MainGameInstance->bHitAssist)
		{
			HitDisplayWidget->PlayAnimation(HitDisplayWidget->FadeOut);
		}
	}
}

void ACubeBase::ApplyInitialSettings()
{
	// Apply dynamic material instances & Set HitDisplayWidget
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);
	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		if (PrimitiveComponent->GetName().Contains("Cube"))
		{
			CubeMaterialInstances.Add(PrimitiveComponent->CreateDynamicMaterialInstance(0, SourceMaterials[0]));
			CubeMaterialInstances.Last()->SetScalarParameterValue(FName("Dissolve"), 0.6f); // 0.6 = No dissolve effect | -0.4 = Full dissolve effect
			if (!MainGameInstance->bTransparentCube)
			{
				// Solid
				CubeMaterialInstances.Last()->SetScalarParameterValue(FName("DefaultOpacity"), 1.f);
			}
		}
		else if (PrimitiveComponent->GetName().Contains("Border"))
		{
			CubeMaterialInstances.Add(PrimitiveComponent->CreateDynamicMaterialInstance(0, SourceMaterials[1]));
			CubeMaterialInstances.Last()->SetScalarParameterValue(FName("Dissolve"), 0.6f); // 0.6 = No dissolve effect | -0.4 = Full dissolve effect
		}
		else if (PrimitiveComponent->GetName().Contains("HitDisplay"))
		{
			HitDisplayWidgetComponent = Cast<UWidgetComponent>(PrimitiveComponent);
			HitDisplayWidget = Cast<UHitDisplayWidgetBase>(HitDisplayWidgetComponent->GetWidget());

			switch (CubeID % 4)
			{
			case 0:
				HitDisplayWidgetComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
				break;
			case 1:
				HitDisplayWidgetComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
				break;
			case 2:
				HitDisplayWidgetComponent->SetRelativeRotation(FRotator(0.f, 0.f, 180.f));
				break;
			case 3:
				HitDisplayWidgetComponent->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
				break;
			default:;
			}
		}
	}
}

int ACubeBase::GenerateRhythmBar(bool bFollowCubeColor)
{
	float randY = FMath::RandRange(-3000.f, 3000.f);
	float randDir;
	if (randY > 1500.f) {
		randDir = FMath::RandRange(0.f, -90.f);
	}
	else if (randY > 0.f) {
		randDir = FMath::RandRange(-30.f, -90.f);
	}
	else if (randY > -1500.f) {
		randDir = FMath::RandRange(30.f, 90.f);
	}
	else {
		randDir = FMath::RandRange(0.f, 90.f);
	}
	// Generate rhythm bar
	ARhythmBar* barSpawned = Cast<ARhythmBar>(GetWorld()->SpawnActor<AActor>(ActorsToSpawn[0], FVector(-10000.f, randY, 2000.f), FRotator(0.f, 0.f, randDir)));
	barSpawned->barID = (bFollowCubeColor) ? (CubeID >= 4) : !(CubeID >= 4); // Set rhythm bar color (Follow | Invert)
	// Update rhythm bar color
	barSpawned->ApplyInitialSettings();
	return (randY < 0.f);
}