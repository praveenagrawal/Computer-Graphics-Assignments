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
#include "hierarchy_node.hpp"
#include "animation_util.hpp"
#include "Light.hpp"
#include <string>

// Translation Parameters
GLfloat xpos=0.0,ypos=0.0,zpos=0.0;
// Rotation Parameters
GLfloat xrot=0.0,yrot=0.0,zrot=0.0;

//mode 0---> Joint controlled, 1---> camera controlled,  2---> lights controlled
GLuint mode=0;

GLuint vPosition,vColor, vNormal,vTexture;
std::vector<glm::mat4> matrixStack;

GLuint selectedNodeID=0, selectedLightID=0, selectedModelID=0, saveFrame=0, recordingMode=1 ;

//Running variable to toggle wireframe/solid modelling
bool solid=true;



//-------------------------------------------------------------------------

#endif
