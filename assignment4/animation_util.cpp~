#include "animation_util.hpp"

void saveKeyframe(std::string keyframeFileName, std::vector<std::vector< glm::vec3> > tParameters,
	std::vector<std::vector< glm::vec3> > rParameters, std::vector<Light> gLights, glm::vec4 cameraLocation, glm::vec4 cameraDirection)
{

    std:: ofstream keyFrameFile( (keyframeFileName).c_str(), std::ios_base::out |std::ios_base::app);

	for(int i=0; i<tParameters.size(); i++)
	{
		for(int j=0; j< tParameters[i].size(); j++)
		{
			keyFrameFile <<tParameters[i][j][0] <<"      " << tParameters[i][j][1] <<"      " << tParameters[i][j][2] <<std::endl;
			keyFrameFile <<rParameters[i][j][0] <<"      " << rParameters[i][j][1] <<"      " << rParameters[i][j][2] <<std::endl;
		}
	}

	keyFrameFile<<std::endl;

	for(int i=0; i<gLights.size(); i++)
	{
		keyFrameFile << gLights[i].on <<std::endl;
		keyFrameFile << gLights[i].intensities[0]<<"    "<<gLights[i].intensities[1]<<"    "<<gLights[i].intensities[2]<<std::endl;
		keyFrameFile << gLights[i].coneDirection[0]<<"    "<<gLights[i].coneDirection[1]<<"    "<<
																	gLights[i].coneDirection[2]<<std::endl;
	}

	keyFrameFile<<std::endl;

	keyFrameFile<< cameraLocation[0]<<"    "<<cameraLocation[1]<<"    "<<cameraLocation[2]<<std::endl;
	keyFrameFile<< cameraDirection[0]<<"    "<<cameraDirection[1]<<"    "<<cameraDirection[2]<<std::endl;

	keyFrameFile<<std::endl<<std::endl<<std::endl;

	keyFrameFile.close();
}

void readKeyframe(std::ifstream &keyframeFile, glm::vec4 frame[], int numOfModelParameters, int numOfLights, int lightStates[])
{
	int ct=0;
	for(int i=0; i<numOfModelParameters; i++)
	{
		glm::vec4 newParameter(1.0f, 1.0f, 1.0f, 1.0f);
		keyframeFile >> newParameter[0] >> newParameter[1] >> newParameter[2];
		frame[ct] = newParameter;
		ct++; 
	}

	for(int i=0; i<numOfLights; i++)
	{
		keyframeFile >> lightStates[i];
		
		glm::vec4 newParameter(1.0f, 1.0f, 1.0f, 1.0f);
		keyframeFile >> newParameter[0] >> newParameter[1] >> newParameter[2];
		frame[ct] = newParameter;
		ct++;

		keyframeFile >> newParameter[0] >> newParameter[1] >> newParameter[2];
		frame[ct] = newParameter;
		ct++; 
	}

	glm::vec4 newParameter(1.0f, 1.0f, 1.0f, 1.0f);
	keyframeFile >> newParameter[0] >> newParameter[1] >> newParameter[2];
	frame[ct] = newParameter;
	ct++;

	keyframeFile >> newParameter[0] >> newParameter[1] >> newParameter[2];
	frame[ct] = newParameter;
	ct++; 

}

/*
void interpolateBezier(glm::vec4 &interpolatedValue, glm::vec4 controlPoints[], int frameNumber, int totalFrames)
{
	glm::mat4 coeffMatrix;
	coeffMatrix[0]=glm::vec4(1.0f, -3.0f, 3.0f, 1.0f);	
	coeffMatrix[1]=glm::vec4(0.0f, 3.0f, -6.0f, 3.0f);
	coeffMatrix[2]=glm::vec4(0.0f, 0.0f, 3.0f, -3.0f);
	coeffMatrix[3]=glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	glm::vec4 tValues(1.0f, frameNumber/totalFrames,(frameNumber/totalFrames)*(frameNumber/totalFrames),
							(frameNumber/totalFrames)*(frameNumber/totalFrames)*(frameNumber/totalFrames));
	
	
	interpolatedValue=coeffMatrix*glm::vec4(controlPoints[0].x,controlPoints[1].x,controlPoints[2].x,controlPoints[3].x);
//	interpolatedValue[1]=tValues*coeffMatrix*glm::vec4(controlPoints[0][1],controlPoints[1][1],controlPoints[2][1],1.0f);
//	interpolatedValue[2]=tValues*coeffMatrix*glm::vec4(controlPoints[0][2],controlPoints[1][2],controlPoints[2][2],1.0f);
	interpolatedValue[3]=1.0f;
}
*/
void interpolateParameter(glm::vec4 &interpolatedValue, glm::vec4 initValue, glm::vec4 finalValue, int frameNumber, int totalFrames,
				bool rotation)
{
	for(int i=0 ;i<3; i++)
	{
		if(rotation)
		{
			if(finalValue[i]-initValue[i] >3.1416)
			{
				interpolatedValue[i] = initValue[i] + ((finalValue[i]-initValue[i]) - 2*3.1416)*(float)frameNumber/(float)totalFrames;
			}
			else if (finalValue[i]-initValue[i] >-3.1416)
			{
				interpolatedValue[i] = initValue[i] + (finalValue[i]-initValue[i])*(float)frameNumber/(float)totalFrames;
			}
			else
			{
				interpolatedValue[i] = initValue[i] + (2*3.1416 +finalValue[i]-initValue[i])*(float)frameNumber/(float)totalFrames;
			}
		}
		else
		{
			interpolatedValue[i] = initValue[i] + ((finalValue[i]-initValue[i]))*(float)frameNumber/(float)totalFrames;
		}
	}
	interpolatedValue[3]=1.0;
}

void interpolateFrames(std::vector<std::vector< glm::vec3> > &tPar,std::vector<std::vector< glm::vec3> > &rPar, 
		std::vector<Light> &lights, glm::vec4 &cameraLocation, glm::vec4 &cameraDirection, glm::vec4 initFrame[],
	    glm::vec4 finalFrame[], int frameNumber, int totalFrames)
{
	int ct=0;
	for(int i=0; i<tPar.size(); i++)
	{
		for(int j=0; j<tPar[i].size(); j++)
		{
			glm::vec4 temp;
			interpolateParameter(temp, initFrame[ct], finalFrame[ct], frameNumber, totalFrames, false);
			tPar[i][j]=glm::vec3(temp);
			ct++;
			interpolateParameter(temp, initFrame[ct], finalFrame[ct], frameNumber, totalFrames, true);
			rPar[i][j]=glm::vec3(temp);
			ct++;
		}
		
	}

	for(int i=0; i<lights.size(); i++)
	{
		interpolateParameter(lights[i].intensities, initFrame[ct], finalFrame[ct], frameNumber, totalFrames,false);
		ct++;
		interpolateParameter(lights[i].coneDirection, initFrame[ct], finalFrame[ct], frameNumber, totalFrames, true);
		ct++;
	}

	interpolateParameter(cameraLocation, initFrame[ct], finalFrame[ct], frameNumber, totalFrames, false);
	ct++;
	interpolateParameter(cameraDirection, initFrame[ct], finalFrame[ct], frameNumber, totalFrames, true);
	ct++;	
}
