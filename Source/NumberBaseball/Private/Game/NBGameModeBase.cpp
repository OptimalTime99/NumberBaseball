// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameModeBase.h"
#include "Game/NBGameStateBase.h"
#include "Player/NBPlayerController.h"
#include "Player/NBPlayerState.h"

ANBGameModeBase::ANBGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(NewPlayer);
	if (IsValid(NBPlayerController) == true)
	{
		AllPlayerControllers.Add(NBPlayerController);
		AssignedPlayerNumber++;

		FString NewName = FString::Printf(TEXT("플레이어 %d"), AssignedPlayerNumber);
		NewPlayer->PlayerState->SetPlayerName(NewName);

		UE_LOG(LogTemp, Display, TEXT("플레이어 입장 완료. 부여된 이름: %s"), *NewName);
	}

	if (GetNumPlayers() >= 2 && CurrentPlayerIndex == -1)
	{
		StartNextTurn();
	}
}

void ANBGameModeBase::Logout(AController* Exiting)
{
	// 나가는 컨트롤러를 캐스팅
	if (ANBPlayerController* ExitingPC = Cast<ANBPlayerController>(Exiting))
	{
		// 배열에서 안전하게 제거!
		AllPlayerControllers.Remove(ExitingPC);
		UE_LOG(LogTemp, Warning, TEXT("플레이어가 퇴장하여 배열에서 제거되었습니다. 현재 남은 인원: %d"), AllPlayerControllers.Num());
	}

	Super::Logout(Exiting);
}

void ANBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GenerateRandomNumbers();
}

void ANBGameModeBase::GenerateRandomNumbers()
{
	// 기존에 있던 정답을 비움
	AnswerNumbers.Empty();

	// 1~9 사이의 중복되지 않는 3자리 숫자 생성
	while (AnswerNumbers.Num() < 3)
	{
		int32 RandNum = FMath::RandRange(1, 9);

		if (!AnswerNumbers.Contains(RandNum))
		{
			AnswerNumbers.Add(RandNum);
		}
	}

	// 서버 로그창에 정답 출력
	FString AnswerStr = FString::Printf(TEXT("%d%d%d"), AnswerNumbers[0], AnswerNumbers[1], AnswerNumbers[2]);
	UE_LOG(LogTemp, Warning, TEXT("[Server] 숫자 야구 정답 생성 완료: %s"), *AnswerStr);
}

bool ANBGameModeBase::IsValidInput(const FString& Input)
{
	// 1. 3자리 숫자인가?
	if (Input.Len() != 3) return false;

	for (int32 i = 0; i < Input.Len(); ++i)
	{
		// 2. 문자가 포함되지는 않았는가? (1~9 사이의 숫자인지 확인)
		if (!FChar::IsDigit(Input[i]) || Input[i] == '0') return false;

		// 3. 중복되는 숫자가 있는가?
		for (int32 j = i + 1; j < Input.Len(); ++j)
		{
			if (Input[i] == Input[j]) return false;
		}
	}

	return true;
}

FString ANBGameModeBase::CheckAnswer(const FString& Input)
{
	int32 Strikes = 0;
	int32 Balls = 0;

	for (int32 i = 0; i < Input.Len(); ++i)
	{
		int32 GuessNum = Input[i] - '0';

		if (AnswerNumbers[i] == GuessNum) Strikes++;
		else if (AnswerNumbers.Contains(GuessNum)) Balls++;
	}

	if (Strikes == 0 && Balls == 0) return TEXT("OUT");

	return FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
}

void ANBGameModeBase::ResetGame()
{
	// 1. 정답 숫자 새로 생성
	GenerateRandomNumbers();

	// 2. 월드에 있는 모든 플레이어 컨트롤러를 순회합니다. 
	for (ANBPlayerController* NBPlayerController : AllPlayerControllers)
	{
		if (IsValid(NBPlayerController))
		{
			// 3. 클라이언트 UI 리셋 명령 (Client RPC 호출)
			NBPlayerController->ClientRPCResetGameUI();

			// 4. 플레이어 상태(데이터) 리셋
			if (ANBPlayerState* NBPlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>())
			{
				NBPlayerState->ResetAttempt();
			}
		}
	}

	// 5. 게임 흐름 초기화: 첫 번째 플레이어부터 다시 시작하도록 세팅
	CurrentPlayerIndex = -1;

	UE_LOG(LogTemp, Log, TEXT("게임이 성공적으로 리셋되었습니다. 모든 UI와 기록이 초기화되었습니다."));
}

void ANBGameModeBase::Multicast_BroadcastResult_Implementation(const FString& ResultMessage, float DisplayTime)
{
	// 1. 서버 로그창에 결과 출력
	UE_LOG(LogTemp, Display, TEXT("[공지] %s"), *ResultMessage);

	// 2. 현재 월드(서버)에 접속해 있는 모든 플레이어 컨트롤러를 순회합니다.
	for (ANBPlayerController* NBPlayerController : AllPlayerControllers)
	{
		if (IsValid(NBPlayerController))
		{
			// 3. 각 PlayerController가 소유한 Client RPC를 호출하여 각자의 화면(UI)에 메시지를 전달합니다.
			NBPlayerController->ClientRPCShowAnnouncement(ResultMessage, DisplayTime);
		}
	}
}

bool ANBGameModeBase::CheckDrawCondition()
{
	// 전광판 역할을 하는 GameState를 가져옵니다.
	AGameStateBase* GameStateBase = GetGameState<AGameStateBase>();
	if (!GameStateBase) return false;

	// 만약 접속한 플레이어가 한 명도 없다면 무승부 판정을 할 필요가 없습니다.
	if (GameStateBase->PlayerArray.Num() == 0) return false;

	// 모든 플레이어의 상태를 검사합니다.
	for (APlayerState* PlayerState : GameStateBase->PlayerArray)
	{
		if (ANBPlayerState* NBPlayerState = Cast<ANBPlayerState>(PlayerState))
		{
			// 단 한 명이라도 현재 시도 횟수가 최대 시도 횟수보다 적다면(기회가 남아있다면)
			// 아직 게임이 끝난 것이 아니므로 false를 반환합니다.
			if (NBPlayerState->GetCurrentAttempt() < NBPlayerState->GetMaxAttempt())
			{
				return false;
			}
		}
	}

	// 위 루프에서 false가 반환되지 않고 여기까지 무사히 도달했다면,
	// "모든 플레이어가 기회를 소진했다"는 뜻이므로 무승부(true)를 반환합니다.
	return true;
}

void ANBGameModeBase::StartNextTurn()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);

	ANBGameStateBase* NBGameStateBase = GetGameState<ANBGameStateBase>();
	if (!NBGameStateBase || NBGameStateBase->PlayerArray.Num() == 0) return;

	// 다음 플레이어로 인덱스 이동
	CurrentPlayerIndex = (CurrentPlayerIndex + 1) % NBGameStateBase->PlayerArray.Num();

	// GameState에 현재 턴 플레이어와 초기 시간 세팅
	NBGameStateBase->CurrentTurnPlayer = Cast<ANBPlayerState>(NBGameStateBase->PlayerArray[CurrentPlayerIndex]);
	NBGameStateBase->TurnTimeRemaining = 10;

	// 1초마다 OnTurnTimerTick 함수를 반복 실행하는 타이머 설정
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANBGameModeBase::OnTurnTimerTick, 1.0f, true);
}

void ANBGameModeBase::OnTurnTimerTick()
{
	ANBGameStateBase* NBGameStateBase = GetGameState<ANBGameStateBase>();
	if (!NBGameStateBase) return;

	NBGameStateBase->TurnTimeRemaining--; // 시간 1초 차감

	// 시간이 0초가 되면
	if (NBGameStateBase->TurnTimeRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(TurnTimerHandle); // 타이머 중지

		if (NBGameStateBase->CurrentTurnPlayer)
		{
			NBGameStateBase->CurrentTurnPlayer->IncrementAttempt(); // 턴 내에 미입력 시 기회 소진
			FString TurnMessage = FString::Printf(
				TEXT("시간 초과!\n %s의 기회가 1회 소진되었습니다."), *NBGameStateBase->CurrentTurnPlayer->GetPlayerName());
			Multicast_BroadcastResult(TurnMessage);

			for (ANBPlayerController* NBPlayerController : AllPlayerControllers)
			{
				if (IsValid(NBPlayerController))
				{
					NBPlayerController->ClientRPCShowAnnouncement(TurnMessage, 3.0f);
				}
			}
		}

		// 무승부 체크 후 턴을 넘기거나 게임을 끝냄 (이전 무승부 로직 활용)
		if (CheckDrawCondition())
		{
			Multicast_BroadcastResult(TEXT("모든 기회 소진!\n 무승부입니다."));
			ResetGame();
		}
		
		StartNextTurn(); // 다음 사람에게 턴 넘김
	}
}


