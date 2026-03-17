// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerState.h"

ANBPlayerState::ANBPlayerState()
{
}

void ANBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ANBPlayerState::IncrementAttempt()
{
}
