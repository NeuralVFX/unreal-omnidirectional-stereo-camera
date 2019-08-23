// Copyright 2019 Charlie Winter, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "UObject/ObjectMacros.h"
#include "ODSCamera.generated.h"

/**
 *
 */
UCLASS()
class ODSPLUGIN_API AODSCamera : public ACineCameraActor
{
	GENERATED_BODY()
public:

	AODSCamera(const FObjectInitializer& ObjectInitializer);

	/** Frame Range To Render */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame Range")
	int32 StartFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame Range")
	int32 EndFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Frame Range")
	int32 CurrentFrame;

	/** Output Resolution Written Into PTStitcher TXT File */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output Settings")
	int32 OutResolutionWidth;

	/** Settings For Capture Component Array */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output Settings")
	int32 SnipResolution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output Settings")
	float SnipFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output Settings")
	float AngleIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stereo Settings")
	float InterocularDistance;

	/** Output File Path Information */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output Settings")
	FString OutputDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output Settings")
	FString OutputName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequencer")
	class ALevelSequenceActor* LevelSequencer;

	/** CaptureComponent Arrays To Be Populated At Runtime */
	TArray<class USceneCaptureComponent2D*> LeftCaptureComponentArray;
	TArray<class USceneCaptureComponent2D*> RightCaptureComponentArray;

	/** Variables To Help Iterate Through CaptureComponent Array*/
	int32 YawStepCount;
	int32 PitchStepCount;
	int32 TotalSteps;

	/** Frame Rate Extracted From Sequencer */
	int32 FrameRate;

	/** PTStitcher TXT File Template, Rebuilt Everytime CaptureComponent Array Rebuilds */
	FString PTStitcherText;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	/** Each Capture Component Added To The Array is Built Here */
	USceneCaptureComponent2D* BuildCaptureComponent(int32 YawStep, int32 PitchStep, FString Eye);

	/** Renders An Individual Capture Component */
	void RenderComponent(int32 YawStep, int32 PitchStep, FString Eye, USceneCaptureComponent2D* CaptureComponent);

	/** Write A Single Line Of PTStitcher TXT FIle */
	FString WriteStitcherLine(FRotator Rotation, int32 YawStep, int32 PitchStep);

	/** Write Entire PTStitcher TXT File */
	void WriteStitcherFile(FString Eye);

	/** Double Check That A Valid Seuquencer Is Selected */
	bool CheckSequencer();

	/** Populate CaptureComponentArray */
	void BuildCaptureArray();

	/** Determine What Yaw And Pitch We Are At From The Step */
	void GetComponentSteps(int32 Step, int32& YawStep, int32& PitchStep);


};




