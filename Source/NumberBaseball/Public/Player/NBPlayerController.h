// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

class UNBHUDWidget;

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

	/** 게임이 리셋될 때 클라이언트의 UI를 초기화하는 RPC */
	UFUNCTION(Client, Reliable)
	void ClientRPCResetGameUI();

	/** 답 제출 시 클라이언트의 화면에 게임 기록에 저장하는 Client RPC */
	UFUNCTION(Client, Reliable)
	void ClientRPCAddGuessHistory(const FString& GuessInput, const FString& Result);

	/** 서버가 클라이언트 화면 정중앙에 팝업 공지를 띄울 때 사용하는 Client RPC */
	UFUNCTION(Client, Reliable)
	void ClientRPCShowAnnouncement(const FString& Message, float DisplayTime = 3.0f);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UNBHUDWidget> HUDWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UNBHUDWidget> HUDWidgetInstance;
};
