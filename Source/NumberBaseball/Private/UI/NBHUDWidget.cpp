// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBHUDWidget.h"

#include "Game/NBGameStateBase.h"
#include "Player/NBPlayerState.h"
#include "UI/NBAnnouncementWidget.h"
#include "UI/NBHistoryWidget.h"
#include "UI/NBInputWidget.h"
#include "UI/NBStatusBarWidget.h"

void UNBHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 1. 전광판(GameState)과 내 신분증(Local PlayerState)을 가져옵니다.
	ANBGameStateBase* NBGameStateBase = GetWorld()->GetGameState<ANBGameStateBase>();
	ANBPlayerState* LocalNBPlayerState = GetOwningPlayerState<ANBPlayerState>();

	if (NBGameStateBase && LocalNBPlayerState)
	{
		// 2. 스테이터스바를 업데이트합니다.
		if (StatusBar)
		{
			int32 CurrentAttempt = LocalNBPlayerState->GetCurrentAttempt();
			int32 MaxAttempt = LocalNBPlayerState->GetMaxAttempt();

			FString CurrentPlayerName = TEXT("대기 중");
			bool bIsMyTurn = false; // 기본값은 false
			if (NBGameStateBase->CurrentTurnPlayer)
			{
				CurrentPlayerName = NBGameStateBase->CurrentTurnPlayer->GetPlayerName();
				
				bIsMyTurn = (NBGameStateBase->CurrentTurnPlayer == LocalNBPlayerState);
			}

			int32 TimeRemaining = NBGameStateBase->TurnTimeRemaining;

			StatusBar->UpdateStatusBar(CurrentAttempt, MaxAttempt, CurrentPlayerName, TimeRemaining, bIsMyTurn);
		}

		if (InputArea)
		{
			bool bIsMyTurn = (NBGameStateBase->CurrentTurnPlayer == LocalNBPlayerState);
			InputArea->SetInputState(bIsMyTurn);
		}
	}
}

void UNBHUDWidget::AddGuessHistory(const FString& Guess, const FString& Result)
{
	if (GameHistory)
	{
		// 자식 위젯인 GameHistory에게 실제 기록 추가 업무를 넘깁니다.
		GameHistory->AddHistoryEntry(Guess, Result);
	}
}

void UNBHUDWidget::ShowAnnouncementPopup(const FString& Message, float DisplayTime)
{
	if (Announcement)
	{
		Announcement->ShowAnnouncement(Message, DisplayTime);
	}
}
