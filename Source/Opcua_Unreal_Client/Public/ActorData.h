// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorData.generated.h"

UCLASS(BlueprintType)
class OPCUA_UNREAL_CLIENT_API AActorData : public AActor
{
	GENERATED_BODY()



public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	int32 TargetIndex;
};
