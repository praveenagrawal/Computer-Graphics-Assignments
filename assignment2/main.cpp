//Citation: Most of the part of this code segment is taken from the color_cube demo file uploaded for CS475/CS675 course in IITB.
//The code from clipping is adopted from wikipedia's 2D clipping code
#include "main.hpp"

GLuint shaderProgram;
//GLuint vbo1, vao1, vbo2, vao2, vbo3, vao3;
std::vector<GLuint> vbo, vao;
GLuint vaof, vbof;

glm::mat4 rotation_matrix;
glm::mat4 frustumRotationMatrix;
glm::mat4 scale_matrix;
glm::mat4 ortho_matrix;
glm::mat4 modelview_matrix;
glm::mat4 frustumViewMatrix;
glm::mat4 wcs2vcsMatrix, vcs2ccsMatrix, ccs2dcsMatrix;
GLuint uModelViewMatrix;

GLuint  normalizeFlag;
GLint normalize_flag;
//-----------------------------------------------------------------

int tri_idx=0;
std::vector<glm::vec4> vPositions;
std::vector<glm::vec4> vColors;
std::vector<int> modelSize;
std::vector<int> modelSizeClipped;

glm::vec4 eye;
glm::vec4 lookAt;
glm::vec4 up;
glm::vec4 lrtb, lrtbFar;
glm::vec3 n,u,v;

float N,F;

// device screen boundaries
float rMax=4.0f, lMax=-4.0f, tMax=4.0f, bMax=-4.0f;
	
  
void initBuffersGL(void)
{
    loadScene("myscene.scn",vPositions,vColors,modelSize, eye, lookAt, up, lrtb, N, F);

    modelSizeClipped=modelSize;

    //define vcs
    glm::vec3 eye3 =glm::vec3(eye[0],eye[1],eye[2]);
    glm::vec3 lookAt3 = glm::vec3(lookAt[0], lookAt[1], lookAt[2]);
    glm::vec3 up3 = glm::vec3(up[0], up[1], up[2]);

    n = -(lookAt3 - eye3);
    float nMod=sqrt(glm::dot(n,n));
    n= n/nMod;

    u = glm::cross(up3,n);
    float uMod = sqrt(glm::dot(u,u));
    u = u/uMod;

    v=glm::cross(n,u);
    //set the transformation matrices
    wcs2vcsMatrix[0] = glm::vec4(u[0],v[0],n[0],0.0f);
    wcs2vcsMatrix[1] = glm::vec4(u[1],v[1],n[1],0.0f);
    wcs2vcsMatrix[2] = glm::vec4(u[2],v[2],n[2],0.0f);
    
    wcs2vcsMatrix[3] = glm::vec4(glm::dot(-u,eye3),glm::dot(-v,eye3),glm::dot(-n,eye3),1.0f);

    vcs2ccsMatrix[0] = glm::vec4((2*N)/(lrtb[1]-lrtb[0]),0.0f,0.0f,0.0f);
    vcs2ccsMatrix[1] = glm::vec4(0.0f, (2*N)/(lrtb[2]-lrtb[3]),0.0f,0.0f);
    vcs2ccsMatrix[2] = glm::vec4((lrtb[1]+lrtb[0])/(lrtb[1]-lrtb[0]), (lrtb[2]+lrtb[3])/(lrtb[2]-lrtb[3]),(F+N)/(F-N),-1.0f);
    vcs2ccsMatrix[3] = glm::vec4(0.0f, 0.0f,2*F*N/(F-N),0.0f);
	
    ccs2dcsMatrix[0] = glm::vec4((rMax-lMax)/2, 0.0f, 0.0f, 0.0f); 
    ccs2dcsMatrix[1] = glm::vec4(0.0f, (tMax-bMax)/2, 0.0f, 0.0f); 
    ccs2dcsMatrix[2] = glm::vec4(0.0f, 0.0f, 0.5f, 0.0f); 
    ccs2dcsMatrix[3] = glm::vec4((rMax+lMax)/2, (tMax+bMax)/2, 0.5f, 1.0f); 


    int numVertices = vPositions.size(), k=0;
	glm::vec4 vPositions1[numVertices];
	glm::vec4 vColors1[numVertices];
	for(int i=0;i<numVertices;i++)
	{
		vPositions1[k]=vPositions[i];
		vColors1[k]=vColors[i];
		k++;
	}

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("03_vshader.glsl");
  std::string fragment_shader_file("03_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  //denotes the pointer of the next set of vertices to be added to the buffer
  int index=0;  

  //Initialise buffers
  for(int i=0; i<modelSize.size();i++)
  {
	  GLuint vaoTemp,vboTemp;

	  glGenVertexArrays (1, &vaoTemp);
	  glBindVertexArray (vaoTemp);

	  glGenBuffers (1, &vboTemp);
  	  glBindBuffer (GL_ARRAY_BUFFER, vboTemp);

  	  //Copy the points into the current buffer  (16 multplied to give size in bytes for integers)
   	  glBufferData (GL_ARRAY_BUFFER,16*2*(modelSize[i]), NULL, GL_STATIC_DRAW);
  	  glBufferSubData( GL_ARRAY_BUFFER, 0, 16*(modelSize[i]), &vPositions1[index]);
  	  glBufferSubData( GL_ARRAY_BUFFER, 16*(modelSize[i]), 16*(modelSize[i]), &vColors1[index]);

	  index=index+modelSize[i];

 	  // set up vertex arrays
 	  GLuint vPos = glGetAttribLocation( shaderProgram, "vPosition" );
  	  glEnableVertexAttribArray( vPos );
  	  glVertexAttribPointer( vPos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
 	  GLuint vCol = glGetAttribLocation( shaderProgram, "vColor" ); 
 	  glEnableVertexAttribArray( vCol );
  	  glVertexAttribPointer( vCol, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((size_t)(16*modelSize[i])));

	  vao.push_back(vaoTemp);
	  vbo.push_back(vboTemp);
  }

  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
  normalizeFlag = glGetUniformLocation( shaderProgram, "normalizeFlag");
}

void initBufferFrustrumGL(void)
{
    float factor = (F)/(N);
    lrtbFar = glm::vec4(factor*lrtb[0], factor*lrtb[1], factor*lrtb[2], factor*lrtb[3]);
    int num_vertices = 32;
    glm::vec4 eyeVCS=glm::vec4(0.0f,0.0f,0.0f,1.0f);
    glm::vec4 positionsVCS[32] = { eyeVCS, 
					  glm::vec4(lrtb[0], lrtb[2], -N, 1.0),
					  eyeVCS,
					  glm::vec4(lrtb[0], lrtb[3], -N, 1.0),
					  eyeVCS,
					  glm::vec4(lrtb[1], lrtb[2], -N, 1.0),
					  eyeVCS,
					  glm::vec4(lrtb[1], lrtb[3], -N, 1.0),
					  
					  glm::vec4(lrtb[0], lrtb[2],-N, 1.0),
					  glm::vec4(lrtb[0], lrtb[3], -N, 1.0),
					  glm::vec4(lrtb[0], lrtb[3], -N, 1.0),
					  glm::vec4(lrtb[1], lrtb[3], -N, 1.0),
					  glm::vec4(lrtb[1], lrtb[3], -N, 1.0),
					  glm::vec4(lrtb[1], lrtb[2], -N, 1.0),
					  glm::vec4(lrtb[1], lrtb[2], -N, 1.0),
					  glm::vec4(lrtb[0], lrtb[2], -N, 1.0),
					  
					  glm::vec4(lrtbFar[0], lrtbFar[2], -F, 1.0),
					  glm::vec4(lrtbFar[0], lrtbFar[3], -F, 1.0),
					  glm::vec4(lrtbFar[0], lrtbFar[3], -F, 1.0),
					  glm::vec4(lrtbFar[1], lrtbFar[3], -F, 1.0),
					  glm::vec4(lrtbFar[1], lrtbFar[3], -F, 1.0),
					  glm::vec4(lrtbFar[1], lrtbFar[2], -F, 1.0),
					  glm::vec4(lrtbFar[1], lrtbFar[2], -F, 1.0),
					  glm::vec4(lrtbFar[0], lrtbFar[2], -F, 1.0),
					  
					  glm::vec4(lrtb[0], lrtb[3], -N, 1.0),
					  glm::vec4(lrtbFar[0], lrtbFar[3], -F, 1.0),
					  glm::vec4(lrtb[1], lrtb[3], -N, 1.0),
					  glm::vec4(lrtbFar[1], lrtbFar[3], -F, 1.0),
					  glm::vec4(lrtbFar[0], lrtbFar[2], -F, 1.0),
					  glm::vec4(lrtb[0], lrtb[2], -N, 1.0),
					  glm::vec4(lrtbFar[1], lrtbFar[2], -F, 1.0),
					  glm::vec4(lrtb[1], lrtb[2], -N, 1.0)
					};
	glm::vec4 colors[32] = {glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)
				};


  glm::mat4 vcs2wcsMatrix=glm::inverse(wcs2vcsMatrix);
  glm::vec4 positions[32];
  for(int i=0;i<32;i++)
  {
    positions[i]=((vcs2wcsMatrix))*positionsVCS[i];
  }


  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vaof);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vaof);

  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbof);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbof);
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER,sizeof(positions) + sizeof(colors), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("03_vshader.glsl");
  std::string fragment_shader_file("03_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(positions)) );

  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
  normalizeFlag = glGetUniformLocation( shaderProgram, "normalizeFlag");
}

void clip()
{
	glm::vec4 vPositionsTransformed[vPositions.size()];

	glm::mat4 wcs2ndcsMatrix = rotation_matrix*vcs2ccsMatrix*frustumRotationMatrix*wcs2vcsMatrix;
	for(int i=0;i< vPositions.size(); i++)
	{
		vPositionsTransformed[i]=wcs2ndcsMatrix*vPositions[i];
		if(vPositionsTransformed[i][3]<0)
		{vPositionsTransformed[i][3]*=-1.0;}
		vPositionsTransformed[i][0]/=vPositionsTransformed[i][3];
		vPositionsTransformed[i][1]/=vPositionsTransformed[i][3];
		vPositionsTransformed[i][2]/=vPositionsTransformed[i][3];
		vPositionsTransformed[i][3]/=vPositionsTransformed[i][3];
	}

	std::vector<glm::vec4> vPositionsClipped;
	std::vector<glm::vec4> vColorsClipped;

	int numTriangles = vPositions.size()/3;

	int currentModel=0;
	int cumulativeSize=0;
	int cumulativeModelSize=0;

	for(int i=0; i<numTriangles;i++)
	{
	    std::vector<glm::vec4> newPolygon;
	    std::vector<glm::vec4> newPolygonColors;
	    for(int j=0; j<3; j++)
	    {
		float x0=vPositionsTransformed[3*i+j][0], y0=vPositionsTransformed[3*i+j][1], z0=vPositionsTransformed[3*i+j][2];
		float x1=vPositionsTransformed[3*i+(j+1)%3][0], y1=vPositionsTransformed[3*i+(j+1)%3][1], 
		                                                              z1=vPositionsTransformed[3*i+(j+1)%3][2];
	
		bool accept=CohenSutherlandLineClipAndDraw(x0,y0,z0,x1,y1,z1);
		if(accept) 
		{
			newPolygon.push_back(glm::vec4(x0,y0,z0,1.0f));
			newPolygonColors.push_back(vColors[3*i+j]);	
			newPolygon.push_back(glm::vec4(x1,y1,z1,1.0f));
			newPolygonColors.push_back(vColors[3*i+(j+1)%3]);
		}

	     }
	     std::vector<glm::vec4> newPolygon1;
	     std::vector<glm::vec4> newPolygonColors1;

	     for(int j=0; j<newPolygon.size(); j++)
	     {
		  bool alreadyPresent=false;
		  for(int k=0; k<newPolygon1.size();k++)
		  {
			if (newPolygon1[k]==newPolygon[j]) 
			{
				alreadyPresent=true;
				break;
			}
		  }
		  if(!alreadyPresent) 
		  {
			newPolygon1.push_back(newPolygon[j]);
			newPolygonColors1.push_back(newPolygonColors[j]);
		  }
	     }

	     for(int j=1; j<newPolygon1.size()-1 && newPolygon1.size()!=0;j++)
	     {
		  vPositionsClipped.push_back(newPolygon1[0]);
		  vPositionsClipped.push_back(newPolygon1[j]);
		  vPositionsClipped.push_back(newPolygon1[j+1]);

		  vColorsClipped.push_back(newPolygonColors1[0]);
		  vColorsClipped.push_back(newPolygonColors1[j]);
		  vColorsClipped.push_back(newPolygonColors1[j+1]);

	     }
		 
	     if(i==((cumulativeModelSize+modelSize[currentModel])/3)-1) 
	     {
		  modelSizeClipped[currentModel]=(vPositionsClipped.size()-cumulativeSize);
		  cumulativeModelSize+=modelSize[currentModel];
		  currentModel++;
		  cumulativeSize=vPositionsClipped.size();
	     }
	}
	//std::cout<<modelSizeClipped[0]<<"	"<<modelSizeClipped[1]<<"	"<<modelSizeClipped[2]<<std::endl;
	if(activateClip == 0)
	{
		vPositionsClipped = vPositions;
		vColorsClipped = vColors;
		modelSizeClipped = modelSize;
	}
        int numVerticesClipped = vPositionsClipped.size(), k=0;
	glm::vec4 vPositionsClipped1[numVerticesClipped];
	glm::vec4 vColorsClipped1[numVerticesClipped];

	glm::mat4 ndcs2wcsMatrix=glm::inverse(wcs2ndcsMatrix);	

     	if(activateClip==1)
	{
		for(int i=0;i<numVerticesClipped;i++)
		{
			vPositionsClipped1[k]=ndcs2wcsMatrix*vPositionsClipped[i];
			vPositionsClipped1[k][0]/=vPositionsClipped1[k][3];
			vPositionsClipped1[k][1]/=vPositionsClipped1[k][3];
			vPositionsClipped1[k][2]/=vPositionsClipped1[k][3];
			vPositionsClipped1[k][3]/=vPositionsClipped1[k][3];
			vColorsClipped1[k]=vColorsClipped[i];
			k++;
		}
	}else {
	        int numVertices = vPositions.size(), k=0;
		for(int i=0;i<numVertices;i++)	
		{
			vPositionsClipped1[k]=vPositions[i];
			vColorsClipped1[k]=vColors[i];
			k++;
		}
	}

  //denotes the pointer of the next set of vertices to be added to the buffer
  int index=0;  

  //Initialise buffers

  for(int i=0; i<modelSizeClipped.size();i++)
  {
//	  GLuint vaoTemp,vboTemp;

//	  glGenVertexArrays (1, &vaoTemp);
	  glBindVertexArray (vao[i]);

//	  glGenBuffers (1, &vboTemp);
  	  glBindBuffer (GL_ARRAY_BUFFER, vbo[i]);

  	  //Copy the points into the current buffer  (16 multplied to give size in bytes for integers)
   	  glBufferData (GL_ARRAY_BUFFER,16*2*(modelSizeClipped[i]), NULL, GL_STATIC_DRAW);
  	  glBufferSubData( GL_ARRAY_BUFFER, 0, 16*(modelSizeClipped[i]), &vPositionsClipped1[index]);
  	  glBufferSubData( GL_ARRAY_BUFFER, 16*(modelSizeClipped[i]), 16*(modelSizeClipped[i]), &vColorsClipped1[index]);

	  index=index+modelSizeClipped[i];

 	  // set up vertex arrays
 	  GLuint vPos = glGetAttribLocation( shaderProgram, "vPosition" );
  	  glEnableVertexAttribArray( vPos );
  	  glVertexAttribPointer( vPos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
 	  GLuint vCol = glGetAttribLocation( shaderProgram, "vColor" ); 
 	  glEnableVertexAttribArray( vCol );
  	  glVertexAttribPointer( vCol, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((size_t)(16*modelSizeClipped[i])));

//	  vao.push_back(vaoTemp);
//	  vbo.push_back(vboTemp);
  }



}

void renderGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set up vertex arrays

	rotation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xpos,ypos,zpos));
	rotation_matrix = glm::rotate(rotation_matrix, xrot, glm::vec3(1.0f,0.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
  
	frustumRotationMatrix = glm::rotate(glm::mat4(1.0f), xfrot, glm::vec3(1.0f,0.0f,0.0f));
	frustumRotationMatrix = glm::rotate(frustumRotationMatrix, yfrot, glm::vec3(0.0f,1.0f,0.0f));
	frustumRotationMatrix = glm::rotate(frustumRotationMatrix, zfrot, glm::vec3(0.0f,0.0f,1.0f));  
  
	ortho_matrix = glm::ortho(-6.0, 6.0, -6.0, 6.0, -100.0, 100.0); //glm::documentation
   	modelview_matrix = ortho_matrix * rotation_matrix;
	frustumViewMatrix = modelview_matrix;
	
	normalize_flag=1;
	if(mode==1)
	{	
		modelview_matrix=modelview_matrix*frustumRotationMatrix*wcs2vcsMatrix;
		frustumViewMatrix=frustumViewMatrix*wcs2vcsMatrix;
	}else if(mode==2)
	{
		modelview_matrix=modelview_matrix*vcs2ccsMatrix*frustumRotationMatrix*wcs2vcsMatrix;
		frustumViewMatrix=frustumViewMatrix*vcs2ccsMatrix*wcs2vcsMatrix;
		normalize_flag=0;	
	}else if(mode==3)
	{
		modelview_matrix=modelview_matrix*vcs2ccsMatrix*frustumRotationMatrix*wcs2vcsMatrix;
		frustumViewMatrix=frustumViewMatrix*vcs2ccsMatrix*wcs2vcsMatrix;		
	}else if(mode==4)
	{
		modelview_matrix=modelview_matrix*ccs2dcsMatrix*vcs2ccsMatrix*frustumRotationMatrix*wcs2vcsMatrix;
		frustumViewMatrix=frustumViewMatrix*ccs2dcsMatrix*vcs2ccsMatrix*wcs2vcsMatrix;
	}

    	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
     	glUniform1i(normalizeFlag, normalize_flag);
  

        clip();
	

    for(int i=0;i<modelSize.size();i++)
    {
	glBindVertexArray (vao[i]);

  
    	// Draw 
    	glDrawArrays(GL_TRIANGLES, 0,modelSizeClipped[i]);
    }
    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(frustumViewMatrix));    
    glBindVertexArray(vaof);
   	glDrawArrays(GL_LINES, 0,32);
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  //This is for MacOSX - can be omitted otherwise
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(512, 512, "Assignment1", NULL, NULL);
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
  	initBufferFrustrumGL();

  std::cout<<"Press 1 to go to VCS, 2 to go to CCS, 3 to go to NDCS and 4 to go to DCS"<<std::endl;
  std::cout<<"Press 'f' to toggle between moving the frustum and moving the world"<<std::endl;
  std::cout<<"Press 'c' to enable/disable clipping"<<std::endl;

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

