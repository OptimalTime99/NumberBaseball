// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBAnnouncementWidget.generated.h"

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API UNBAnnouncementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AnnouncementText;

	/** 팝업을 띄우는 함수 */
	void ShowAnnouncement(const FString& Message, float DisplayTime = 3.0f);

protected:
	/** 팝업을 숨기는 내부 함수 */
	void HideAnnouncement();

private:
	/** 타이머를 관리할 핸들 */
	FTimerHandle HideTimerHandle;
};
