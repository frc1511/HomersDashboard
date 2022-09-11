# Homer's Dashboard
Dashboard application for Homer, FRC Team 1511's favorite robot.

## Table of Contents
* [App Usage](#app-usage)
  - [Camera Streams](#camera-streams)
  - [Network Tables](#network-tables)
  - [Auto Chooser](#auto-chooser)
  - [Blinky Blinky](#blinky-blinky)
  - [Robot Position](#robot-position)
  - [Motion Profile](#motion-profile)
* [Building](#building)

## App Usage
To make the driver station open the dashboard, edit the "DashboardCmdLine" entry in the "C:\Users\Public\Documents\FRC\FRC DS Data Storage.ini" file with the path to the executable.

Homer's Dashboard contains many useful pages used for accessing robot functionality and providing diagnostic data about the robot code.

### Camera Streams
In camera-server builds, the Intake and Limelight camera pages will present video streams from their respective cameras. The Intake Camera Page looks for a stream at http://roborio-1511-frc.local:1181/stream.mjpg, and the Limelight Page looks at http://limelight-homer.local:5800. If a stream is not found or the camera server is disabled, the view will present the words "No Camera".

### Network Tables

<img src="https://raw.githubusercontent.com/wiki/petelilley/HomersDashboard/web/screenshots/network_tables.png" width="400">

The Network Tables page presents a list of key-value pairs in the SmartDashboard network table. The values are readonly and are sorted alphabetically.

### Auto Chooser

<img src="https://raw.githubusercontent.com/wiki/petelilley/HomersDashboard/web/screenshots/auto_chooser_empty.png" width="400">

The Auto Chooser Page contains the functionality to set the "Auto_Mode" and "Auto_Delay" SmartDashboard network table entries. The list of auto modes is obtained by using the comma seperated list entry "thunderdashboard_auto_list", along with each "thunderdashboard_auto_XXX" description entry. The "Auto_Mode" entry is set to the index of the selected auto mode in the given list. The "Auto_Delay" can be set to any value between 0 and 10 seconds.

### Blinky Blinky

<img src="https://raw.githubusercontent.com/wiki/petelilley/HomersDashboard/web/screenshots/blinky_blinky_custom.png" width="400">

The Blinky Blinky Page allows users to adjust the color of the LEDs on Homer. The LED Mode dropdown has a number of different configurations,
* Robot State
  - Colors change depending on the robot's state (Disabled = orange, Enabled = Alliance Color, Crater Mode = Green, etc).
* Alliance
  - The alliance color.
* Custom
  - A custom color. When this mode is selected, the user can pick any color to set the lights to.
* Off
  - Off or something.

The "LED_Mode" SmartDashboard network tables entry is set to the index of the selected mode. Also the "LED_Custom_Color_X" R, G, and B entries are set to the configured custom color.

### Robot Position

<img src="https://raw.githubusercontent.com/wiki/petelilley/HomersDashboard/web/screenshots/robot_position_0_0.png" width="500">

The Robot Position page displays the calculated position of the robot on the field. It reads the X and Y position (meters) from the "DriveCSV_x_pos" and "DriveCSV_y_pos" SmartDashboard network table entries, and the rotation from "DriveCSV_ang". The page displays a point representing the robot's position and a line representing its heading overlayed on top of the field image.

### Motion Profile

<img src="https://raw.githubusercontent.com/wiki/petelilley/HomersDashboard/web/screenshots/motion_profile_empty.png" width="500">

The Motion Profile Page allows users to record positional data of the robot over a period of time and plot it on a graph. The user can record an entire autonomous mode or just an interval, and can adjust the period at which data is recorded. The user can specify which pieces of data are plotted on the graph and how the graph is structured (based on time or position).
The "/home/lvuser/trajectory_motion.csv" file stored on the RoboRIO contains the same data recorded by the Motion Profile Page for autonomous modes. Switching to the "File" tab will present the option to load a CSV file for examination on the graph.

## Building
Supported operating systems are Windows and macOS. Make sure to resolve all the git submodules before building!
```bash
git submodule init
git submodule update
```
Build projects can be generated using CMake. Tested targets include Visual Studio 2019 or 2022 on Windows and Xcode or Unix Makefiles on macOS.
```bash
# Configure Windows
cmake . -Bbuild -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release
```
```bash
# Configure macOS
cmake . -Bbuild -G "Xcode" -DCMAKE_BUILD_TYPE=Release
```
```bash
# Build
cmake --build build
```
All the app's resources (Images, Fonts, etc.) are built into the executable, so there's no need to worry about moving them around once it's built.

__Important:__ OpenCV is used for creating the camera pages so it is required to be installed when building and running the app. The `-DDASHBOARD_WITH_CS=0` flag may be added to the cmake configuration to build without the camera server if OpenCV is not installed.

Problems have emerged when compiling the wpilib libraries (ntcore, cscore, wpiutil), so the `-DDASHBOARD_DOWNLOAD_WPILIB=1` flag may be added to the cmake configuration to download and link to pre-built libraries instead. The version downloaded can be specified by adding the `-DDASHBOARD_DOWNLOAD_WPILIB_VERSION=YEAR.X.X` flag.
