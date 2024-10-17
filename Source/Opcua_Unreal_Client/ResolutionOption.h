// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResolutionOption.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FResolutionOption
{
    GENERATED_BODY()

    // 해상도 텍스트 (예: 1920x1080)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
    FString ResolutionLabel;

    // 해상도 값 (가로 x 세로)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
    FVector2D ResolutionValue;
};

USTRUCT(BlueprintType)
struct FResolutionCategory
{
    GENERATED_BODY()

    // 해상도 카테고리 (예: 16:9)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
    FString CategoryLabel;

    // 이 카테고리 내의 해상도 목록
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
    TArray<FResolutionOption> Resolutions;
};