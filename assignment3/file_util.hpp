#ifndef _FILE_UTIL_HPP_
#define _FILE_UTIL_HPP_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
#include "hierarchy_node.hpp"
#include "Light.hpp"

void loadScene(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors,std::vector<glm::vec4> &normals,std::vector<glm::vec2> &textures,std::vector<std::vector<int> > &modelSize, std::vector<std::string> &modelHierarchy,std::vector<Light> &gLights,std::vector<std::string> &lightNames, std::vector<glm::vec4> &camera,std::vector<std::string> &modelNames,std::vector<std::vector<int> > &useTexture,std::vector<std::vector<std::string> > &texFileName,std::vector<std::vector<std::vector<int> > > &texFileSize, std::string &backgroundFileName );

void loadModel(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors,std::vector<glm::vec4> &normals,std::vector<glm::vec2> &textures, std::vector<int> &modelSize, std::vector<int> &useTexture,std::vector<std::string> &texFileName,std::vector<std::vector<int> > &texFileSize);

void loadHierarchy(std::string fileName,std::vector<std::string> &nodeNames,std::vector<glm::vec3> &tPar,std::vector<glm::vec3> &tFlags,std::vector<glm::vec3> &rPar,std::vector<glm::vec3> &rFlags, std::vector<csX75::HNode*> &nodeList, glm::vec4 positions[], glm::vec4 colors[],glm::vec4 normals[],glm::vec2 textures[], std::vector<int> &modelSize, int &offset,std::vector<int> useTexture,std::vector<std::string> texFileName,std::vector<std::vector<int> >texFileSize);

void loadBackground(std::string backgroundFileName,std::vector<glm::vec4> &bPositions, std::vector<glm::vec4> &bColors,std::vector<glm::vec4> &bNormals, std::vector<glm::vec2> &bTextures,std::vector<int> &backgroundSize,std::vector<std::string>  &bTexFileNames,std::vector<std::vector<int> > &bTexFileSize);

#endif


