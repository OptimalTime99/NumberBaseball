// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerState.h"

#include "Net/UnrealNetwork.h"

ANBPlayerState::ANBPlayerState()
{
	// PlayerState는 기본적으로 리플리케이트 되지만, 명시적으로 선언
	bReplicates = true;
	
	// 초기값 세팅
	CurrentAttempt = 0;
	MaxAttempt = 3;
}

void ANBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// CurrentAttempt 변수를 서버에서 모든 클라이언트에 복제하도록 등록
	DOREPLIFETIME(ANBPlayerState, CurrentAttempt);
}

void ANBPlayerState::IncrementAttempt()
{
	// Authority(권한)을 가진 서버에서만 호출되도록 함
	if (HasAuthority() == true)
	{
		CurrentAttempt++;
	}
}

void ANBPlayerState::ResetAttempt()
{
	// Authority(권한)을 가진 서버에서만 호출되도록 함
	if (HasAuthority() == true)
	{
		CurrentAttempt = 0;
	}
}
