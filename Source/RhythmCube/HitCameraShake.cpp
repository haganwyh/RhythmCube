// Fill out your copyright notice in the Description page of Project Settings.


#include "HitCameraShake.h"

// Constructor
UHitCameraShake::UHitCameraShake()
{
	OscillationDuration = 0.15f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	FOVOscillation.Amplitude = 20.0f;
	FOVOscillation.Frequency = 7.0f;
	FOVOscillation.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
}