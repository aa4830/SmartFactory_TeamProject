// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyCustomStruct.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UListViewObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)  // 블루프린트에서 사용할 수 있도록 표시
class OPCUA_UNREAL_CLIENT_API UUListViewObject : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
      FMyLevelStruct MyLevelStruct;

      UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
      FMySwitchStruct MySwitchStruct;

      UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
      FEventIdStruct EventIdStruct;

      UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
      FReciveTimeStruct ReciveTimeStruct;

      UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
      FSeverityStruct SeverityStruct;

    // 타입을 구분하는 플래그
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 StructType; // 구조체 타입을 1, 2, 3, 4, 5로 구분
};