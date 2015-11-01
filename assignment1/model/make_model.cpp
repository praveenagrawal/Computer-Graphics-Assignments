#include "../glm/vec3.hpp"
#include "../glm/vec4.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtx/rotate_vector.hpp"
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>

int main(){
    std::string cubeFile("cube.txt");
    std::string outputFile("glassesModel.raw");
    std::string transformationFile("glassesTrans.txt");

    std::ifstream cubeModel(cubeFile.c_str());
    std::vector<glm::vec4> cubePositions;
    std::vector<glm::vec4> modelPositions;
    std::vector<glm::vec4> modelColors;

    std:: ofstream modelFile( (outputFile).c_str(), std::ios_base::out | std::ios_base::trunc);

// Load the vertices of the cube
    while(1){
	float v1, v2, v3, c1, c2, c3;

	cubeModel >> v1;
	cubeModel >> v2;
	cubeModel >> v3;

	cubeModel >> c1;
	cubeModel >> c2;
	cubeModel >> c3;

	if(cubeModel.eof()) {
	    break;
	}

	glm::vec4 newPosition(v1, v2, v3, 1.0);
	cubePositions.push_back(newPosition);
   }


    std::ifstream transformationData(transformationFile.c_str());

    int numberOfTransformations;
    transformationData >> numberOfTransformations;

    for(int i=0; i<numberOfTransformations; i++){
	float xrot, yrot, zrot, tx, ty, tz, sx, sy, sz, r, g, b, a;

	transformationData >> xrot;
	transformationData >> zrot;
	transformationData >> yrot;

	transformationData >> tx;
	transformationData >> tz;
	transformationData >> ty;

	transformationData >> sx;
	transformationData >> sz;
	transformationData >> sy;

	transformationData >> r;
	transformationData >> g;
	transformationData >> b;
	transformationData >> a;

        glm::mat4 rotationMatrix, translationMatrix, scaleMatrix, transformMatrix; 

	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tx,ty,tz));

	rotationMatrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
  	rotationMatrix = glm::rotate(rotationMatrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
  	rotationMatrix = glm::rotate(rotationMatrix, zrot, glm::vec3(0.0f,0.0f,1.0f));

	scaleMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(sx,sy,sz));

	transformMatrix = translationMatrix*rotationMatrix*scaleMatrix;

	for(int j=0; j<36; j++){
	    glm::vec4 newPos=transformMatrix*cubePositions[j];
	    glm::vec4 newCol( r,g,b,a);
	    modelPositions.push_back(newPos);
	    modelColors.push_back(newCol);   
	}
    }			

    glm::vec4 sum(0.f,0.f,0.f,1.0);

    for(int i=0; i<modelPositions.size();i++){
	sum=sum+modelPositions[i];
    }

    sum=(float)(1.0/modelPositions.size())*sum;

    for(int i=0; i<modelPositions.size(); i++){
	modelPositions[i]=modelPositions[i]-sum;
	modelFile << modelPositions[i][0] << "    " << modelPositions[i][1] << "    " << modelPositions[i][2] << "    " << modelColors[i][0] <<"    "<< modelColors[i][1]<<"    "<<modelColors[i][2]<<"    "<<modelColors[i][3]<<std::endl;

	if(i%3 ==2) modelFile<< std::endl;
    }

    modelFile.close();

}
