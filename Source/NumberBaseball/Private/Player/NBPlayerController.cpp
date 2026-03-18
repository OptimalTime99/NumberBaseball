// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerController.h"

#include "Components/EditableTextBox.h"
#include "Game/NBGameModeBase.h"
#include "Game/NBGameStateBase.h"
#include "Player/NBPlayerState.h"
#include "UI/NBHUDWidget.h"

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

	if (HUDWidgetInstance->EditableTextBox_ChatInput)
	{
		InputMode.SetWidgetToFocus(HUDWidgetInstance->EditableTextBox_ChatInput->TakeWidget());
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
		ClientRPCReceiveSystemMessage(TEXT("지금은 당신의 턴이 아닙니다. 기다려주세요."));
		return;
	}


	// 기회를 모두 소진했다면 더 이상 진행하지 않습니다.
	if (NBPlayerState->GetCurrentAttempt() >= NBPlayerState->GetMaxAttempt())
	{
		ClientRPCReceiveSystemMessage(TEXT("기회를 모두 소진했습니다. 결과를 기다려주세요."));
		return;
	}

	// 3. 게임의 규칙을 통제하는 서버의 GameMode 가져오기
	ANBGameModeBase* NBGameModeBase = Cast<ANBGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!NBGameModeBase) return;

	// 4. 유효성 검사: 입력이 규칙에 맞지 않으면 기회를 깎지 않고 돌려보냅니다.
	if (!NBGameModeBase->IsValidInput(GuessInput))
	{
		ClientRPCReceiveSystemMessage(TEXT("다시 입력하세요. 중복되지 않은 1~9 사이의 3자리 숫자여야 합니다."));
		return;
	}

	// 5. 유효한 입력이므로 시도 횟수를 1회 증가시킵니다.
	NBPlayerState->IncrementAttempt();

	// 6. GameMode에 판정을 요청합니다.
	FString Result = NBGameModeBase->CheckAnswer(GuessInput);

	// 7. 판정 결과를 개인에게 알려줍니다.
	int32 RemainingAttempts = NBPlayerState->GetMaxAttempt() - NBPlayerState->GetCurrentAttempt();
	FString ResultMessage = FString::Printf(
		TEXT("입력: %s → 판정: %s (남은 기회: %d)"), *GuessInput, *Result, RemainingAttempts);
	ClientRPCReceiveSystemMessage(ResultMessage);

	// 8. 승리 조건 체크
	if (Result == TEXT("3S0B"))
	{
		FString WinMessage = FString::Printf(TEXT("%s 플레이어 승리!"), *NBPlayerState->GetPlayerName());

		NBGameModeBase->Multicast_BroadcastResult(WinMessage);
		NBGameModeBase->ResetGame();
	}
	else // 9. 무승부 조건 체크
	{
		if (NBGameModeBase->CheckDrawCondition() == true)
		{
			NBGameModeBase->Multicast_BroadcastResult(TEXT("모든 플레이어가 기회를 소진했습니다. 무승부!"));
			NBGameModeBase->ResetGame();
		}
	}

	NBGameModeBase->StartNextTurn();
}


void ANBPlayerController::ClientRPCReceiveSystemMessage_Implementation(const FString& Message)
{
	if (HUDWidgetInstance == nullptr) return;

	HUDWidgetInstance->UpdateSystemMessage(Message);
}
