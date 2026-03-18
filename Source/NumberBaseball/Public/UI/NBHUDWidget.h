// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBHUDWidget.generated.h"

class UTextBlock;
class UNBStatusBarWidget;
class UNBHistoryWidget;
class UNBAnnouncementWidget;
class UNBInputWidget;

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API UNBHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 매 프레임마다 호출되어 UI를 업데이트할 함수입니다. (블루프린트의 Event Tick과 동일)
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** 우측 기록창에 숫자와 판정 결과를 추가하는 함수 */
	void AddGuessHistory(const FString& Guess, const FString& Result);

	/** 공지 팝업을 띄우라고 자식에게 명령하는 함수 */
	void ShowAnnouncementPopup(const FString& Message, float DisplayTime = 3.0f);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNBStatusBarWidget> StatusBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNBHistoryWidget> GameHistory;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNBAnnouncementWidget> Announcement;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNBInputWidget> InputArea;
};
