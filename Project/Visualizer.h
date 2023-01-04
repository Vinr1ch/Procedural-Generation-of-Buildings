#pragma once
#include <GL/glut.h>
#include "Textures.h"
#include "Vec3.h"
#include "Geometry.h"
#include "ShapeTree.h"



// GLUT callbacks and functions
void initGlut(int whatToDraw, int argc, char **argv);
void displayFunc(void);
void idleFunc(void);
void reshapeFunc(int width, int height);
void mouseCallbackFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);
void keyboardFunc(unsigned char key, int x, int y);

// other [OpenGL] functions

void renderBitmapString(float x, float y, float z, void *font, const char *string);

//Drawing 3d objects
void drawBlock(Vec3& basePoint, Vec3& size);//draw cuboid
void drawCylinder(Vec3& basePoint, Vec3& size);
void drawPlain(Vec3& position, Vec3& size);

//Drawing 2D objects
void draw2DBlock(float centerX, float centerY, float width, float height);
void drawLine(float startX, float startY, float endX, float endY);


void drawShapes(std::vector<Geometry> shapes, int argc, char **argv); // draws 3d model of building
