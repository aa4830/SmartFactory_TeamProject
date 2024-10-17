// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SShutDownWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPCUA_UNREAL_CLIENT_API USShutDownWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_yes;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_no;

	UFUNCTION()
	void OnClickedYes();

	UFUNCTION()
	void OnClickedNo();

};
