// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBInputWidget.h"

#include "Components/EditableTextBox.h"
#include "Player/NBPlayerController.h"

void UNBInputWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NumberInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnInputCommitted) == false)
	{
		NumberInputBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnInputCommitted);
	}
}

void UNBInputWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (NumberInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnInputCommitted) == true)
	{
		NumberInputBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnInputCommitted);
	}
}

void UNBInputWidget::SetInputState(bool bIsMyTurn)
{
	if (NumberInputBox)
	{
		// 턴에 따라 입력 가능 여부 설정
		NumberInputBox->SetIsEnabled(bIsMyTurn);

		if (bIsMyTurn)
		{
			NumberInputBox->SetHintText(FText::FromString(TEXT("숫자를 입력 후 Enter")));
		}
		else
		{
			NumberInputBox->SetHintText(FText::FromString(TEXT("상대방의 턴을 기다리는 중...")));
		}
	}
}

void UNBInputWidget::OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 엔터키를 쳤을 때만 동작
	if (CommitMethod == ETextCommit::OnEnter)
	{
		FString InputStr = Text.ToString();
		if (InputStr.IsEmpty()) return;

		// PlayerController를 찾아 서버로 입력값 전송
		if (ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(GetOwningPlayer()))
		{
			NBPlayerController->SubmitNumberGuess(InputStr);
		}

		// 전송 후 입력창 비우기 및 포커스 유지
		if (NumberInputBox)
		{
			NumberInputBox->SetText(FText::GetEmpty());
			NumberInputBox->SetKeyboardFocus();
		}
	}
}
