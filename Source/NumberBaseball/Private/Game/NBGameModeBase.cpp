// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameModeBase.h"

#include "Player/NBPlayerController.h"

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
	GenerateRandomNumbers();
	
	// TODO: 현재 접속 중인 모든 PlayerState의 '시도 횟수'를 0으로 초기화하는 로직 추가 필요
}

void ANBGameModeBase::Multicast_BroadcastResult_Implementation(const FString& ResultMessage)
{
	UE_LOG(LogTemp, Display, TEXT("[공지] %s"), *ResultMessage);
}
