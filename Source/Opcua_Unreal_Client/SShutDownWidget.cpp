// Fill out your copyright notice in the Description page of Project Settings.


#include "SShutDownWidget.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetSystemLibrary.h"

void USShutDownWidget::NativeConstruct()
{
	Button_yes->OnClicked.AddDynamic(this, &USShutDownWidget::OnClickedYes);
	Button_no->OnClicked.AddDynamic(this, &USShutDownWidget::OnClickedNo);
}

void USShutDownWidget::OnClickedYes()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void USShutDownWidget::OnClickedNo()
{
	SetVisibility(ESlateVisibility::Hidden);
}