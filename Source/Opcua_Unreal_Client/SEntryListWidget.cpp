// Fill out your copyright notice in the Description page of Project Settings.


#include "SEntryListWidget.h"
#include "Components/TextBlock.h"
#include "MyCustomStruct.h"
#include "UListViewObject.h"

void USEntryListWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (const UUListViewObject* ListItem = Cast<UUListViewObject>(ListItemObject))
    {
        switch (ListItem->StructType)
        {
        case 1:
            // 첫 번째 구조체 데이터 처리
            DataText_1->SetText(FText::FromString(ListItem->MyLevelStruct.DisplayName));
            DataText_2->SetText(FText::AsNumber(ListItem->MyLevelStruct.ValueDouble));
            break;
        case 2:
            // 두 번째 구조체 데이터 처리
            DataText_1->SetText(FText::FromString(ListItem->MySwitchStruct.DisplayName));
            DataText_2->SetText(FText::FromString(ListItem->MySwitchStruct.ValueBool ? "True" : "False"));
            break;
        case 3:
            // 세 번째 구조체 데이터 처리
        {
            DataText_1->SetText(FText::FromString(ListItem->EventIdStruct.DisplayName));

            TArray<uint8> ByteArray = ListItem->EventIdStruct.ValueByteString;

            // 각 요소를 문자열로 변환하여 하나의 FString으로 결합
            FString ByteString;
            for (uint8 ByteValue : ByteArray)
            {
                ByteString += FString::FromInt(ByteValue) + TEXT(" ");  // 각 값 사이에 공백 추가
            }

            // 결합된 FString을 FText로 변환하여 SetText에 전달
            DataText_2->SetText(FText::FromString(ByteString));
            break;
        }
        case 4:
            // 네 번째 구조체 데이터 처리
            DataText_1->SetText(FText::FromString(ListItem->ReciveTimeStruct.DisplayName));
            DataText_2->SetText(FText::AsDateTime(ListItem->ReciveTimeStruct.ValueDateTime));
            break;
        case 5:
            // 다섯 번째 구조체 데이터 처리
            DataText_1->SetText(FText::FromString(ListItem->SeverityStruct.DisplayName));
            DataText_2->SetText(FText::AsNumber(ListItem->SeverityStruct.ValueInt32));
            break;
        default:
            break;
        }
    }
}
