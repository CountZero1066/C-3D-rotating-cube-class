# C++ 3D rotating cube class
Class for drawing a rotating 3D cube on a ST7735 display with a flexible implementation 

![ezgif-6ae366bdc1b501-ezgif com-resize](https://github.com/user-attachments/assets/706bb76b-86f1-48e1-96d5-3174236a2283)

## Overview
A lightweight C++ class designed for rendering and animating a 3D wireframe cube on displays using the Adafruit GFX library. The cube rotates smoothly around the X, Y, and Z axes, creating a dynamic visual effect. The class is optimized for embedded systems, particularly microcontrollers driving small TFT or OLED displays.

## Key Features
- Real-time 3D wireframe cube rotation around the X, Y, and Z axes.
- Customizable rotation speeds for each axis.
- Optimized screen clearing using an erasing circle that adapts to cube size.
- Easy integration into projects using Adafruit GFX-compatible displays.
- Scalable rendering that adjusts based on the cube’s side length and position.

## Software Structure
The Cube3D class encapsulates the logic for rendering a 3D cube and updating its rotation. It includes the following key methods:
Cube3D(TFT_eSPI &display, int centerX, int centerY, int sideLength)
- Constructor that initializes the cube’s center position, size, and display reference.
void update()
- Handles rotation updates and rendering.
void drawCube(uint16_t color)
- Renders the cube with the specified color.
void erasePreviousFrame()
- Clears the previous cube position using a dynamically calculated circle.

## Implementation Example
To integrate Cube3D into your project, below is a basic implementation. The example initializes the display, creates a Cube3D instance, and continuously updates the cube rotation in the loop.

![example pseduo code](https://github.com/user-attachments/assets/5369cb63-ef68-4def-90b2-047711833657)

## Potential Future Improvements
- Support for different projection methods (e.g., perspective projection).
- Adjustable erasure methods for improved performance on larger displays.
- Support for additional display drivers beyond Adafruit GFX, such as TFT_eSPI or U8g2.
