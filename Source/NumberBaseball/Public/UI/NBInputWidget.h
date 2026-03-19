// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBInputWidget.generated.h"

/**
 * */
UCLASS()
class NUMBERBASEBALL_API UNBInputWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;
    virtual void NativeDestruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UEditableTextBox> NumberInputBox;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> SubmitNumberButton;

    /** 내 턴인지 여부에 따라 입력창을 켜고 끄는 함수 */
    void SetInputState(bool bIsMyTurn);

protected:
    // --- UI 이벤트 바인딩용 껍데기 함수 (UFUNCTION 필수) ---
    /** 텍스트 박스에서 입력을 완료(Enter 등)했을 때 실행될 함수 */
    UFUNCTION()
    void HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    /** 제출 버튼을 클릭했을 때 실행될 함수 */
    UFUNCTION()
    void HandleSubmitButtonClicked();

    // --- 실제 데이터 처리용 알맹이 함수 (UFUNCTION 불필요) ---
    /** 텍스트를 읽어와 서버로 전송하는 핵심 로직 */
    void ProcessSubmit();
};