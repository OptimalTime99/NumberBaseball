// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameModeBase.h"

ANBGameModeBase::ANBGameModeBase()
{
}

void ANBGameModeBase::GenerateRandomNumbers()
{
}

bool ANBGameModeBase::IsValidInput(const FString& Input)
{
	return false;
}

FString ANBGameModeBase::CheckAnswer(const FString& Input)
{
	return Input;
}

void ANBGameModeBase::ResetGame()
{
}

void ANBGameModeBase::Multicast_BroadcastResult_Implementation(const FString& ResultMessage)
{
}
