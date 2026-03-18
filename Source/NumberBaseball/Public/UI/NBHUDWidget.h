// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBHUDWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API UNBHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	// 매 프레임마다 호출되어 UI를 업데이트할 함수입니다. (블루프린트의 Event Tick과 동일)
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** PlayerController가 서버 메시지를 받았을 때 호출할 함수 */
	void UpdateSystemMessage(const FString& Message);
	
protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> EditableTextBox_ChatInput;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NotificationText;
	
	/** 타이머를 표시할 텍스트 블록 */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock_Timer;
};
