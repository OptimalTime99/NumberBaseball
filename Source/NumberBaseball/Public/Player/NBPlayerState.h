// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

/**
 * NBPlayerState: 플레이어의 개별 데이터(시도 횟수 등)를 독립적으로 관리/동기화
 */
UCLASS()
class NUMBERBASEBALL_API ANBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ANBPlayerState();

protected:
	/** 언리얼 네트워크 복제(Replication) 설정 함수 */
	virtual void GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** 시도 횟수를 1 증가시키는 함수 */
	void IncrementAttempt();

	/** UI에 바인딩하기 위한 Getter 함수들 */
	int32 GetCurrentAttempt() const { return CurrentAttempt; }
	int32 GetMaxAttempt() const { return MaxAttempt; }

private:
	/** 클라이언트에게 자동으로 동기화될 현재 시도 횟수 */
	UPROPERTY(Replicated)
	int32 CurrentAttempt;

	/** 최대 시도 횟수 (기본 3회) */
	int32 MaxAttempt;
};
