#include "file_util.hpp"

void loadScene(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors, std::vector<int> &modelSize, glm::vec4 &eye, glm::vec4 &lookAt, glm::vec4 &up, glm::vec4 &lrtb, float &N, float &F)
{
    std::ifstream sceneFile(fileName.c_str());

    for(int k=0;k<3;k++)
    {
		std::vector<glm::vec4> modelPositions;

		float Sx, Sy, Sz, Rx, Ry, Rz, Tx, Ty, Tz;
		std::string modelName;	
		sceneFile >> modelName;
		std::ifstream model(modelName.c_str());
		
		int numOfCoordinates = 0;

		while(1)
		{
			float v1, v2, v3, c1, c2, c3, c4;
			model >> v1;
			model >> v2;
			model >> v3;

			model >> c1;
			model >> c2;
			model >> c3;
			model >> c4;

			if(model.eof()) 
				break;

			glm::vec4 newPosition(v1, v2, v3, 1.0);
			glm::vec4 newColor(c1, c2, c3, c4);
			modelPositions.push_back(newPosition);
			colors.push_back(newColor);
			numOfCoordinates++;
	 	}
	 	model.close();

		sceneFile >> Sx;
		sceneFile >> Sy;
		sceneFile >> Sz;

		sceneFile >> Rx;
		sceneFile >> Ry;
		sceneFile >> Rz;
	
		sceneFile >> Tx;
		sceneFile >> Ty;
		sceneFile >> Tz;
	
		glm::mat4 rotationMatrix, translationMatrix, scaleMatrix, transformMatrix; 

		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(Tx,Ty,Tz));
		rotationMatrix = glm::rotate(glm::mat4(1.0f), Rx, glm::vec3(1.0f,0.0f,0.0f));
	  	rotationMatrix = glm::rotate(rotationMatrix, Ry, glm::vec3(0.0f,1.0f,0.0f));
	  	rotationMatrix = glm::rotate(rotationMatrix, Rz, glm::vec3(0.0f,0.0f,1.0f));
		scaleMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(Sx,Sy,Sz));
		transformMatrix = translationMatrix*rotationMatrix*scaleMatrix;
		
		for(int j=0; j<numOfCoordinates; j++)
		{
			glm::vec4 newPos=transformMatrix*modelPositions[j];
			positions.push_back(newPos);
		}

		modelSize.push_back(numOfCoordinates);

    }
    sceneFile >> eye[0];
    sceneFile >> eye[1];
    sceneFile >> eye[2];
    eye[3] = 1.0f;
    
    sceneFile >> lookAt[0];
    sceneFile >> lookAt[1];
    sceneFile >> lookAt[2];
    lookAt[3] = 1.0f;
    
    sceneFile >> up[0];
    sceneFile >> up[1];
    sceneFile >> up[2];
    up[3] = 1.0f;
    
    sceneFile >> lrtb[0];
    sceneFile >> lrtb[1];
    sceneFile >> lrtb[2];
    sceneFile >> lrtb[3];
    sceneFile >> N;
    sceneFile >> F;

    sceneFile.close();

}
