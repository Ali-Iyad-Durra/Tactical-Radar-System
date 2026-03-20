# 🛡️ Tactical Combat HUD & Target Tracker (C++ & OpenCV)

An advanced real-time computer vision system that emulates a fighter jet's **Head-Up Display (HUD)**. This project features multi-color object tracking, a dynamic radar mini-map, and a proximity-based audio/visual warning system.

---

## 🚀 Key Features

* **Multi-Color Tracking**: Simultaneous or individual tracking of three primary tactical colors: **Red**, **Blue**, and **Yellow**.
* **Tactical Control Panel**: Built-in GUI Trackbars allow users to toggle specific color sensors on/off in real-time without restarting the application.
* **Dynamic Target ID**: Automatically identifies the target's color signature (`RED`, `BLUE`, `YELLOW`) and displays it directly above the tracked object.
* **Combat HUD Interface**:
    * **Tactical Crosshair**: Centralized aiming reticle for precision tracking.
    * **Radar Mini-map**: A functional 2D radar at the bottom corner that maps the target's relative position with a vector line.
    * **Scanlines Effect**: A visual filter simulating high-tech military CRT or digital displays.
* **Proximity Alert System**:
    * **Visual Warning**: A flashing **"WARNING: PROXIMITY"** alert triggers when a target enters the "danger zone" (large area size).
    * **Adaptive Audio (Beep System)**: System frequency and pulse rate increase as the target gets closer, providing auditory spatial awareness.

---

## 🛠️ Technical Stack

* **Language**: C++ (Optimized for high-speed frame processing).
* **Library**: OpenCV 4.x (Image processing, HSV filtering, and Contour analysis).
* **OS Interface**: Windows API (`windows.h`) for hardware-level audio alerts.

---

## ⚙️ How It Works

1.  **HSV Space Filtering**: Converts raw BGR frames to the **HSV color space** for robust detection under varying lighting conditions.
2.  **Morphological Operations**: Applies `Erode` and `Dilate` functions to eliminate background noise and stabilize the target shape.
3.  **Contour Analysis**: Calculates the area of detected objects to isolate the primary target while ignoring small artifacts.
4.  **Coordinate Transformation**: Maps 3D-like camera coordinates onto a 2D scaled radar interface in real-time.

---

## 📋 Requirements

* **Compiler**: MSVC (Visual Studio 2019 or later).
* **Library**: OpenCV 4.0+ installed and linked to your project.
* **OS**: Windows (Required for the `Beep` function).
* **Hardware**: Standard 720p/1080p Webcam.

---

## 🕹️ Controls

* **Trackbars**: Slide to `1` to activate a color sensor, `0` to deactivate.
* **ESC Key**: Terminate the system and close all windows.

---

## 📸 HUD Preview

```text
STATUS: COMBAT_MODE ACTIVE  
SENSOR: RED_TARGET_ACQUIRED  
WARNING: PROXIMITY ALERT (Visual & Audio trigger)
