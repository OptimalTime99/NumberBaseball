// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

/**
 * NBPlayerController: 클라이언트(플레이어)의 입력을 서버로 전달
 */
UCLASS()
class NUMBERBASEBALL_API ANBPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	/** 클라이언트가 UI에 숫자를 입력했을 때 호출되는 함수 */
	void SubmitNumberGuess(const FString& GuessInput);

	/** 클라이언트가 입력을 서버로 전달하는 Server RPC */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCSubmitNumberGuess(const FString& GuessInput);
};
