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

void loadScene(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors, std::vector<int> &modelSize, glm::vec4 &eye, glm::vec4 &lookAt, glm::vec4 &up, glm::vec4 &lrtb, float &N, float &F);

#endif
//void save_model();

