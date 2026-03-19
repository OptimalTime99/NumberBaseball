// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerController.h"

#include "Components/EditableTextBox.h"
#include "Game/NBGameModeBase.h"
#include "Game/NBGameStateBase.h"
#include "Player/NBPlayerState.h"
#include "UI/NBHistoryWidget.h"
#include "UI/NBHUDWidget.h"
#include "UI/NBInputWidget.h"

class ANBGameStateBase;

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false) return;
	if (IsValid(HUDWidgetClass) == false) return;

	HUDWidgetInstance = CreateWidget<UNBHUDWidget>(this, HUDWidgetClass);
	if (HUDWidgetInstance == nullptr) return;

	HUDWidgetInstance->AddToViewport();

	bShowMouseCursor = true;
	FInputModeUIOnly InputMode;

	if (HUDWidgetInstance->InputArea->NumberInputBox)
	{
		InputMode.SetWidgetToFocus(HUDWidgetInstance->InputArea->NumberInputBox->TakeWidget());
	}
	else
	{
		InputMode.SetWidgetToFocus(HUDWidgetInstance->TakeWidget());
	}

	SetInputMode(InputMode);
}

void ANBPlayerController::SubmitNumberGuess(const FString& GuessInput)
{
	ServerRPCSubmitNumberGuess(GuessInput);
}

void ANBPlayerController::ClientRPCAddGuessHistory_Implementation(const FString& GuessInput, const FString& Result)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->AddGuessHistory(GuessInput, Result);
	}
}

void ANBPlayerController::ClientRPCShowAnnouncement_Implementation(const FString& Message, float DisplayTime)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->ShowAnnouncementPopup(Message, DisplayTime);
	}
}

void ANBPlayerController::ClientRPCResetGameUI_Implementation()
{
	if (HUDWidgetInstance && HUDWidgetInstance->GameHistory)
	{
		// 1. 우측 기록창 비우기
		HUDWidgetInstance->GameHistory->ClearHistory();
	}
}

bool ANBPlayerController::ServerRPCSubmitNumberGuess_Validate(const FString& GuessInput)
{
    return true;
}

void ANBPlayerController::ServerRPCSubmitNumberGuess_Implementation(const FString& GuessInput)
{
	// 1. 플레이어 상태(시도 횟수) 가져오기
	ANBPlayerState* NBPlayerState = GetPlayerState<ANBPlayerState>();
	if (!NBPlayerState) return;

	// 2. 내 턴 아니면 입력 차단
	ANBGameStateBase* NBGameStateBase = GetWorld()->GetGameState<ANBGameStateBase>();
	if (NBGameStateBase && NBGameStateBase->CurrentTurnPlayer != NBPlayerState)
	{
		ClientRPCShowAnnouncement(TEXT("지금은 당신의 턴이 아닙니다.\n 기다려주세요."), 3.0f);
		return;
	}


	// 기회를 모두 소진했다면 더 이상 진행하지 않습니다.
	if (NBPlayerState->GetCurrentAttempt() >= NBPlayerState->GetMaxAttempt())
	{
		ClientRPCShowAnnouncement(TEXT("기회를 모두 소진했습니다.\n 결과를 기다려주세요."), 3.0f);
		return;
	}

	// 3. 게임의 규칙을 통제하는 서버의 GameMode 가져오기
	ANBGameModeBase* NBGameModeBase = Cast<ANBGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!NBGameModeBase) return;

	// 4. 유효성 검사: 입력이 규칙에 맞지 않으면 기회를 깎지 않고 돌려보냅니다.
	if (!NBGameModeBase->IsValidInput(GuessInput))
	{
		ClientRPCShowAnnouncement(TEXT("다시 입력하세요.\n 중복되지 않은 1~9 사이의\n 3자리 숫자여야 합니다."), 3.0f);
		return;
	}

	// 5. 유효한 입력이므로 시도 횟수를 1회 증가시킵니다.
	NBPlayerState->IncrementAttempt();

	// 6. GameMode에 판정을 요청합니다.
	FString Result = NBGameModeBase->CheckAnswer(GuessInput);
	ClientRPCAddGuessHistory(GuessInput, Result);


	// 8. 승리 조건 체크
	if (Result == TEXT("3S0B"))
	{
		FString WinMessage = FString::Printf(
			TEXT("%s 승리!\n 정답: %s\n 새 게임을 시작합니다."), *NBPlayerState->GetPlayerName(), *GuessInput);

		// 모든 접속자에게 승리 팝업(3초 유지)을 띄우도록 명령
		NBGameModeBase->Multicast_BroadcastResult(WinMessage, 3.0f);

		
		NBGameModeBase->ResetGame();
	}
	else // 9. 무승부 조건 체크
	{
		if (NBGameModeBase->CheckDrawCondition() == true)
		{
			FString DrawMessage = TEXT("모든 기회 소진!\n 무승부입니다.");
			
			// 모든 접속자에게 무승부 팝업(3초 유지)을 띄우도록 명령
			NBGameModeBase->Multicast_BroadcastResult(DrawMessage, 3.0f);
			NBGameModeBase->ResetGame();
		}
	}

	NBGameModeBase->StartNextTurn();
}
