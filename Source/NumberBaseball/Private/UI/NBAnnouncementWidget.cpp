// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBAnnouncementWidget.h"

#include "Components/TextBlock.h"

void UNBAnnouncementWidget::ShowAnnouncement(const FString& Message, float DisplayTime)
{
	if (AnnouncementText)
	{
		// 1. 텍스트 설정
		AnnouncementText->SetText(FText::FromString(Message));

		// 2. 위젯을 화면에 보이게 설정
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// 3. 기존에 돌아가던 타이머가 있다면 초기화 (연속으로 팝업이 뜰 경우 꼬임 방지)
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);

		// 4. 지정된 시간(DisplayTime) 후에 HideAnnouncement 함수를 실행하도록 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(
			HideTimerHandle,
			this,
			&UNBAnnouncementWidget::HideAnnouncement,
			DisplayTime,
			false // 반복 안 함
		);
	}
}

void UNBAnnouncementWidget::HideAnnouncement()
{
	// 위젯을 다시 숨김 처리
	SetVisibility(ESlateVisibility::Hidden);
}
