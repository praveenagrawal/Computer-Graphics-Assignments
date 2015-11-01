//Citation: Most of the part of this code segment is taken from the color_cube demo file uploaded for CS475/CS675 course in IITB.
/*
  A program which opens a window and draws the "color cube."

  Use the arrow keys and PgUp,PgDn, 
  keys to make the cube move.

  Written by - 
               Parag Chaudhuri
*/
#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "file_util.hpp"
#include "glm/gtx/rotate_vector.hpp"

// Translation Parameters
GLfloat xpos=0.0,ypos=0.0,zpos=0.0;
// Rotation Parameters
GLfloat xrot=0.0,yrot=0.0,zrot=0.0;
//Get Mouse position
GLfloat getPos =0.0;
//Add New Point
GLfloat AddPoint = 0.0;
//Remove Previous Point
GLfloat RemovePoint = 0.0;
//Modelling Mode = 1, Inspection mode = 0
GLfloat Mode = 0.0;
//Mode for loading and saving file
GLfloat LoadModel = 0.0;
GLfloat SaveModel = 0.0;
//Get Cursor Position
GLfloat cursorX = 0.0, cursorY = 0.0, cursorZ = 0.0;
//Number of Modelling Points added
GLfloat NumPoints = 0.0;
//Flag to signal closing the polygon
GLfloat ClosePolygon = 0.0;
//Clear Buffer
GLfloat ClearBuffer = 0.0;

std::string fileName;

//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;



//-------------------------------------------------------------------------

#endif
