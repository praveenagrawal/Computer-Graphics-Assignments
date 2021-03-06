//Citation: Most of the part of this code segment is taken from the color_cube demo file uploaded for CS475/CS675 course in IITB.
#include "main.hpp"

GLuint shaderProgram;
GLuint vbo, vao, vbo2, vao2;

glm::mat4 rotation_matrix;
glm::mat4 scale_matrix;
glm::mat4 ortho_matrix;
glm::mat4 modelview_matrix;
GLuint uModelViewMatrix;

//-----------------------------------------------------------------

int tri_idx=0;
std::vector<glm::vec4> v_positions;
std::vector<glm::vec4> v_colors;
std::vector<glm::vec4> ModelPositions;
std::vector<glm::vec4> ModelColors;



void InitModelBuffersGL(void)
{
	if(RemovePoint == 1.0)
	{
		RemovePoint = 0.0;
		ModelPositions.pop_back();
		ModelColors.pop_back();
	}
	if(AddPoint == 1.0)
	{
		glm::mat4 modelview_inverse=glm::inverse(modelview_matrix);
		glm::vec4 newPoint = modelview_inverse*glm::vec4(cursorX, cursorY, 0.0f, 1.0f);
		ModelPositions.push_back(newPoint);
		ModelColors.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		AddPoint = 0.0;
	}
	glm::vec4 ModelPositions1[int(NumPoints)];
	glm::vec4 ModelColors1[int(NumPoints)];
	for(int i=0,k=0;i<ModelPositions.size();i++,k++)
	{
		ModelPositions1[k]=ModelPositions[i];
		ModelColors1[k]=ModelColors[i];
	}
    //Ask GL for a Vertex Attribute Object (vao)
    glGenVertexArrays (1, &vao2);
    //Set it as the current array to be used by binding it
    glBindVertexArray (vao2);
	glGenBuffers (1, &vbo2);
	glBindBuffer (GL_ARRAY_BUFFER, vbo2);
    glBufferData (GL_ARRAY_BUFFER,sizeof(ModelPositions1) + sizeof(ModelColors1), NULL, GL_STATIC_DRAW);
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(ModelPositions1), ModelPositions1);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(ModelPositions1), sizeof(ModelColors1), ModelColors1);
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
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ModelPositions1)) );
    uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");

}
  
void initBuffersGL(void)
{
  if(LoadModel == 1.0)
  {
	v_positions.clear();
	v_colors.clear();
       load_model(fileName,v_positions,v_colors);
       LoadModel=0.0;
       std::cout<<"Model Loaded \n";
  }
  if(ClosePolygon == 1.0)
  {
  	  for (int i=1;i<=ModelPositions.size()-2;i++)
  	  {
  	  	v_positions.push_back(ModelPositions[0]);
  	  	v_positions.push_back(ModelPositions[i]);
  	  	v_positions.push_back(ModelPositions[i+1]);
	        v_colors.push_back(ModelColors[0]);
	        v_colors.push_back(ModelColors[i]);
	        v_colors.push_back(ModelColors[i+1]);
      }
      	ModelPositions.clear();
	    ModelColors.clear();
	    NumPoints = 0;
        ClosePolygon = 0.0;
  }
  //CORRECTION CODE ADDED HERE	
  int num_vertices = v_positions.size(), k=0;
	glm::vec4 v_positions1[num_vertices];
	glm::vec4 v_colors1[num_vertices];
	for(int i=0;i<v_positions.size();i++)
	{
		v_positions1[k]=v_positions[i];
		v_colors1[k]=v_colors[i];
		k++;
	}
  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);

  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER,sizeof(v_positions1) + sizeof(v_colors1), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions1), v_positions1);
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions1), sizeof(v_colors1), v_colors1);

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
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions1)) );

  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
}

void renderGL(void)
{
 	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set up vertex arrays
	glBindVertexArray (vao);
    rotation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xpos,ypos,zpos));
    rotation_matrix = glm::rotate(rotation_matrix, xrot, glm::vec3(1.0f,0.0f,0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
  
    ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
    modelview_matrix = ortho_matrix * rotation_matrix;
    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  
    // Draw 
    glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
	
	    // set up vertex arrays
    glBindVertexArray (vao2);
     if(NumPoints == 1 && ClosePolygon == 0.0)
         glDrawArrays(GL_POINTS, 0, ModelPositions.size());
     else if(NumPoints >= 2)
      	glDrawArrays(GL_LINE_STRIP, 0, ModelPositions.size());
}

int main(int argc, char** argv)
{
  std::cout<<"Press L to load a model file or press M to enter the modelling mode \n";
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
  
  glfwSetMouseButtonCallback(window, csX75::mouse_callback);

  
  //glfwSetCursorPosCallback(window, csX75::cursor_callback);
  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
     if(LoadModel == 1.0)
     {
	initBuffersGL();
     }
     if(SaveModel == 1.0)
     {
	save_model(fileName,v_positions,v_colors);
	std::cout<<"Model Saved \n";
	SaveModel=0.0;
     }
     if(AddPoint == 1 || RemovePoint == 1)
     {	//Vertex entered in Modelling mode
      	InitModelBuffersGL();
     }  
     if(ClosePolygon == 1.0)
     {
     	initBuffersGL();
     }
     	
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

