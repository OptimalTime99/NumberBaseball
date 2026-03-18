// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBHistoryWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UNBHistoryWidget::AddHistoryEntry(const FString& Guess, const FString& Result)
{
	if (!HistoryScrollBox) return;
	if (Guess.IsEmpty()) return;

	// 1. 스크롤 박스의 자식으로 들어갈 새로운 텍스트 블록 위젯을 생성합니다.
	UTextBlock* NewEntry = NewObject<UTextBlock>(this);
	if (NewEntry)
	{
		// 2. 표시할 텍스트 구성 (예: "입력: 123 ➔ 판정: 1S1B")
		FString EntryStr = FString::Printf(TEXT("입력: %s ➔ 판정: %s"), *Guess, *Result);
		NewEntry->SetText(FText::FromString(EntryStr));

		// 3. 텍스트 스타일 설정
		FSlateFontInfo FontInfo = NewEntry->GetFont();
		FontInfo.Size = 18;
		NewEntry->SetFont(FontInfo);

		// 약간의 여백(Padding) 추가
		NewEntry->SetMargin(FMargin(0, 5));

		// 4. 스크롤 박스에 자식으로 추가
		HistoryScrollBox->AddChild(NewEntry);

		// 5. 새로운 기록이 추가되면 자동으로 맨 아래로 스크롤
		HistoryScrollBox->ScrollToEnd();
	}
}

void UNBHistoryWidget::ClearHistory()
{
	if (HistoryScrollBox)
	{
		HistoryScrollBox->ClearChildren();
	}
}
