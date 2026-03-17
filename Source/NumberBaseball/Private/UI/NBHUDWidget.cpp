// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBHUDWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Player/NBPlayerController.h"

void UNBHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UNBHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UNBHUDWidget::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
if (CommitMethod == ETextCommit::OnEnter)
    {
        FString InputStr = Text.ToString();
        
        // 빈칸인 상태에서 엔터를 쳤을 때는 무시하도록 예외 처리
        if (InputStr.IsEmpty()) return;

        // 서버(PlayerController)로 입력값 전송
        if (ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(GetOwningPlayer()))
        {
            NBPlayerController->SubmitNumberGuess(InputStr);
        }
        
        // 전송 완료 후 다음 입력을 위해 텍스트 박스를 비우고, 
        // 연속해서 입력할 수 있도록 다시 텍스트 박스에 포커스를 줍니다.
        if (IsValid(EditableTextBox_ChatInput) == true)
        {
            EditableTextBox_ChatInput->SetText(FText::GetEmpty());
            EditableTextBox_ChatInput->SetKeyboardFocus();
        }
    }
}

void UNBHUDWidget::UpdateSystemMessage(const FString& Message)
{
	if (IsValid(TextBlock_NotificationText) == true)
	{
		TextBlock_NotificationText->SetText(FText::FromString(Message));
	}
}
