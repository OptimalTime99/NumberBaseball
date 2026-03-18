// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBInputWidget.generated.h"

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API UNBInputWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> NumberInputBox;

	/** 내 턴인지 여부에 따라 입력창을 켜고 끄는 함수 */
	void SetInputState(bool bIsMyTurn);

protected:
	/** 엔터키를 쳤을 때 실행될 함수 */
	UFUNCTION()
	void OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
