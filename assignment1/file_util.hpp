#ifndef _FILE_UTIL_HPP_
#define _FILE_UTIL_HPP_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "glm/vec4.hpp"
#include <vector>

void load_model(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors);
void save_model(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors);

#endif
//void save_model();

