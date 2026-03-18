// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBStatusBarWidget.h"
#include "Components/TextBlock.h"

void UNBStatusBarWidget::UpdateStatusBar(
	int32 CurrentAttempt,
	int32 MaxAttempt,
	const FString& CurrentPlayerName,
	int32 TimeRemaining,
	bool bIsMyTurn)
{
	// 1. 남은 기회 업데이트
	if (AttemptsText)
	{
		int32 AttemptsLeft = MaxAttempt - CurrentAttempt;
		AttemptsLeft = FMath::Max(0, AttemptsLeft);

		FString AttemptStr = FString::Printf(TEXT("남은 기회: %d / %d"), AttemptsLeft, MaxAttempt);
		AttemptsText->SetText(FText::FromString(AttemptStr));
	}

	// 2. 현재 턴 플레이어 이름 업데이트
	if (TurnText)
	{
		FString TurnStr;
		if (bIsMyTurn)
		{
			TurnStr = TEXT("당신의 턴입니다.");
		}
		else
		{
			TurnStr = FString::Printf(TEXT("%s의 턴입니다."), *CurrentPlayerName);
		}

		TurnText->SetText(FText::FromString(TurnStr));
	}

	// 3. 남은 시간 타이머 업데이트
	if (TimerText)
	{
		FString TimerStr = FString::Printf(TEXT("%d초"), TimeRemaining);
		TimerText->SetText(FText::FromString(TimerStr));
	}
}
