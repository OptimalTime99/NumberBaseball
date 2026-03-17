// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerController.h"

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController() == false) return;
}

void ANBPlayerController::SubmitNumberGuess(const FString& GuessInput)
{
}

void ANBPlayerController::ServerRPCSubmitNumberGuess_Implementation(const FString& GuessInput)
{
}

bool ANBPlayerController::ServerRPCSubmitNumberGuess_Validate(const FString& GuessInput)
{
	return false;
}
