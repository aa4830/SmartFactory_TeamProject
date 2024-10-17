// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeEntryWidget.h"
#include "Components/TextBlock.h"
#include "UTreeViewObject.h"

void UTreeEntryWidget::NativeOnListItemObjectSet(UObject* ItemData)
{
    if (const UTreeViewObject* TreeItem = Cast<UTreeViewObject>(ItemData))
    {
        if (TreeItem)
        {
            TreeText_1->SetText(FText::FromString(TreeItem->Key));
            TreeText_2->SetText(FText::FromString(TreeItem->Value));
        }
    }
}
