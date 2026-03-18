// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBHUDWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Game/NBGameStateBase.h"
#include "Player/NBPlayerState.h"
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

void UNBHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// 1. 전광판(GameState)과 내 신분증(Local PlayerState)을 가져옵니다.
    ANBGameStateBase* NBGameStateBase = GetWorld()->GetGameState<ANBGameStateBase>();
    ANBPlayerState* LocalNBPlayerState = GetOwningPlayerState<ANBPlayerState>();

    if (NBGameStateBase && LocalNBPlayerState)
    {
        // 2. 타이머 텍스트 업데이트
        if (TextBlock_Timer)
        {
            FString TimerStr = FString::Printf(TEXT("남은 시간: %d초"), NBGameStateBase->TurnTimeRemaining);
            TextBlock_Timer->SetText(FText::FromString(TimerStr));
        }

        // 3. 내 턴인지 확인하여 입력창 활성화/비활성화 처리
        if (EditableTextBox_ChatInput)
        {
            // 전광판에 등록된 '현재 턴 플레이어'와 '나'의 메모리 주소가 같은지(동일 인물인지) 비교
            bool bIsMyTurn = (NBGameStateBase->CurrentTurnPlayer == LocalNBPlayerState);
            
            // 내 턴이면 활성화(true), 아니면 비활성화(false)하여 물리적 입력 차단
            EditableTextBox_ChatInput->SetIsEnabled(bIsMyTurn);
            
            // 디테일 추가: 활성화 상태에 따라 입력창 내부의 힌트 텍스트(배경 글자)를 변경해줍니다.
            if (bIsMyTurn)
            {
                EditableTextBox_ChatInput->SetHintText(FText::FromString(TEXT("숫자를 입력 후 Enter")));
            }
            else
            {
                EditableTextBox_ChatInput->SetHintText(FText::FromString(TEXT("상대방의 턴을 기다리는 중...")));
            }
        }
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
