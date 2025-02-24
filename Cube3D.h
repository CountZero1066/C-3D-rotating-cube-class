#ifndef CUBE3D_SINGLE_FILE_H
#define CUBE3D_SINGLE_FILE_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <math.h>

/*
  Cube3D Class:
  
  This class implements a simple 3D cube that is rotated and drawn onto a display
  using the Adafruit_GFX library. The cube is defined by 8 vertices in 3D space.
  The class provides methods to rotate the cube about the X, Y, and Z axes and then
  projects these 3D vertices onto a 2D plane (the display) using an orthogonal projection.

*/
//-----------------------------------------------------------------------
// Class Constructor
//-----------------------------------------------------------------------
class Cube3D {
public:
  //   display: Reference to the Adafruit_ST7735 display to draw on.
  //   centerX, centerY: The display coordinates at which the cube is centered.
  //   sideLength: Scaling factor for the cube (the cube’s face, when unrotated, is 2*sideLength in width/height).
  Cube3D(Adafruit_ST7735 &display, int centerX, int centerY, float sideLength);

  // Draws the cube using the current rotation.
  void draw();
  // Updates the cube's rotation and redraws it.
  void update();

private:
  // 3D point structure.
  struct Point3D {
    float x, y, z;
  };
  // 2D point structure.
  struct Point2D {
    int x, y;
  };

  // Projects a 3D point to 2D
  Point2D project(Point3D p);
  // Rotation functions for each axis.
  Point3D rotateX(Point3D p, float angle);
  Point3D rotateY(Point3D p, float angle);
  Point3D rotateZ(Point3D p, float angle);

  // Draws a line between two 3D points (projected to 2D) using the given color.
  void drawLine3D(Point3D p1, Point3D p2, uint16_t color);

  // Draws the cube by connecting its (rotated) vertices.
  void drawCube(uint16_t color);

  // Erases the cube’s drawing area by drawing a black circle covering the entire region.
  void eraseCubeArea();

  // Reference to the display.
  Adafruit_ST7735 &tft;
  // Center coordinates for drawing the cube.
  int centerX, centerY;
  // Scaling factor: the unrotated cube’s face spans 2*sideLength.
  float sideLength;
  // Rotation angles (in radians) about X, Y, and Z.
  float angleX, angleY, angleZ;
  // The original cube vertices (for a cube centered at the origin).
  Point3D baseCubeVertices[8];
};

//-----------------------------------------------------------------------
// Class Implementation
//-----------------------------------------------------------------------

Cube3D::Cube3D(Adafruit_ST7735 &display, int cx, int cy, float sl)
  : tft(display), centerX(cx), centerY(cy), sideLength(sl),
    angleX(0.0), angleY(0.0), angleZ(0.0)
{
  // Initialize the base cube vertices for a unit cube (coordinates: -1 to +1).
  baseCubeVertices[0] = {-1, -1, -1};
  baseCubeVertices[1] = {-1,  1, -1};
  baseCubeVertices[2] = { 1,  1, -1};
  baseCubeVertices[3] = { 1, -1, -1};
  baseCubeVertices[4] = {-1, -1,  1};
  baseCubeVertices[5] = {-1,  1,  1};
  baseCubeVertices[6] = { 1,  1,  1};
  baseCubeVertices[7] = { 1, -1,  1};
}

// Projects a 3D point onto the 2D display.
Cube3D::Point2D Cube3D::project(Point3D p) {
  Point2D projected;
  projected.x = centerX + int(p.x * sideLength);
  projected.y = centerY + int(p.y * sideLength);
  return projected;
}

// Rotates a point around the X axis.
Cube3D::Point3D Cube3D::rotateX(Point3D p, float angle) {
  Point3D rotated;
  rotated.x = p.x;
  rotated.y = p.y * cos(angle) - p.z * sin(angle);
  rotated.z = p.y * sin(angle) + p.z * cos(angle);
  return rotated;
}

// Rotates a point around the Y axis.
Cube3D::Point3D Cube3D::rotateY(Point3D p, float angle) {
  Point3D rotated;
  rotated.x = p.x * cos(angle) + p.z * sin(angle);
  rotated.y = p.y;
  rotated.z = -p.x * sin(angle) + p.z * cos(angle);
  return rotated;
}

// Rotates a point around the Z axis.
Cube3D::Point3D Cube3D::rotateZ(Point3D p, float angle) {
  Point3D rotated;
  rotated.x = p.x * cos(angle) - p.y * sin(angle);
  rotated.y = p.x * sin(angle) + p.y * cos(angle);
  rotated.z = p.z;
  return rotated;
}

// Draws a line between two 3D points (after projection) with the given color.
void Cube3D::drawLine3D(Point3D p1, Point3D p2, uint16_t color) {
  Point2D proj1 = project(p1);
  Point2D proj2 = project(p2);
  tft.drawLine(proj1.x, proj1.y, proj2.x, proj2.y, color);
}

// Draws the cube by first rotating the base vertices by the current angles.
void Cube3D::drawCube(uint16_t color) {
  // Create an array for the rotated vertices.
  Point3D rotatedVertices[8];
  for (int i = 0; i < 8; i++) {
    Point3D v = baseCubeVertices[i];
    v = rotateX(v, angleX);
    v = rotateY(v, angleY);
    v = rotateZ(v, angleZ);
    rotatedVertices[i] = v;
  }
  
  // Draw the edges of the cube (front face, back face, and connecting edges).
  // Front face
  drawLine3D(rotatedVertices[0], rotatedVertices[1], color);
  drawLine3D(rotatedVertices[1], rotatedVertices[2], color);
  drawLine3D(rotatedVertices[2], rotatedVertices[3], color);
  drawLine3D(rotatedVertices[3], rotatedVertices[0], color);
  
  // Back face
  drawLine3D(rotatedVertices[4], rotatedVertices[5], color);
  drawLine3D(rotatedVertices[5], rotatedVertices[6], color);
  drawLine3D(rotatedVertices[6], rotatedVertices[7], color);
  drawLine3D(rotatedVertices[7], rotatedVertices[4], color);
  
  // Connecting edges
  drawLine3D(rotatedVertices[0], rotatedVertices[4], color);
  drawLine3D(rotatedVertices[1], rotatedVertices[5], color);
  drawLine3D(rotatedVertices[2], rotatedVertices[6], color);
  drawLine3D(rotatedVertices[3], rotatedVertices[7], color);
}

// Erases the area occupied by the rotating cube by drawing a black circle.
// The circle is centered at (centerX, centerY) with a radius that covers
// a square of side length 2*sideLength.
void Cube3D::eraseCubeArea() {
  // Calculate the radius: half the diagonal of the square with side = 2*sideLength.
int radius = int(sideLength * sqrt(2)) + (sideLength / 2);
  tft.fillCircle(centerX, centerY, radius, ST77XX_BLACK);
}

// Draws the cube in white.
void Cube3D::draw() {
  drawCube(ST77XX_WHITE);
}

// Updates the cube's rotation.
// First erases the previous cube by drawing a black circle, then increments
// the rotation angles, and finally draws the updated cube.
void Cube3D::update() {
  // Erase the previous drawing using our erasing function.
  eraseCubeArea();

  // Increment rotation angles (adjust these values to match your desired speed)
  angleX += 0.01;
  angleY += 0.02;
  angleZ += 0.03;

  // Keep the angles within 0 to 2π.
  if (angleX > TWO_PI) angleX -= TWO_PI;
  if (angleY > TWO_PI) angleY -= TWO_PI;
  if (angleZ > TWO_PI) angleZ -= TWO_PI;

  // Draw the new rotated cube.
  draw();
}

#endif // CUBE3D_SINGLE_FILE_H
