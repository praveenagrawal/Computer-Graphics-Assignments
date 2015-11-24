#include "file_util.hpp"

void loadScene(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors,std::vector<glm::vec4> &normals,std::vector<glm::vec2> &textures, std::vector<std::vector<int> > &modelSize, std::vector<std::string> &modelHierarchy,std::vector<Light> &gLights, std::vector<std::string>  &lightNames, std::vector<glm::vec4> &camera,std::vector<std::string> &modelNames,std::vector<std::vector<int> > &useTexture,std::vector<std::vector<std::string> > &texFileName,std::vector<std::vector<std::vector<int> > > &texFileSize, std::string &backgroundFileName )
{
    std::ifstream sceneFile(fileName.c_str()); 
    int numOfModels;
    sceneFile >> numOfModels;
    for(int k=0;k<numOfModels;k++)
    {
		std::string modelName;
		sceneFile >>modelName;
		modelNames.push_back(modelName);

		std::string modelFileName;
		sceneFile >> modelFileName;
		
		std::vector<int> newModelSize;
		std::vector<int> newUseTexture;
		std::vector<std::string> newTexFileName;
		std::vector<std::vector<int> >newTexFileSize;
		loadModel(modelFileName,positions,colors,normals,textures,newModelSize,newUseTexture,newTexFileName,newTexFileSize);
		modelSize.push_back(newModelSize);
		useTexture.push_back(newUseTexture);
		texFileName.push_back(newTexFileName);
		texFileSize.push_back(newTexFileSize);

		std::string newModelHierarchy;
		sceneFile >> newModelHierarchy;
		modelHierarchy.push_back(newModelHierarchy);
	}

	//Lighting
	std::string lightFileName;
	sceneFile >> lightFileName;
	std::ifstream lightFile(lightFileName.c_str());
 
	while(1)
	{
		std::string newLightName;
		lightFile >> newLightName;


		if(lightFile.eof()) 
		{
			break;
		}

		lightNames.push_back(newLightName);

		Light newLight;
		newLight.on=1;

		float l1,l2,l3,l4;
		lightFile >> l1 >> l2 >> l3 >> l4;
		newLight.position= glm::vec4(l1,l2,l3,l4);

		lightFile >> l1 >> l2 >> l3;
		newLight.intensities=glm::vec4(l1,l2,l3,1.0);

		lightFile >> l1 >> l2 >> l3;
		newLight.attenuation = l1;
		newLight.ambientCoefficient = l2; //no ambient light
		newLight.coneAngle = l3;

		lightFile >> l1 >> l2 >> l3;
		newLight.coneDirection = glm::vec4(l1,l2,l3,1.0);

		gLights.push_back(newLight);
	}

	//Camera
	float l1,l2,l3,l4;

	sceneFile >> l1 >> l2 >> l3;
	glm::vec4 cameraEye(l1,l2,l3,1.0);
	camera.push_back(cameraEye);

	sceneFile >> l1 >> l2 >> l3;
	glm::vec4 cameraDirection(l1,l2,l3,1.0);
	camera.push_back(cameraDirection);

	sceneFile >> l1 >> l2 >> l3 >>l4;
	glm::vec4 cameraRTNF(l1,l2,l3,l4);
	camera.push_back(cameraRTNF);

	sceneFile >> backgroundFileName;

    sceneFile.close();

}

void loadModel(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors,std::vector<glm::vec4> &normals,std::vector<glm::vec2> &textures, std::vector<int> &modelSize, std::vector<int> &useTexture,std::vector<std::string> &texFileName,std::vector<std::vector<int> > &texFileSize)
{
	std::ifstream modelFile(fileName.c_str());
	while(1)
	{
		int newUseTexture;
		modelFile >> newUseTexture;
		useTexture.push_back(newUseTexture);		
		
		std::string newTexFileName;
		std::vector<int> newTexSize;
		int w=0,h=0;
		if(newUseTexture==1)
		{
			modelFile >> newTexFileName;
			modelFile >>w>>h;
		}
		newTexSize.push_back(w);
		newTexSize.push_back(h);
		texFileSize.push_back(newTexSize);
		texFileName.push_back(newTexFileName);

		std::string partFile;	
		modelFile >> partFile;

		if(modelFile.eof()) 
		{	
			break;
		}

		std::ifstream model(partFile.c_str());
		int numOfCoordinates = 0;
		while(1)
		{
			float v1, v2, v3, c1, c2, c3, c4, n1, n2, n3, u1=0, u2=0;

			model >> v1;
			if(model.eof()) 
			{
				break;
			}

			model >> v2;
			model >> v3;

			model >> c1;
			model >> c2;
			model >> c3;
			model >> c4;
			
			model >> n1;
			model >> n2;
			model >> n3;

			if(newUseTexture==1)
			{
				model >> u1;
				model >> u2;
			}

			glm::vec4 newPosition(v1, v2, v3, 1.0);
			glm::vec4 newColor(c1, c2, c3, c4);
			glm::vec4 newNormal(n1, n2, n3, 1.0);
			glm::vec2 newTexture(u1,u2);	        

			positions.push_back(newPosition);
			colors.push_back(newColor);
			normals.push_back(newNormal);
			textures.push_back(newTexture);

			numOfCoordinates++;
		}
		modelSize.push_back(numOfCoordinates);
	}
}

void loadHierarchy(std::string fileName,std::vector<std::string> &nodeNames,std::vector<glm::vec3> &tPar,std::vector<glm::vec3> &tFlags,std::vector<glm::vec3> &rPar,std::vector<glm::vec3> &rFlags, std::vector<csX75::HNode*> &nodeList, glm::vec4 positions[], glm::vec4 colors[],glm::vec4 normals[],glm::vec2 textures[], std::vector<int> &modelSize, int &offset,std::vector<int> useTexture,std::vector<std::string> texFileName,std::vector<std::vector<int> >texFileSize)
{
	std::ifstream hierarchyFile(fileName.c_str());
	int nodeCount=0;

	while(1)
	{	
		int nodeID;
		hierarchyFile >> nodeID;

		std::string newNodeName;
		hierarchyFile >> newNodeName;
		nodeNames.push_back(newNodeName);
	
		int parentID;
		hierarchyFile >> parentID;

		if(nodeID <= parentID)
		{
			std::cout<<"   "<<modelSize.size()<<"   "<<"Error 1 in hierarchy file. Exiting \n";
			break;
		}

		if(nodeID== nodeCount)
		{
			csX75::HNode* node;
			if(nodeID==0)
			{
				node= new csX75::HNode(NULL,modelSize[nodeID],&positions[offset],&colors[offset],&normals[offset],&textures[offset],(size_t)(16*modelSize[nodeID]),(size_t)(16*modelSize[nodeID]),(size_t)(16*modelSize[nodeID]),(size_t)(8*modelSize[nodeID]),useTexture[nodeID],texFileName[nodeID],texFileSize[nodeID]);
			}else
			{
				node= new csX75::HNode(nodeList[parentID], modelSize[nodeID],&positions[offset],&colors[offset],&normals[offset],&textures[offset],(size_t)(16*modelSize[nodeID]),(size_t)(16*modelSize[nodeID]),(size_t)(16*modelSize[nodeID]),(size_t)(8*modelSize[nodeID]),useTexture[nodeID],texFileName[nodeID],texFileSize[nodeID]);
			}
	
			float tx,ty,tz, rx,ry,rz;
			float txf,tyf,tzf, rxf,ryf,rzf;
			hierarchyFile >> tx >> ty >> tz >> txf >> tyf >> tzf;
			hierarchyFile >> rx >> ry >> rz >> rxf >> ryf >> rzf;
			glm::vec3 newTPar(tx,ty,tz);
			glm::vec3 newTFlag(txf,tyf,tzf);

			glm::vec3 newRPar(rx,ry,rz);
			glm::vec3 newRFlag(rxf,ryf,rzf);

			tPar.push_back(newTPar);
			tFlags.push_back(newTFlag);

			rPar.push_back(newRPar);
			rFlags.push_back(newRFlag);

			node->change_parameters(tx, ty, tz, rx, ry, rz);
			nodeList.push_back(node);
			offset+=modelSize[nodeID];
			nodeCount++;
			if(nodeCount == modelSize.size()) {break;}
			
		}else
		{
			std::cout<<"Error 2  in hierarchy file. Exiting \n";
			break;
		}
	} 	
}

void loadBackground(std::string backgroundFileName,std::vector<glm::vec4> &bPositions, std::vector<glm::vec4> &bColors,std::vector<glm::vec4> &bNormals, std::vector<glm::vec2> &bTextures,std::vector<int> &backgroundSize,std::vector<std::string>  &bTexFileName,std::vector<std::vector<int> > &bTexFileSize)
{
	std::ifstream backgroundFile(backgroundFileName.c_str());
	while(1)
	{
		std::string newTexFileName;
		std::vector<int> newTexSize;
		int w=0,h=0;
		backgroundFile >> newTexFileName;
		if(backgroundFile.eof()) 
		{
			break;
		}


		backgroundFile >> w >> h;

		newTexSize.push_back(w);
		newTexSize.push_back(h);
		bTexFileSize.push_back(newTexSize);
		bTexFileName.push_back(newTexFileName);

		int numVertices=0;
		backgroundFile>>numVertices;

		for(int i=0;i<numVertices; i++)
		{
			float v1, v2, v3, c1, c2, c3, c4, n1, n2, n3, u1=0, u2=0;

			backgroundFile >> v1;
			backgroundFile >> v2;
			backgroundFile >> v3;

			backgroundFile >> c1;
			backgroundFile >> c2;
			backgroundFile >> c3;
			backgroundFile >> c4;
			
			backgroundFile >> n1;
			backgroundFile >> n2;
			backgroundFile >> n3;

			backgroundFile>> u1;
			backgroundFile>> u2;

			glm::vec4 newPosition(v1, v2, v3, 1.0);
			glm::vec4 newColor(c1, c2, c3, c4);
			glm::vec4 newNormal(n1, n2, n3, 1.0);
			glm::vec2 newTexture(u1,u2);	        

			bPositions.push_back(newPosition);
			bColors.push_back(newColor);
			bNormals.push_back(newNormal);
			bTextures.push_back(newTexture);

		}
		backgroundSize.push_back(numVertices);
	}
}



