// Fill out your copyright notice in the Description page of Project Settings.


#include "SOptionWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/ComboBoxString.h"

void USOptionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PopulateResolutionComboBox();

    Button_close->OnClicked.AddDynamic(this, &USOptionWidget::OnClickedclose);

    // 콤보박스에서 선택이 변경될 때의 델리게이트 설정
    ComboBox_Resolution->OnSelectionChanged.AddDynamic(this, &USOptionWidget::OnResolutionChanged);

    if (ComboBox_ScreenMode)
    {
        ComboBox_ScreenMode->AddOption(TEXT("Fullscreen"));
        ComboBox_ScreenMode->AddOption(TEXT("Windowed"));
        ComboBox_ScreenMode->AddOption(TEXT("Windowed Fullscreen"));

        // 콤보박스 값이 변경될 때 호출될 함수를 바인딩
        ComboBox_ScreenMode->OnSelectionChanged.AddDynamic(this, &USOptionWidget::OnScreenModeChanged);

        // 초기 선택값 설정 (예: 현재 설정된 화면 모드에 맞춰서)
        UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
        if (UserSettings)
        {
            switch (UserSettings->GetFullscreenMode())
            {
            case EWindowMode::Fullscreen:
                ComboBox_ScreenMode->SetSelectedOption(TEXT("Fullscreen"));
                break;
            case EWindowMode::Windowed:
                ComboBox_ScreenMode->SetSelectedOption(TEXT("Windowed"));
                break;
            case EWindowMode::WindowedFullscreen:
                ComboBox_ScreenMode->SetSelectedOption(TEXT("Windowed Fullscreen"));
                break;
            }
        }
    }
}

void USOptionWidget::PopulateResolutionComboBox()
{
    // 예시 해상도 옵션을 추가

    FResolutionOption Res1;
    Res1.ResolutionLabel = TEXT("1920x1080");
    Res1.ResolutionValue = FVector2D(1920, 1080);
    ResolutionOptions.Add(Res1);

    FResolutionOption Res2;
    Res2.ResolutionLabel = TEXT("1280x720");
    Res2.ResolutionValue = FVector2D(1280, 720);
    ResolutionOptions.Add(Res2);

    FResolutionOption Res3;
    Res3.ResolutionLabel = TEXT("800x600");
    Res3.ResolutionValue = FVector2D(800, 600);
    ResolutionOptions.Add(Res3);

    // 콤보박스에 해상도 추가
    if (ComboBox_Resolution)
    {
        for (const FResolutionOption& ResOption : ResolutionOptions)
        {
            ComboBox_Resolution->AddOption(ResOption.ResolutionLabel);
        }
    }
}

void USOptionWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    // 선택된 해상도에 맞는 ResolutionOption을 찾음
    for (const FResolutionOption& ResOption : ResolutionOptions)
    {
        if (ResOption.ResolutionLabel == SelectedItem)
        {
            // 선택된 해상도 적용
            ApplyResolution(ResOption.ResolutionValue);
            break;
        }
    }
}

void USOptionWidget::ApplyResolution(FVector2D Resolution)
{
    // UGameUserSettings를 통해 해상도 설정 적용
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (UserSettings)
    {
        // 해상도 설정
        UserSettings->SetScreenResolution(FIntPoint(Resolution.X, Resolution.Y));

        // 변경 사항 적용
        UserSettings->ApplySettings(false);
    }

}

void USOptionWidget::ApplyScreenMode(const FString& SelectedMode)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (UserSettings)
    {
        if (SelectedMode == "Fullscreen")
        {
            UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
        }
        else if (SelectedMode == "Windowed")
        {
            UserSettings->SetFullscreenMode(EWindowMode::Windowed);
        }
        else if (SelectedMode == "Windowed Fullscreen")
        {
            UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
        }

        // 설정 적용
        UserSettings->ApplySettings(false);
    }
}

void USOptionWidget::OnClickedclose()
{
    SetVisibility(ESlateVisibility::Hidden);
}
void USOptionWidget::OnScreenModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    ApplyScreenMode(SelectedItem);
};