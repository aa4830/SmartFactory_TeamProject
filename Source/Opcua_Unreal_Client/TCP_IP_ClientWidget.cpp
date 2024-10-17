// Fill out your copyright notice in the Description page of Project Settings.


#include "TCP_IP_ClientWidget.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Components/TextBlock.h"

// 정적 변수 초기화
FMyStruct UTCP_IP_ClientWidget::ParsedData;

void UTCP_IP_ClientWidget::NativeConstruct()
{
    Super::NativeConstruct();
	Socket = nullptr;
    ConnectToServer();
}

void UTCP_IP_ClientWidget::NativeDestruct()
{
    Super::NativeDestruct();
    CloseSocket();
}

void UTCP_IP_ClientWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    ReceiveData();  // 매 프레임마다 데이터 수신 확인
}

void UTCP_IP_ClientWidget::ConnectToServer()
{
    FString ServerAddress = TEXT("127.0.0.1"); // 서버 IP
    int32 Port = 10990; // 포트 번호
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

    // 서버 주소 설정
    TSharedRef<FInternetAddr> Addr = SocketSubsystem->CreateInternetAddr();
    bool bIsValid;
    Addr->SetIp(*ServerAddress, bIsValid);
    Addr->SetPort(Port);

    // IP 주소 유효성 확인
    if (!bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid IP address"));
        return;
    }

    // 소켓 생성
    Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("DefaultSocket"), false);
    if (Socket == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create socket"));
        return;
    }

    // 서버에 연결 시도
    bool bConnected = Socket->Connect(*Addr);
    if (bConnected)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully connected to server"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect to server"));
    }
}

void UTCP_IP_ClientWidget::ReceiveJsonData(const FString& JsonString)
{
    // JSON 파싱
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

    // JSON 파싱 성공 여부 확인
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        // "MyStruct" 객체 가져오기
        TSharedPtr<FJsonObject> MyStruct = JsonObject->GetObjectField(TEXT("MyStruct"));
        if (MyStruct.IsValid())  // MyStruct 유효성 확인
        {
            // State 값을 가져와 ParsedData에 저장
            ParsedData.State = MyStruct->GetStringField(TEXT("State"));
            // Temperature 단일 값 가져오기
            ParsedData.TemperatureValue = MyStruct->GetNumberField(TEXT("Temperature"));

            // State와 Temperature 값만 로그 출력
            LogParsedData();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("MyStruct is invalid or missing"));
        }
    }
    else
    {
        // JSON 파싱 실패 시 더 많은 정보 제공
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON: %s"), *JsonString);
    }
}

void UTCP_IP_ClientWidget::ReceiveData()
{
    if (Socket && Socket->GetConnectionState() == SCS_Connected)
    {
        uint32 DataSize;
        if (Socket->HasPendingData(DataSize))
        {
            TArray<uint8> ReceivedData;
            ReceivedData.SetNumUninitialized(DataSize);

            int32 BytesRead = 0;
            if (Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), BytesRead))
            {
                // 수신된 데이터를 문자열로 변환
                FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));

                // 수신한 문자열 정리
                if (ReceivedString.EndsWith(TEXT("\n"))) {
                    ReceivedString = ReceivedString.Left(ReceivedString.Len() - 1);
                }

                // 추가 정리 (예: 특정 특수문자 제거)
                ReceivedString.RemoveFromEnd(TEXT("\r")); // 캐리지 리턴 제거

                // 수신한 문자열을 JSON으로 처리
                ReceiveJsonData(ReceivedString);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to read from socket"));
            }
        }
    }
}

void UTCP_IP_ClientWidget::LogParsedData()
{
	// State와 Temperature 값만 로그 출력
	UE_LOG(LogTemp, Log, TEXT("State: %s"), *ParsedData.State);
	UE_LOG(LogTemp, Log, TEXT("Temperature: %.1f"), ParsedData.TemperatureValue);

    TCPData_1->SetText(FText::FromString(*ParsedData.State));
    TCPData_2->SetText(FText::AsNumber(ParsedData.TemperatureValue));
}

void UTCP_IP_ClientWidget::CloseSocket()
{
    // ClientSocket이 nullptr가 아니고, 유효한 소켓 타입인지 확인
    if (Socket != nullptr && Socket->GetSocketType() != ESocketType::SOCKTYPE_Unknown)
    {
        Socket->Close(); // 소켓 닫기
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket); // 소켓 리소스 해제
        Socket = nullptr; // 소켓 포인터를 null로 설정하여 메모리 누수 방지
        UE_LOG(LogTemp, Log, TEXT("Socket closed successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Socket is already closed or not initialized."));
    }
}
