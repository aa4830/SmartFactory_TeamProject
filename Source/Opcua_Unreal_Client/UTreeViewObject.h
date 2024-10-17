// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

/**
 * 
 */
class OPCUA_UNREAL_CLIENT_API UTreeViewObject : public UObject
{
public:
    UPROPERTY()
    UTreeViewObject* Parent;

    UPROPERTY()
    TArray<UTreeViewObject*> Children;

    UPROPERTY()
    FString Key; // 노드 이름

    UPROPERTY()
    FString Value;

    UTreeViewObject()
    {
        Parent = nullptr;
    }
};
