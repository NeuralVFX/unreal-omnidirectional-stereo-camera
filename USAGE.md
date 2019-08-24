
# Getting Started
## Project Setup

- Make a C++ Unreal Project 
- Make sure you have a `Plugins` folder inside of the project
- Clone this Repo into the plugins folder (Or download the zip file and unzip there)
- Right click your `.uproject` file and click `Generate Visual Studio Project Files`
- Compile the project from Visual Studio


## Scene Setup

- From Edit->Plugins, locate the "Installed" Tab
- Find  `Omni-Directional Stereo Camera` and click `Enable`
- In the Content Manager, navigate to ODSPlugin C++ `Classes->ODSPlugin->Classes`
- Click on `ODSCamera` and drag this into your scene file
- In the attributes of the ODSCamera, find the `LevelSequencer` attribute and select the sequencer you would like to render from
- Apply any camera animation you want within this level sequencer
- In the attributes of the ODSCamera, set the `Frame Range`, `Output Settings`, and `Stereo Settings` to your needs
- Play the level!
- No need to setup anything to play your sequencer, the sequencer selected in the node will play automatically when you play the level

## ODSCamera Attributes

### Frame Range
```
--Start Frame, default=100, type=int                           # Frame to start capturing from
--End Frame, default=105, type=int                             # Frame to end capturing
--Current Frame, default=0, type=int                           # Current frame in Sequencer, used during capture
```
### Output Settings
```
--Out Resolution Width, default=4096, type=int                 # Width of final output, height will be half this value (Written into PTStitcher TXT file)
--Snip Resolution, default=720, type=int                       # Width and height used for each snapshot
--Snip FOV, default=50.0, type=float                           # Camera FOV for each snaphot, should be wider than "Angle Increment"
--Angle Increment, default=36.0, type=float                    # Rotation between each snapshot
--Output Dir, default='C:/Output/', type=str                   # Output directory for images and TXT files
--Output Name, default='Render Test', type=str                 # File prefix for all output files
```
### Stereo Settings
```
--Interocular Distance, default=6.2, type=float                # Distance between the left and right eye
--Interocular Verticle Fade, default=false, type=bool          # Whether to decrease the Interoclar Distance at top and bottom of frame (Reduces twist artifact)
```
### Sequencer
```
--Level Sequencer, default=None, type=LevelSequencer           # Which Level Sequencer to play, also fixed framerate is set based on this
```
