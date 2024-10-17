// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "TreeViewStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTreeViewStruct
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Struct")
	FString Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Struct")
	FString Value;

};
