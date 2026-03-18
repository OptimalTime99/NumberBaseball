// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBHistoryWidget.generated.h"


class UScrollBox;

/**
 * NBHistoryWidget: 게임 기록을 보여주는 위젯
 */
UCLASS()
class NUMBERBASEBALL_API UNBHistoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* HistoryScrollBox;

	/** 새로운 기록을 한 줄 추가하는 함수 */
	void AddHistoryEntry(const FString& Guess, const FString& Result);

	/** 게임 리셋 시 기록을 모두 지우는 함수 */
	void ClearHistory();
};
