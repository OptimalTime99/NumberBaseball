// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameModeBase.generated.h"

/**
 * NBGameModeBase: 게임의 전체적인 규칙(판정, 승리 조건)을 서버 측에서 제어
 */
UCLASS()
class NUMBERBASEBALL_API ANBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANBGameModeBase();
	
	virtual void OnPostLogin(AController* NewPlayer) override;
	
protected:
	virtual void BeginPlay() override;

public:
	/** 게임 시작 시 서버에서 정답 숫자 3개를 생성하는 함수 */
	void GenerateRandomNumbers();

	/** 유효성 검사: 3자리인지, 숫자인지, 중복이 없는지 체크하는 함수 */
	bool IsValidInput(const FString& Input);

	/** 클라이언트가 입력한 숫자를 검증하고 결과를 반환하는 함수 (S, B, OUT) */
	FString CheckAnswer(const FString& Input);

	/** 승리 또는 무승부 시 게임 변수 초기화 및 재시작 함수 */
	void ResetGame();

	/** 게임 결과를 모든 클라이언트에게 알리는 Multicast RPC 함수 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastResult(const FString& ResultMessage);
	
	/** 무승부 상태인지 확인하는 함수 */
	bool CheckDrawCondition();

private:
	/** 서버만 알고 있는 정답 배열 */
	TArray<int32> AnswerNumbers;
	
	TArray<TObjectPtr<class ANBPlayerController>> AllPlayerControllers;
};
