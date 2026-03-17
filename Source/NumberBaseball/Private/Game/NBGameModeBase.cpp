// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameModeBase.h"

#include "GameFramework/GameStateBase.h"
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
	}
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
    
    // 2. 현재 게임 상태(전광판)를 가져옵니다.
    AGameStateBase* GameStateBase = GetGameState<AGameStateBase>();
    if (GameStateBase)
    {
        // 3. GameState 안에 있는 모든 플레이어의 신분증 목록(PlayerArray)을 순회합니다.
        for (APlayerState* PS : GameStateBase->PlayerArray)
        {
            // 4. 우리가 만든 NBPlayerState로 캐스팅이 성공하면 리셋 함수를 호출합니다.
            if (ANBPlayerState* NBPlayerState = Cast<ANBPlayerState>(PS))
            {
                NBPlayerState->ResetAttempt();
            }
        }
    }
}

void ANBGameModeBase::Multicast_BroadcastResult_Implementation(const FString& ResultMessage)
{
	// 1. 서버 로그창에 결과 출력
    UE_LOG(LogTemp, Display, TEXT("[공지] %s"), *ResultMessage);
    
    // 2. 현재 월드(서버)에 접속해 있는 모든 플레이어 컨트롤러를 순회합니다.
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        // 3. 우리가 만든 커스텀 PlayerController로 캐스팅합니다.
        if (ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(It->Get()))
        {
            // 4. 각 PlayerController가 소유한 Client RPC를 호출하여 각자의 화면(UI)에 메시지를 전달합니다.
            NBPlayerController->ClientRPCReceiveSystemMessage(ResultMessage);
        }
    }
}
