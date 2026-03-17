// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBHUDWidget.generated.h"

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

	/** PlayerController가 서버 메시지를 받았을 때 호출할 함수 */
	void UpdateSystemMessage(const FString& Message);
	
protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> EditableTextBox_ChatInput;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_NotificationText;
};
