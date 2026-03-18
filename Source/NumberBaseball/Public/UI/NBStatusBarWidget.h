// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBStatusBarWidget.generated.h"

class UTextBlock;

/**
 * UNBStatusBarWidget: 남은 기회, 현재 턴, 남은 시간을 보여주는 위젯
 */
UCLASS()
class NUMBERBASEBALL_API UNBStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttemptsText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TurnText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;

	/** 스테이터스바 상태 갱신 함수 */
	void UpdateStatusBar(
		int32 CurrentAttempt,
		int32 MaxAttempt,
		const FString& CurrentPlayerName,
		int32 TimeRemaining,
		bool bIsMyTurn);
};
