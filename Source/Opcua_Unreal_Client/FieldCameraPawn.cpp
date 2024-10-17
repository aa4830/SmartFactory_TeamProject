#include "FieldCameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "TreeViewStruct.h"
#include "UTreeViewObject.h"
#include "SMainWidget.h"
#include <ActorData.cpp>

// Sets default values
//컴포넌트
AFieldCameraPawn::AFieldCameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	UE_LOG(LogTemp, Warning, TEXT("AFieldCameraPawnSPawn"));
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 200.0f;
	SpringArmComponent->bDoCollisionTest = false;


	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	


}

// Called every frame
//선형 보간 
void AFieldCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraBounds();
	const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(InterpolatedLocation);

	const float InterpolatedZoom = UKismetMathLibrary::FInterpTo(SpringArmComponent->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	SpringArmComponent->TargetArmLength = InterpolatedZoom;

	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(SpringArmComponent->GetRelativeRotation(), TargetRotation, DeltaTime, RotateSpeed);
	SpringArmComponent->SetRelativeRotation(InterpolatedRotation);
}

// Called when the game starts or when spawned
//시작시 플레이어 스타트 위치값회전값 저장
void AFieldCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	if (PlayerStart)
	{
		SaveLocation = PlayerStart->GetActorLocation(); // 플레이어 스타트 위치 저장

		const FRotator Rotation = SpringArmComponent->GetRelativeRotation();
		SaveRotation = FRotator(Rotation.Pitch + -50, Rotation.Yaw, 0.0f); // 플레이어 스타트 회전 저장

		TargetLocation = SaveLocation; // 초기 위치 설정
		TargetRotation = SaveRotation; // 초기 회전 설정
		TargetZoom = 200.0f;
	}
	
}



// Forward and Right movement
//W,S 로도 줌기능
void AFieldCameraPawn::Forward(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Forward:%f"), AxisValue);
	TargetLocation = SpringArmComponent->GetForwardVector() * AxisValue * MoveSpeed + TargetLocation;

}
//D,A 45도 회전기능
void AFieldCameraPawn::Right(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Right:%f"), AxisValue);
	//TargetLocation = SpringArmComponent->GetRightVector() * AxisValue * MoveSpeed + TargetLocation;
	TargetRotation = UKismetMathLibrary::ComposeRotators(SpringArmComponent->GetRelativeRotation(), FRotator(0.0f, AxisValue * -45.0f, 0.0f));

	// 회전 상태 활성화
	CanRotate = true;
}

// Zoom functionality
//마우스 휠 줌
void AFieldCameraPawn::Zoom(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}
	const float Zoom = AxisValue * 100.0f;
	TargetZoom = FMath::Clamp(Zoom + TargetZoom, ZoomMin, ZoomMax);
}

// Rotation functionality
//E눌르면 45도 회전
void AFieldCameraPawn::RotateRight()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, 45.0f, 0.0f));

}
//Q눌르면 -45도 회전
void AFieldCameraPawn::RotateLeft()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, -45.0f, 0.0f));
}

//회전상태 체크
void AFieldCameraPawn::EnableRotate()
{
	CanRotate = true;
}
//회전상태 체크
void AFieldCameraPawn::DisableRotate()
{
	CanRotate = false;
}

//마우스 수평 수직 제한이지만 필요없어서 주석처리함 안쓰는기능
void AFieldCameraPawn::RotateHorizontal(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}
	if (CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, AxisValue, 0.0f));
	}
}

void AFieldCameraPawn::RotateVertical(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}
	if (CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(AxisValue, 0.0f, 0.0f));
	}
}

// Restrict camera pitch and location bounds
//카메라 떨림 방지
void AFieldCameraPawn::CameraBounds()
{
	float NewPitch = TargetRotation.Pitch;
	if (TargetRotation.Pitch < (RotatePitchMax * -1))
	{
		NewPitch = (RotatePitchMax * -1);
	}
	else if (TargetRotation.Pitch > (RotatePitchMin * -1))
	{
		NewPitch = (RotatePitchMin * -1);
	}
	TargetRotation = FRotator(NewPitch, TargetRotation.Yaw, 0.0f);
	FVector(TargetLocation.X, TargetLocation.Y, 0.0f);
}



//Z 눌르면 모든 액터 아웃라인하이라이트 해제
void AFieldCameraPawn::PowerOff()
{

	// 먼저 월드의 모든 액터의 CustomDepth를 false로 설정
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		TArray<UPrimitiveComponent*> Components;
		Actor->GetComponents<UPrimitiveComponent>(Components);
		for (UPrimitiveComponent* PrimitiveComponent : Components)
		{
			if (PrimitiveComponent)
			{
				PrimitiveComponent->SetRenderCustomDepth(false);
			}
		}
	}
}

//FocusOnAllBPTestC함수 에서 받은 액터들 아웃라인 하이라트적용 및 액터들 사이 거리 중심으로 이동 
void AFieldCameraPawn::FocusOnActors(TArray<AActor*>& TargetActors)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("FocusOnActors")));
	PowerOff();
	if (TargetActors.Num() > 0)
	{
		FVector CenterLocation = FVector::ZeroVector;
		int32 ActorCount = 0;

		for (AActor* TargetActor : TargetActors)
		{
			if (TargetActor)
			{
				CenterLocation += TargetActor->GetActorLocation();
				ActorCount++;

				// BP_Test_C를 포함한 액터만 CustomDepth를 true로 설정
				TArray<UPrimitiveComponent*> Components;
				TargetActor->GetComponents<UPrimitiveComponent>(Components);
				for (UPrimitiveComponent* PrimitiveComponent : Components)
				{
					if (PrimitiveComponent)
					{
						PrimitiveComponent->SetRenderCustomDepth(true);
					}
				}
			}
		}

		if (ActorCount > 0)
		{
			CenterLocation /= ActorCount;
		}
		TargetLocation = CenterLocation;
	}
	ResetFocus();
}



//FocusOnBPTestC1 받은 액터이름 아웃라인적용및 해당 액터로이동
void AFieldCameraPawn::FocusOnActor2(AActor* TargetActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("FocusOnActor2")));
	PowerOff();
	if (TargetActor)
	{
		TArray<UPrimitiveComponent*> Components;
		TargetActor->GetComponents<UPrimitiveComponent>(Components);
		for (UPrimitiveComponent* PrimitiveComponent : Components)
		{
			if (PrimitiveComponent)
			{
				PrimitiveComponent->SetRenderCustomDepth(true);
			}
		}
		TargetLocation = TargetActor->GetActorLocation();
	}

	ResetFocus();
}


//1번키 누르면 BP_Test_C 포함한 액터 찾기
void AFieldCameraPawn::FocusOnAllBPTestC(int32 InIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("InIndex ; %d"), InIndex));
	/*TArray<AActor*> FoundActors;
	FoundActors.Empty();
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->GetName().Contains(TEXT("BP_Test_C")))
		{
			FoundActors.Add(*It);
		}
	}
	FocusOnActors(FoundActors);*/
	TArray<AActor*> AllActors;
	TArray<AActor*> FoundActors;
	TArray<AActor*> TaggedActors;
	AActor* SpecificActor = nullptr;
	FName TargetTag = TEXT("Name");
	FoundActors.Empty();
	TaggedActors.Empty();
	

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("HHH %s"), &InName));
	UGameplayStatics::GetAllActorsOfClass(GWorld, AActorData::StaticClass(), AllActors);
	
	for (auto TargetActor : AllActors)
	{
		if (TargetActor->ActorHasTag(TargetTag))
		{
			TaggedActors.Add(TargetActor);
		}

		if (Cast<AActorData>(TargetActor)->TargetIndex == InIndex)
		{
			//FoundActors.Add(TargetActor);
			SpecificActor = TargetActor;
			
		}
		
	}
	if (TaggedActors.Num() > 0 && InIndex == 0)
	{
		FocusOnActors(TaggedActors);
	}
	if (SpecificActor)
	{
		FocusOnActor2(SpecificActor);
	}
	CurrentTartgetIndex = InIndex;
	ResetFocus();
}



//2번키 누르면 BP_Test_C_1 이름을 가진 액터 찾기
void AFieldCameraPawn::FocusOnBPTestC1()
{
	AActor* SpecificActor = nullptr;
	FName TargetTag = TEXT("Test");
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found actor: %s"), *It->GetName());  // 모든 액터 이름 출력
		AActor* Actor = *It;
		// 액터가 해당 태그를 가지고 있는지 확인
		if (Actor->ActorHasTag(TargetTag))
		{
			SpecificActor = Actor;
			break;
		}
	}
	
	if (SpecificActor)
	{
		FocusOnActor2(SpecificActor);
	}
}

void AFieldCameraPawn::ResetFocus()
{
	FocusedActor.Empty();
	CurrentTartgetIndex = 0;
}





//F 누르면 플레이어 스타트 지점으로 이동
void AFieldCameraPawn::GoPlayerStartposition()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("GoPlayerStartposition")));
	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	if (PlayerStart)
	{
		PowerOff();
		TargetLocation = SaveLocation; // 플레이어 스타트 위치로 이동
		TargetRotation = SaveRotation; // 플레이어 스타트 회전으로 회전
		UE_LOG(LogTemp, Warning, TEXT("Moved to Player Start at: %s"), *TargetLocation.ToString());
	}

}
// Called to bind functionality to input
void AFieldCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AFieldCameraPawn::Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AFieldCameraPawn::Right);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AFieldCameraPawn::Zoom);
	PlayerInputComponent->BindAxis(TEXT("RotateHorizontal"), this, &AFieldCameraPawn::RotateHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateVertical"), this, &AFieldCameraPawn::RotateVertical);

	PlayerInputComponent->BindAction(TEXT("RotateRight"), IE_Pressed, this, &AFieldCameraPawn::RotateRight);
	PlayerInputComponent->BindAction(TEXT("RotateLeft"), IE_Pressed, this, &AFieldCameraPawn::RotateLeft);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Pressed, this, &AFieldCameraPawn::EnableRotate);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Released, this, &AFieldCameraPawn::DisableRotate);
	//PlayerInputComponent->BindAction(TEXT("FocusOnActor"), IE_Pressed, this, &AFieldCameraPawn::FocusOnTarget);
	//PlayerInputComponent->BindAction(TEXT("FocusOnActors"), IE_Pressed, this, &AFieldCameraPawn::FocusOnAllBPTestC(FString InName));
	PlayerInputComponent->BindAction(TEXT("FocusOnActor2"), IE_Pressed, this, &AFieldCameraPawn::FocusOnBPTestC1);
	//PlayerInputComponent->BindAction(TEXT("FocusOnActor3"), IE_Pressed, this, &AFieldCameraPawn::FocusOnBPTestC0);
	PlayerInputComponent->BindAction(TEXT("PowerOff"), IE_Pressed, this, &AFieldCameraPawn::PowerOff);
	PlayerInputComponent->BindAction(TEXT("GoPlayerStartposition"), IE_Pressed, this, &AFieldCameraPawn::GoPlayerStartposition);
}




