#ifndef _ANIMATION_UTIL_HPP_
#define _ANIMATION_UTIL_HPP_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
#include "Light.hpp"

void saveKeyframe(std::string keyframeFileName, std::vector<std::vector< glm::vec3> > tParameters,
	std::vector<std::vector< glm::vec3> > rParameters, std::vector<Light> gLights, glm::vec4 cameraLocation, glm::vec4 cameraDirection);

void readKeyframe(std::ifstream &keyframeFile, glm::vec4 frame[], int numOfModelParameters, int numOfLights, int lightStates[]);

void interpolateParameter(glm::vec4 &interpolatedValue, glm::vec4 initValue, glm::vec4 finalValue, int frameNumber, int totalFrames, 
							bool rotation);

void interpolateFrames(std::vector<std::vector< glm::vec3> > &tPar,std::vector<std::vector< glm::vec3> > &rPar, 
		std::vector<Light> &lights, glm::vec4 &cameraLocation, glm::vec4 &cameraDirection,glm::vec4 initFrame[],
	    glm::vec4 finalFrame[], int frameNumber, int totalFrames);

void interpolateBezier(glm::vec4 &interpolatedValue, glm::vec4 controlPoints[], int frameNumber, int totalFrames);

#endif
