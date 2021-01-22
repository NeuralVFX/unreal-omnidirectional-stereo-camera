// Copyright 2019 Charlie Winter, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "UObject/ObjectMacros.h"
#include "ODSCamera.generated.h"

/**
* Camera Actor designed to render Omnidirectional Stereo,
* taking sequencer driven position and rotation into account.
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stereo Settings")
	bool bInterocularVerticalFade;

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

	/**
	* Build an individual capture component with proper orientation and placement.
	* @param YawStep - Count of current Yaw step, determines horizontal orientation.
	* @param PitchStep - Count of current Pitch step, determines vertical orientation.
	* @param Eye - Which eye this camera belongs to, "Left" or "Right", determines horizontal offset.
	* @return Capture Component which was created.
	*/
	USceneCaptureComponent2D* BuildCaptureComponent(int32 YawStep, int32 PitchStep, FString Eye);

	/** 
	* Renders an individual capture component.
	* @param YawStep - Count of current Yaw step, used in name of output png file.
	* @param PitchStep - Count of current Pitch step, used in name of output png file.
	* @param Eye - Which eye this camera belongs to, "Left" or "Right", used in name of output sticher text file. 
	* @param CaptureComponent - A Capture component to render.
	*/
	void RenderComponent(int32 YawStep, int32 PitchStep, FString Eye, USceneCaptureComponent2D* CaptureComponent);

	/** 
	* Uses step info and orientation from FRotator to create a single line of information formatted for PTStitcher.
	* @param Rotation - Orientation of current render.
	* @param YawStep - Count of current Yaw step, used in name of output sticher text file.
	* @param PitchStep - Count of current Pitch step, used in name of output sticher text file.
	* @return String containing single line of sticher file.
	*/
	FString WriteStitcherLine(FRotator Rotation, int32 YawStep, int32 PitchStep);

	/** 
	* Write entire PTStitcher TXT file
	* @param Eye - Whether to write left or right eye.
	*/
	void WriteStitcherFile(FString Eye);

	/** 
	* Double check that a valid sequencer is selected on the object.
	* @return bool describing whether or not there is a valid Sequencer selected.
	*/
	bool CheckSequencer();

	/** Loop through all Yaw and Pitch steps and create array of CaptureComponents */
	void BuildCaptureArray();

	/** 
	*  Determine What Yaw And Pitch We Are At From The Step.
	* @param Step - Count of which step we are on.
	* @param YawStep - The calculated count of current Yaw step.
	* @param PitchStep - The calculated count of current Pitch step.
	*/
	void GetComponentSteps(int32 Step, int32& YawStep, int32& PitchStep);


};




