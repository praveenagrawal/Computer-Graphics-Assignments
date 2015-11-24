//Citation: Most of the part of this code segment is taken from the color_cube demo file uploaded for CS475/CS675 course in IITB.
#include "main.hpp"

GLuint shaderProgram;
std::vector<GLuint> vbo, vao, backTex;

glm::mat4 rotation_matrix;
glm::mat4 translation_matrix;
glm::mat4 projection_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;

glm::vec4 camera_location;
glm::vec4 eye;

glm::mat4 wcs2vcsMatrix;

GLuint uModelMatrix;
GLuint normalMatrix;
GLuint viewMatrix;
GLuint cameraLocation;

GLuint useTexture;
GLuint numLights;

std::vector<std::vector<GLuint> > gLightsID;

std::vector<std::string> modelNames;
std::vector<std::vector<std::string> > nodeNames;
std::vector<std::string> lightNames;

std::vector<std::vector<std::string> > texFileNames;
std::vector<std::vector<std::vector<int> > >texFileSize;

std::vector<std::vector<int> > use_texture;

std::vector<std::vector<glm::vec3> >tParameters;
std::vector<std::vector<glm::vec3> >rParameters;

std::vector<std::vector<glm::vec3> >tFlags;
std::vector<std::vector<glm::vec3> >rFlags;
//-----------------------------------------------------------------

std::vector<glm::vec4> vPositions;
std::vector<glm::vec4> vColors;
std::vector<glm::vec4> vNormals;
std::vector<glm::vec2> vTextures;

std::vector<glm::vec4> bPositions;
std::vector<glm::vec4> bColors;
std::vector<glm::vec4> bNormals;
std::vector<glm::vec2> bTextures;

std::vector<glm::vec4> camera;


//keeps track of the number of vertices in each part of each model
std::vector<std::vector<int> > modelSize;

std::vector<int>  backgroundSize;
std::vector<std::string>  bTexFileNames;
std::vector<std::vector<int> > bTexFileSize;
//List of nodes in each model
std::vector<std::vector<csX75::HNode*> > nodeList; 

std::vector<Light> gLights;


void initBackground(std::string backgroundFileName)
{
  loadBackground(backgroundFileName,bPositions,bColors,bNormals,bTextures, backgroundSize,bTexFileNames,bTexFileSize);

  int bNumVertices = bPositions.size(), k=0;

  glm::vec4 bPositions1[bNumVertices];
  glm::vec4 bColors1[bNumVertices];
  glm::vec4 bNormals1[bNumVertices];
  glm::vec2 bTextures1[bNumVertices];

  for(int i=0;i<bNumVertices;i++)
  {
	bPositions1[k]=bPositions[i];
	bColors1[k]=bColors[i];

	std::cout<<bPositions1[i][0]<<std::endl;

	bNormals1[k]=bNormals[i];
	bTextures1[k]=bTextures[i];
	std::cout<<bTextures1[i][0]<<std::endl;
	k++;
  }

  int index=0;

  for(int i=0; i<backgroundSize.size();i++)
  {
	  GLuint newTex=LoadTexture(bTexFileNames[i].c_str(),bTexFileSize[i][0],bTexFileSize[i][1]);
	  backTex.push_back(newTex);

	  GLuint vaoTemp,vboTemp;

	  glGenVertexArrays (1, &vaoTemp);
	  glBindVertexArray (vaoTemp);

	  glGenBuffers (1, &vboTemp);
  	  glBindBuffer (GL_ARRAY_BUFFER, vboTemp);

  	  //Copy the points into the current buffer  (16 multplied to give size in bytes for integers)
   	  glBufferData (GL_ARRAY_BUFFER,56*(backgroundSize[i]), NULL, GL_STATIC_DRAW);
  	  glBufferSubData( GL_ARRAY_BUFFER, 0, 16*(backgroundSize[i]), &bPositions1[index]);
  	  glBufferSubData( GL_ARRAY_BUFFER, 16*(backgroundSize[i]), 16*(backgroundSize[i]), &bColors1[index]);
  	  glBufferSubData( GL_ARRAY_BUFFER, 16*2*(backgroundSize[i]), 16*(backgroundSize[i]), &bNormals1[index]);
  	  glBufferSubData( GL_ARRAY_BUFFER, 16*3*(backgroundSize[i]), 8*(backgroundSize[i]), &bTextures1[index]);

	  index=index+backgroundSize[i];

 	  // set up vertex arrays
// 	  GLuint vPos = glGetAttribLocation( shaderProgram, "vPosition" );
  	  glEnableVertexAttribArray( vPosition );
  	  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
// 	  GLuint vCol = glGetAttribLocation( shaderProgram, "vColor" ); 
 	  glEnableVertexAttribArray( vColor );
  	  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((size_t)(16*backgroundSize[i])));

// 	  GLuint vNor = glGetAttribLocation( shaderProgram, "vNormal" ); 
	  glEnableVertexAttribArray( vNormal );
	  glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((size_t)(32*backgroundSize[i])));

// 	  GLuint vTex = glGetAttribLocation( shaderProgram, "vTexture" ); 
	  glEnableVertexAttribArray( vTexture );
	  glVertexAttribPointer( vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((size_t)(48*backgroundSize[i])));

	  vao.push_back(vaoTemp);
	  vbo.push_back(vboTemp);
  }
}

void initBuffersGL(void)
{
	std::vector<std::string> hierarchyFileList;
	std::string backgroundFileName;

    loadScene("myscene.txt",vPositions,vColors,vNormals,vTextures, modelSize,hierarchyFileList,gLights, lightNames,camera,modelNames,use_texture,texFileNames,texFileSize, backgroundFileName );

	std::cout<<backgroundFileName;

	//store the vertices in glm array
    int numVertices = vPositions.size(), k=0;

	glm::vec4 vPositions1[numVertices];
	glm::vec4 vColors1[numVertices];

	glm::vec4 vNormals1[numVertices];

	glm::vec2 vTextures1[numVertices];

	for(int i=0;i<numVertices;i++)
	{
		vPositions1[k]=vPositions[i];
		vColors1[k]=vColors[i];
		vNormals1[k]=vNormals[i];
		vTextures1[k]=vTextures[i];
		k++;
	}


	int offset=0;
    // Load shaders and use the resulting shader program
	std::string vertex_shader_file("03_vshader.glsl");
	std::string fragment_shader_file("03_fshader.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    shaderProgram = csX75::CreateProgramGL(shaderList);
    glUseProgram( shaderProgram );

	vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
	vNormal = glGetAttribLocation( shaderProgram, "vNormal" ); 
	vTexture = glGetAttribLocation( shaderProgram, "vTexture" ); 

	normalMatrix =  glGetUniformLocation( shaderProgram, "normalMatrix");
	uModelMatrix = glGetUniformLocation( shaderProgram, "uModelMatrix");
	viewMatrix = glGetUniformLocation( shaderProgram, "viewMatrix");

	useTexture=glGetUniformLocation( shaderProgram, "useTexture");
	numLights=glGetUniformLocation( shaderProgram, "numLights");
	cameraLocation=glGetUniformLocation( shaderProgram, "cameraLocation");

	for(int i=0; i<gLights.size();i++)
	{
		std::ostringstream ss;
		ss << i;
		std::string I = ss.str();

		std::vector<GLuint> newID;
		GLuint on    = glGetUniformLocation( shaderProgram, ("allLights["+I+"].on").c_str());
		GLuint position    = glGetUniformLocation( shaderProgram, ("allLights["+I+"].position").c_str());	
		GLuint intensities = glGetUniformLocation( shaderProgram, ("allLights["+I+"].intensities").c_str());	
		GLuint attenuation = glGetUniformLocation( shaderProgram, ("allLights["+I+"].attenuation").c_str());	
		GLuint ambientCoefficient = glGetUniformLocation( shaderProgram, ("allLights["+I+"].ambientCoefficient").c_str());	
		GLuint coneAngle  =  glGetUniformLocation( shaderProgram, ("allLights["+I+"].coneAngle").c_str());
		GLuint coneDirection  =  glGetUniformLocation( shaderProgram, ("allLights["+I+"].coneDirection").c_str());
		
		newID.push_back(on);
		newID.push_back(position);	
		newID.push_back(intensities);
		newID.push_back(attenuation);
		newID.push_back(ambientCoefficient);
		newID.push_back(coneAngle);
		newID.push_back(coneDirection);

		gLightsID.push_back(newID);					
	}

	for(int i=0; i<hierarchyFileList.size(); i++)
	{
		std::vector<csX75::HNode*> newNodeList;
		std::vector<std::string> newNodeNames;
		std::vector<glm::vec3> newTPar;
		std::vector<glm::vec3> newRPar;
		std::vector<glm::vec3> newTFlags;
		std::vector<glm::vec3> newRFlags;

		loadHierarchy(hierarchyFileList[i],newNodeNames,newTPar,newTFlags,newRPar,newRFlags, newNodeList, vPositions1, vColors1,vNormals1,vTextures1, modelSize[i],offset, use_texture[i],texFileNames[i],texFileSize[i]);
		nodeList.push_back(newNodeList);
		nodeNames.push_back(newNodeNames);
		tParameters.push_back(newTPar);
		rParameters.push_back(newRPar);
		tFlags.push_back(newTFlags);
		rFlags.push_back(newRFlags);
	}

	//set camera
	eye=camera[0];
	glm::vec4 up=camera[1];
	glm::vec4 lookAt=camera[2];
	glm::vec4 RTNF=camera[3];
   
    camera_location=eye;

    glm::vec3 eye3 =glm::vec3(eye[0],eye[1],eye[2]);
    glm::vec3 lookAt3 = glm::vec3(lookAt[0], lookAt[1], lookAt[2]);
    glm::vec3 up3 = glm::vec3(up[0], up[1], up[2]);

    glm::vec3 n = -(lookAt3 - eye3);
    float nMod=sqrt(glm::dot(n,n));
    n= n/nMod;

    glm::vec3 u = glm::cross(up3,n);
    float uMod = sqrt(glm::dot(u,u));
    u = u/uMod;

    glm::vec3 v=glm::cross(n,u);
    //set the transformation matrices
    wcs2vcsMatrix[0] = glm::vec4(u[0],v[0],n[0],0.0f);
    wcs2vcsMatrix[1] = glm::vec4(u[1],v[1],n[1],0.0f);
    wcs2vcsMatrix[2] = glm::vec4(u[2],v[2],n[2],0.0f);
	wcs2vcsMatrix[3] = glm::vec4(glm::dot(-u,eye3),glm::dot(-v,eye3),glm::dot(-n,eye3),1.0f);

	projection_matrix = glm::frustum(-RTNF[0], RTNF[0], -RTNF[1], RTNF[1], RTNF[2], RTNF[3]);
   	//view_matrix = projection_matrix*wcs2vcsMatrix ;

	initBackground(backgroundFileName);

}

void renderGL(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set up vertex arrays
	matrixStack.clear();


	camera_location = eye + glm::vec4(xpos,ypos,zpos,0.0f);
	rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
	translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(camera_location[0],camera_location[1],camera_location[2]));

	view_matrix = projection_matrix*(rotation_matrix)*translation_matrix;//*wcs2vcsMatrix ;
	
    for(int j=0;j<backgroundSize.size();j++)
    {
		glBindTexture(GL_TEXTURE_2D, backTex[j]);

		glm::mat4 backTransform=glm::mat4(1.0f);

		glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(backTransform));

		//remove the scaling and translation from the model matrix to get normal matrix
		glm::mat3 normal_matrix = glm::transpose (glm::inverse(glm::mat3(backTransform)));
    	glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

		glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));

		glUniform1i(numLights,gLights.size());

		int use_texture=1;
		glUniform1i(useTexture,use_texture);		

		glUniform4fv(cameraLocation, 1,glm::value_ptr(camera_location) );
	
		for(int i=0; i<gLights.size();i++)
		{
			glUniform1i(gLightsID[i][0], gLights[i].on);
			glUniform4fv(gLightsID[i][1], 1,glm::value_ptr(gLights[i].position) );
			glUniform4fv(gLightsID[i][2], 1,glm::value_ptr(gLights[i].intensities) );
			glUniform1f(gLightsID[i][3], gLights[i].attenuation  );
			glUniform1f(gLightsID[i][4], gLights[i].ambientCoefficient );
			glUniform1f(gLightsID[i][5], gLights[i].coneAngle  );
			glUniform4fv(gLightsID[i][6], 1,glm::value_ptr(gLights[i].coneDirection) );		

		}
		glBindVertexArray (vao[j]); 
    	glDrawArrays(GL_TRIANGLES, 0,backgroundSize[j]);
    }

	model_matrix = glm::mat4(1.0f);

	if(mode==0)
	{
		float tx=tParameters[selectedModelID][selectedNodeID][0];
		float ty=tParameters[selectedModelID][selectedNodeID][1];
		float tz=tParameters[selectedModelID][selectedNodeID][2];

		float rx=rParameters[selectedModelID][selectedNodeID][0];
		float ry=rParameters[selectedModelID][selectedNodeID][1];
		float rz=rParameters[selectedModelID][selectedNodeID][2];

		nodeList[selectedModelID][selectedNodeID]->change_parameters(tx, ty, tz, rx, ry, rz);
	}

	matrixStack.push_back(model_matrix);

	for(int i=0; i<nodeList.size(); i++)
	{
    	nodeList[i][0]->render_tree();
	}
}

int main(int argc, char** argv)
{
	//! The pointer to the GLFW window
	GLFWwindow* window; 

	//! Setting up the GLFW Error callback
	glfwSetErrorCallback(csX75::error_callback);

	//! Initialize GLFW
	if (!glfwInit())
	return -1;

	//We want OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//This is for MacOSX - can be omitted otherwise
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	//We don't want the old OpenGL 
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	//! Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(720, 720, "Assignment3:Star Wars", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//! Make the window's context current 
	glfwMakeContextCurrent(window);

	//Initialize GLEW
	//Turn this on to get Shader based OpenGL
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cerr<<"GLEW Init Failed : %s"<<std::endl;
	}

	//Keyboard Callback
	glfwSetKeyCallback(window, csX75::key_callback);
	//Framebuffer resize callback
	glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);



	//glfwSetCursorPosCallback(window, csX75::cursor_callback);
	//Initialize GL state
	csX75::initGL();
	initBuffersGL();



	// Loop until the user closes the window
	while (glfwWindowShouldClose(window) == 0)
	{

  		// Render here
	  
		renderGL();

		// Swap front and back buffers
		glfwSwapBuffers(window);
	  
		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

//-------------------------------------------------------------------------

