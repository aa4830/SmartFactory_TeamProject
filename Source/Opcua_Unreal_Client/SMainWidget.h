// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UListViewObject.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "SMainWidget.generated.h"
/**
 * 
 */
UCLASS()
class OPCUA_UNREAL_CLIENT_API USMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedOff();

	UFUNCTION()
	void OnClickedOption();

	void UpdateDateTime();
	UFUNCTION(BlueprintCallable)
	void OnResetButtonClicked();
	UFUNCTION(BlueprintCallable)
	void UpdateFouceAcotr(UObject* ClickedItem);

	UPROPERTY(meta = (BindWidget))
	class UButton* home;

	UPROPERTY(meta = (BindWidget))
	class UWidget* BP_ShutDownWidget;

	UPROPERTY(meta = (BindWidget))
	class UWidget* BP_optionwidget;

	UPROPERTY(meta = (BindWidget))
	class UButton* Off;

	UPROPERTY(meta = (BindWidget))
	class UButton* option;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTreeView* TreeView;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Timer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UListView* ListView;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//class UTreeView* TreeView;

	UFUNCTION(BlueprintCallable, Category = "DateTime")
	FText GetCurrentDateTime() const;

private:
	FTimerHandle TimerHandle;
};
