// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/TreeView.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "TreeEntryWidget.generated.h"
/**
 * 
 */
UCLASS()
class OPCUA_UNREAL_CLIENT_API UTreeEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
    virtual void NativeOnListItemObjectSet(UObject* ItemData) override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TreeText_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TreeText_2;
	
};
