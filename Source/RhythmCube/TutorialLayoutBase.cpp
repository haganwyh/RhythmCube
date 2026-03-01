// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLayoutBase.h"
#include "Components/TextBlock.h"

void UTutorialLayoutBase::NativeOnInitialized()
{
	TutorialCount = 0;
}

void UTutorialLayoutBase::UpdateTutorialText()
{
	// Fade out animation
	switch (TutorialCount - 1)
	{
	case 1:
		PlayAnimation(Tutorial1Out);
		break;
	case 4:
		PlayAnimation(Tutorial4Out);
		break;
	case 7:
		PlayAnimation(Tutorial7Out);
		break;
	case 8:
		PlayAnimation(Tutorial8Out);
		break;
	case 9:
		PlayAnimation(Tutorial9Out);
		break;
	case 10:
		PlayAnimation(Tutorial10Out);
		break;
	case 13:
		PlayAnimation(Tutorial13Out);
		break;
	case 14:
		PlayAnimation(Tutorial14Out);
		break;
	case 15:
		PlayAnimation(Tutorial15Out);
		break;
	case 16:
		PlayAnimation(Tutorial16Out);
		break;
	default:;
	}

	if (TutorialCount == 0 || TutorialTextList[TutorialCount - 1] == "")
	{
		TutorialText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *TutorialTextList[TutorialCount])));
		PlayAnimation(FadeIn);
		TutorialCount++;
	}
	else
	{
		if (TutorialTextList[TutorialCount] == "")
		{
			PlayAnimation(FadeOut);
		}
		else
		{
			PlayAnimation(TextFadeOutIn);
		}
		FTimerHandle AnimationTimer;
		GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this]
			{
				// Fade in animation
				switch (TutorialCount)
				{
				case 1:
					PlayAnimation(Tutorial1In);
					break;
				case 4:
					PlayAnimation(Tutorial4In);
					break;
				case 7:
					PlayAnimation(Tutorial7In);
					break;
				case 8:
					PlayAnimation(Tutorial8In);
					break;
				case 9:
					PlayAnimation(Tutorial9In);
					break;
				case 10:
					PlayAnimation(Tutorial10In);
					break;
				case 13:
					PlayAnimation(Tutorial13In);
					break;
				case 14:
					PlayAnimation(Tutorial14In);
					break;
				case 15:
					PlayAnimation(Tutorial15In);
					break;
				case 16:
					PlayAnimation(Tutorial16In);
					break;
				default:;
				}

				TutorialText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *TutorialTextList[TutorialCount])));
				TutorialCount++;
			}, 0.25f, false);
	}
}

void UTutorialLayoutBase::EndTutorialText()
{
	PlayAnimation(EndTextFadeInOut);
}