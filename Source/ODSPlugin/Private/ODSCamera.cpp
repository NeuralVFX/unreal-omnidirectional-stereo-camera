// Copyright 2019 Charlie Winter, Inc. All Rights Reserved.

#include "ODSCamera.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Camera/CameraComponent.h"


AODSCamera::AODSCamera(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FrameRate = 48;
	StartFrame = 100;
	EndFrame = 105;
	CurrentFrame = 0;

	SnipResolution = 720;
	SnipFOV = 50.f;
	AngleIncrement = 36.f;

	InterocularDistance = 6.2f;
	bInterocularVerticalFade = false;

	OutResolutionWidth = 4096;
	OutputDir = "C:/Output/";
	OutputName = "RenderTest";
}


bool AODSCamera::CheckSequencer()
{
	//Check if a Sequencer is Selected
	bool SequencerIsGood = false;

	if (LevelSequencer != nullptr)
	{
		if (LevelSequencer->SequencePlayer != nullptr)
		{
			SequencerIsGood = true;
		}
	}
	return SequencerIsGood;
}


void AODSCamera::BeginPlay()
{
	Super::BeginPlay();

	// Check for Level Sequencer Before Attempting to Play
	if (CheckSequencer())
	{
		// Begin Playing, Set Unreal to Fixed Frame Rate Based on Sequencer FPS
		LevelSequencer->SequencePlayer->Play();

		FrameRate = LevelSequencer->SequencePlayer->GetFrameRate().Numerator;
		FApp::SetFixedDeltaTime(1.0 / (float)FrameRate);
		FApp::SetUseFixedTimeStep(true);
		UE_LOG(LogTemp, Warning, TEXT("Taking Frame Rate From Sequencer: %02d"), FrameRate);

		// Build Array Of Capture Components
		BuildCaptureArray();
	}
}


void AODSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int32 YawStep= 0;
	int32 PitchStep = 0;

	// Only Attempt to Render if Level Sequencer is Intact
	if (CheckSequencer())
	{
		// Set Time Variable to Time From Sequencer
		float Time = LevelSequencer->SequencePlayer->GetCurrentTime().AsSeconds();
		CurrentFrame = (int32)(Time*(float)FrameRate);

		// If Time Within Render Range, Render
		if (CurrentFrame >= StartFrame && CurrentFrame <= EndFrame)
		{
			// Loop Through Every Capture Component and Render
			UE_LOG(LogTemp, Warning, TEXT("Writing Frame: %03d"), CurrentFrame);
			FDateTime StartTime = FDateTime::UtcNow();
			for (int i = 0; i < TotalSteps; i++)
			{
				GetComponentSteps(i, YawStep, PitchStep);
				RenderComponent(YawStep, PitchStep, FString("Left"), LeftCaptureComponentArray[i]);
				RenderComponent(YawStep, PitchStep, FString("Right"), RightCaptureComponentArray[i]);
			}

			FDateTime EndTime = FDateTime::UtcNow();
			FTimespan Duration = EndTime - StartTime;
			UE_LOG(LogTemp, Warning, TEXT("Duration: %g seconds for frame %03d"), Duration.GetTotalSeconds(), CurrentFrame);

			// Write TXT file for PTStitcher
			WriteStitcherFile(FString("Left"));
			WriteStitcherFile(FString("Right"));
		}
	}
}


void AODSCamera::GetComponentSteps(int32 Step, int32& YawStep, int32& PitchStep)
{
	YawStep= Step / PitchStepCount;
	PitchStep = Step - (YawStep* PitchStepCount);
}


void AODSCamera::BuildCaptureArray()
{
	// Determine How Many Components We Need
	YawStepCount = (int32)(360.0f / AngleIncrement);
	PitchStepCount = (int32)(180.0f / AngleIncrement);
	TotalSteps = YawStepCount * PitchStepCount;

	// Write Header of TXT file for PTStitcher
	PTStitcherText = FString::Printf(TEXT("p f2 w%d h%d v360  n\"TIFF\"")LINE_TERMINATOR, OutResolutionWidth, OutResolutionWidth / 2);

	// Loop Through and Create All Capture Components
	int32 YawStep= 0;
	int32 PitchStep = 0;
	for (int i = 0; i < TotalSteps; i++)
	{
		GetComponentSteps(i, YawStep, PitchStep);
		LeftCaptureComponentArray.Add(BuildCaptureComponent(YawStep, PitchStep, FString("Left")));
		RightCaptureComponentArray.Add(BuildCaptureComponent(YawStep, PitchStep, FString("Right")));
	}
}


FString AODSCamera::WriteStitcherLine(FRotator Rotation, int32 YawStep, int32 PitchStep)
{
	// Format Filename
	FString FileString = FString::Printf(TEXT("%s_%03d_%03d.$FRAMECOUNT$.png"), *OutputName, YawStep, PitchStep);
	// Format All of the Arguments Required to Add an Image to a PTStitcher TXT file
	TArray< FStringFormatArg > args;
	args.Add(FStringFormatArg(180 - Rotation.Roll));
	args.Add(FStringFormatArg(Rotation.Yaw - 180));
	args.Add(FStringFormatArg(180 - Rotation.Pitch));
	args.Add(SnipFOV);
	args.Add(SnipResolution);
	args.Add(SnipResolution);
	args.Add(FileString);
	args.Add(LINE_TERMINATOR);
	FString OutString = FString::Format(TEXT("o f0 w{4} h{5} n\"{6}\" r{0} p{2} y{1} v{3}{7}"), args);
	return OutString;
}


void AODSCamera::WriteStitcherFile(FString Eye)
{
	// Format FilePath
	FString PTSticherFileName = FString::Printf(TEXT("%s.%05d.txt"), *OutputName, CurrentFrame);
	FString SticherPath = OutputDir / Eye / PTSticherFileName;

	// Replace Frame Number
	FString FrameChar = FString::Printf(TEXT("%05d"), CurrentFrame);
	FString OutPTStitcherText = PTStitcherText;
	OutPTStitcherText = OutPTStitcherText.Replace(TEXT("$FRAMECOUNT$"), *FrameChar);

	// Save File
	FFileHelper::SaveStringToFile(OutPTStitcherText, *SticherPath, FFileHelper::EEncodingOptions::ForceAnsi);
}


USceneCaptureComponent2D* AODSCamera::BuildCaptureComponent(int32 YawStep, int32 PitchStep, FString Eye)
{
	// Build Capture Component
	FString CamName = FString::Printf(TEXT("Cam_%s_%03d_%03d"), *Eye, YawStep, PitchStep);
	USceneCaptureComponent2D* CaptureComponent = NewObject<USceneCaptureComponent2D>(this, *CamName);
	CaptureComponent->RegisterComponent();

	CaptureComponent->FOVAngle = SnipFOV;
	CaptureComponent->bCaptureEveryFrame = false;
	CaptureComponent->bCaptureOnMovement = false;
	CaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

	// Build Texture Target
	FString TexName = FString::Printf(TEXT("Tex_%s_%03d_%03d"), *Eye, YawStep, PitchStep);
	CaptureComponent->TextureTarget = NewObject<UTextureRenderTarget2D>(this, *TexName);
	CaptureComponent->TextureTarget->InitCustomFormat(SnipResolution, SnipResolution, PF_A16B16G16R16, false);
	CaptureComponent->AttachTo(GetRootComponent());

	// Setup Rotation
	FRotator Rotation = FRotator((90.0f - (.5f*AngleIncrement)) - 1.0f * PitchStep * AngleIncrement, 180.0f + YawStep * AngleIncrement, 0);
	Rotation = Rotation.Clamp();
	FVector Offset(0.0f, InterocularDistance / 2.0f, 0.0f);

	float InterocularDistMult = 1.f;

	// Setup Stereo Offset, Fading to Zero At Top and Bottom
	if (bInterocularVerticalFade)
	{
		InterocularDistMult = (fabs(180.f - Rotation.Pitch) - 90.f) / 90.f;
	}
	// Rotate Offset Vector
	Offset = Rotation.RotateVector(Offset * InterocularDistMult);

	float OffsetMult = 1.f;
	if (Eye == "Left")
	{
		OffsetMult = -1.f;
		// Add Line Representing this Component to PTStitcher TXT file
		PTStitcherText += WriteStitcherLine(Rotation, YawStep, PitchStep);
	}
	CaptureComponent->SetRelativeLocationAndRotation(OffsetMult * Offset, Rotation);
	return CaptureComponent;
}


void AODSCamera::RenderComponent(int32 YawStep, int32 PitchStep, FString Eye, USceneCaptureComponent2D* CaptureComponent)
{
	// Copy Post Processing Settings
	FPostProcessSettings PPS;
	UCameraComponent* MainCam = GetCameraComponent();
	PPS = MainCam->PostProcessSettings;
	CaptureComponent->PostProcessSettings = PPS;
	// Capture
	CaptureComponent->CaptureScene();

	// Setup Output
	EImageFormat Eformat = EImageFormat::PNG;
	FString SnipFileName = FString::Printf(TEXT("%s_%03d_%03d.%05d"), *OutputName, YawStep, PitchStep, CurrentFrame);
	FString CaptureName = OutputDir / Eye / SnipFileName + TEXT(".png");

	// Make Image Wrapper
	IImageWrapperModule* ImageWrapperModule = FModuleManager::LoadModulePtr<IImageWrapperModule>("ImageWrapper");
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule->CreateImageWrapper(Eformat);

	// Get Data From Texture Target
	TArray<FColor> SnipImage;
	SnipImage.AddUninitialized(SnipResolution* SnipResolution);
	FTextureRenderTargetResource* RenderTarget = CaptureComponent->TextureTarget->GameThread_GetRenderTargetResource();

	RenderTarget->ReadPixelsPtr(SnipImage.GetData(), FReadSurfaceDataFlags());
	ImageWrapper->SetRaw(SnipImage.GetData(), SnipImage.GetAllocatedSize(), SnipResolution, SnipResolution, ERGBFormat::BGRA, 8);
	const TArray64<uint8> PNGData = ImageWrapper->GetCompressed(100);

	// Write Output
	FFileHelper::SaveArrayToFile(PNGData, *CaptureName);
	ImageWrapper.Reset();
}