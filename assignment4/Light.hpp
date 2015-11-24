#ifndef _Light_HPP_
#define _Light_HPP_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

struct Light {
	int on;
    glm::vec4 position;
    glm::vec4 intensities;
    float attenuation;
    float ambientCoefficient;
    float coneAngle; 
    glm::vec4 coneDirection; 
};

#endif
