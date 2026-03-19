// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBInputWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Player/NBPlayerController.h"

void UNBInputWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    // 1. 텍스트 박스 이벤트 바인딩
    if (NumberInputBox)
    {
       NumberInputBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::HandleTextCommitted);
       NumberInputBox->OnTextCommitted.AddDynamic(this, &ThisClass::HandleTextCommitted);
    }
    
    // 2. 제출 버튼 이벤트 바인딩 (주의: AddDynamic의 첫 인자는 무조건 this여야 합니다)
    if (SubmitNumberButton)
    {
        SubmitNumberButton->OnClicked.RemoveDynamic(this, &ThisClass::HandleSubmitButtonClicked);
        SubmitNumberButton->OnClicked.AddDynamic(this, &ThisClass::HandleSubmitButtonClicked);
    }
}

void UNBInputWidget::NativeDestruct()
{
    Super::NativeDestruct();

    // 깔끔한 메모리 관리를 위해 바인딩 해제 (IsAlreadyBound 체크보다 확실합니다)
    if (NumberInputBox)
    {
       NumberInputBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::HandleTextCommitted);
    }

    if (SubmitNumberButton)
    {
       SubmitNumberButton->OnClicked.RemoveDynamic(this, &ThisClass::HandleSubmitButtonClicked);
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

// --- 껍데기 함수 구현부 ---
void UNBInputWidget::HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    // Enter 키를 쳤을 때만 ProcessSubmit 실행
    if (CommitMethod == ETextCommit::OnEnter)
    {
       ProcessSubmit();
    }
}

void UNBInputWidget::HandleSubmitButtonClicked()
{
    // 버튼을 클릭하면 바로 ProcessSubmit 실행
    ProcessSubmit();
}

// --- 실제 로직 (알맹이 함수) 구현부 ---
void UNBInputWidget::ProcessSubmit()
{
    // NumberInputBox가 없으면 크래시 방지를 위해 바로 종료
    if (!NumberInputBox) return;

    // 텍스트 박스에서 현재 입력된 글자를 가져옵니다.
    FString InputStr = NumberInputBox->GetText().ToString();
    
    // 아무것도 입력하지 않았다면 무시
    if (InputStr.IsEmpty()) return;

    // PlayerController를 찾아 서버로 입력값 전송
    if (ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(GetOwningPlayer()))
    {
        NBPlayerController->SubmitNumberGuess(InputStr);
    }

    // 전송 후 입력창 비우기 및 포커스 유지
    NumberInputBox->SetText(FText::GetEmpty());
    NumberInputBox->SetKeyboardFocus();
}