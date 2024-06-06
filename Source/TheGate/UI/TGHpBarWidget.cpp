// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TGHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/TGCharacterWidgetInterface.h"

UTGHpBarWidget::UTGHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UTGHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensure(HpProgressBar);

	ITGCharacterWidgetInterface* CharacterWidget = Cast<ITGCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UTGHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
