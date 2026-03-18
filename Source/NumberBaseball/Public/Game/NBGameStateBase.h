// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NBGameStateBase.generated.h"

/**
 * NBGameStateBase: 남은 시간과 현재 누구 턴인지를 모든 클라이언트가 알 수 있도록 함
 */
UCLASS()
class NUMBERBASEBALL_API ANBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ANBGameStateBase();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** 서버가 줄여나갈 남은 시간 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball Turn")
	int32 TurnTimeRemaining;

	/** 현재 턴을 진행 중인 플레이어 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball Turn")
	class ANBPlayerState* CurrentTurnPlayer;
};
