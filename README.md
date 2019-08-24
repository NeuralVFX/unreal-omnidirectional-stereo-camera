![](Images/example_a.jpg)
# Unreal-Omnidirectional-Stereo-Camera

This plugin adds a new Camera Class to Unreal which can be used to render Omnidirectional Stereo Images. 

## About

- Rathen than output full images directly, this outputs the array of images plus a TXT file required to stitch using "PTStitcher" or "PTMender", similar to what Ansel does behind the scenes.

- This is not for Game snapshots, however instead for recordering a scene which is pre-animated with the Sequencer (Usefull for VR or 360 Rides)

- This automatically takes rotation of the camera(Pitch Roll and Yaw) into account

- Uses LevelSequencer to automatically set Fixed Framerate, and output files with exact framenumbers

## Plugin Usage
Usage instructions found here: [user manual page](USAGE.md).
