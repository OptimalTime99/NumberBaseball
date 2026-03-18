// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameStateBase.h"

#include "Net/UnrealNetwork.h"

ANBGameStateBase::ANBGameStateBase()
{
	TurnTimeRemaining = 10;
	CurrentTurnPlayer = nullptr;
}

void ANBGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 두 변수를 모든 클라이언트에게 동기화
	DOREPLIFETIME(ANBGameStateBase, TurnTimeRemaining);
	DOREPLIFETIME(ANBGameStateBase, CurrentTurnPlayer);
}
