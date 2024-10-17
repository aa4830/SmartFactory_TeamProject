// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TreeView.h"
#include "UTreeViewObject.h"
#include "FieldCameraPawn.generated.h"


class USpringArmComponent;
class UCameraComponent;
UCLASS()
class OPCUA_UNREAL_CLIENT_API AFieldCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFieldCameraPawn();
	
	// Called every frame
	//선형 보간 
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//InputMapping 적용
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//F 누르면 플레이어 스타트 지점으로 이동
	void GoPlayerStartposition();

	//Z 눌르면 모든 액터 아웃라인하이라이트 해제
	UFUNCTION()
	void PowerOff();
	
	//void FocusOnActor(AActor* TargetActor);
	//FocusOnAllBPTestC함수 에서 받은 액터들 아웃라인 하이라트적용 및 액터들 사이 거리 중심으로 이동
	void FocusOnActors(TArray<AActor*>& TargetActors);
	//FocusOnBPTestC1 받은 액터이름 아웃라인적용및 해당 액터로이동
	void FocusOnActor2(AActor* TargetActor);


	//void FocusOnTarget();
	//1번키 누르면 BP_Test_C 포함한 액터 찾기
	void FocusOnAllBPTestC(int32 InIndex);
	//2번키 누르면 BP_Test_C_1 이름을 가진 액터 찾기
	void FocusOnBPTestC1();
	
	//포커스 액터초기화
	UFUNCTION(BlueprintCallable)
	void ResetFocus();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//W,S 로도 줌기능
	UFUNCTION()
	void Forward(float AxisValue);

	//D,A 45도 회전기능
	UFUNCTION()
	void Right(float AxisValue);

	//마우스 휠 줌
	UFUNCTION()
	void Zoom(float AxisValue);

	//E눌르면 45도 회전
	UFUNCTION()
	void RotateRight();

	//Q눌르면 -45도 회전
	UFUNCTION()
	void RotateLeft();
	//회전상태 체크
	UFUNCTION()
	void EnableRotate();
	//회전상태 체크
	UFUNCTION()
	void DisableRotate();


	//아웃라인 적용되있는지 확인
	UPROPERTY()
	bool bOutLineHightRight = false;

	//마우스 수평 수직 제한이지만 필요없어서 주석처리함 안쓰는기능
	UFUNCTION()
	void RotateHorizontal(float AxisValue);
	//마우스 수평 수직 제한이지만 필요없어서 주석처리함 안쓰는기능
	UFUNCTION()
	void RotateVertical(float AxisValue);





protected:
	//이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float MoveSpeed = 20.0f;

	//회전속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float RotateSpeed = 3.0f;

	//CameraBounds 함수에서 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float RotatePitchMin = 10.0f;

	//CameraBounds 함수에서 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float RotatePitchMax = 80.0f;

	//줌 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float ZoomSpeed = 5.0f;

	//최소 줌
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float ZoomMin = 100.0f;

	//최대 줌
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float ZoomMax = 300.0f;

private:
	//카메라 떨림 방지
	UFUNCTION()
	void CameraBounds();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", Category = "Root"))
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Root")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Root")
	UCameraComponent* CameraComponent;

	//폰 위치값
	UPROPERTY()
	FVector TargetLocation;

	//폰 회전값
	UPROPERTY()
	FRotator TargetRotation;

	//플레이어 스타트 지점으로 이동시 저장한 위치값
	UPROPERTY()
	FVector SaveLocation;

	//플레이어 스타트 지점으로 이동시 저장한 회전값
	UPROPERTY()
	FRotator SaveRotation;

	//줌 변수
	UPROPERTY()
	float TargetZoom;

	//회전 상태 체크
	UPROPERTY()
	bool CanRotate;

	UPROPERTY()
	int32 CurrentTartgetIndex;

	TArray<AActor*> FocusedActor;
	


};
