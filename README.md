![](https://github.com/NeuralVFX/basic_libtorch_dll/blob/master/coke.png)
# Unreal-Omnidirectional-Stereo-Camera

This plugin adds a new Camera Class to Unreal which can be used to render Omnidirectional Stereo Sequences. 

## About

- Rather than output full images directly, this outputs the array of images plus a TXT file required to stitch using `PTStitcher` or `PTMender`, similar to what Ansel does behind the scenes.

- This is not for Game snapshots, however instead for recording a scene which is pre-animated with the Sequencer (Useful for VR or 360 Rides where the camera might take a predefined path)

- This automatically takes rotation of the camera(Pitch Roll and Yaw) into account

- Uses LevelSequencer to automatically set Fixed FrameRate, and output files with exact frame numbers

## Plugin Usage
Usage instructions found here: [user manual page](USAGE.md).
