//Citation: Most of the part of this code segment is taken from the color_cube demo file uploaded for CS475/CS675 course in IITB.
#include "gl_framework.hpp"

extern GLfloat xrot,yrot,zrot,xpos,ypos,zpos,getPos,Mode,cursorX,cursorY,cursorZ,NumPoints,AddPoint,ClosePolygon, RemovePoint,ClearBuffer,LoadModel,SaveModel;

extern std::string fileName;

GLfloat ShiftDown = 0.0;
namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action != GLFW_RELEASE))
      yrot -= 0.1;
    else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action != GLFW_RELEASE))
      yrot += 0.1;
    else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action != GLFW_RELEASE))
      xrot -= 0.1;
    else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action != GLFW_RELEASE))
      xrot += 0.1;
    else if (key == GLFW_KEY_PAGE_UP && (action == GLFW_PRESS || action != GLFW_RELEASE))
      zrot += 0.1;
    else if (key == GLFW_KEY_PAGE_DOWN && (action == GLFW_PRESS || action != GLFW_RELEASE))
      zrot -= 0.1;
    else if (key == GLFW_KEY_R && (action == GLFW_PRESS || action != GLFW_RELEASE))
    {
	  xpos=0;ypos=0;zpos=0;
    }
    else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action != GLFW_RELEASE))
    	ypos+=0.1;
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action != GLFW_RELEASE))
    	ypos-=0.1;
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action != GLFW_RELEASE))
    	xpos-=0.1;
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action != GLFW_RELEASE))
    	xpos+=0.1;
    else if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action != GLFW_RELEASE))
    	zpos+=0.1;
    else if (key == GLFW_KEY_X && (action == GLFW_PRESS || action != GLFW_RELEASE))
     	zpos-=0.1;
    else if (key == GLFW_KEY_M && action == GLFW_PRESS && Mode == 0)
    {	
    	Mode = 1;	//Activate Modelling Mode   
    	std::cout<<"MODELLING MODE ACTIVATED \n";
    	ClosePolygon = 0.0;
    }
    else if (key == GLFW_KEY_I && action == GLFW_PRESS && Mode == 1)
    {
      	Mode = 0;		//Activate Inspection mode 
      	getPos = 0;		//Do not store the clicked point anymore
      	std::cout<<"INSPECTION MODE ACTIVATED \n";
      	ClearBuffer = 1.0;
      	NumPoints = 0.0;
    }
    else if ((key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)  || (key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS))
    {
    	;//Remove previous vertex from model
    	ShiftDown = 1.0;	//Shift Key Pressed
    }
    else if ((key == GLFW_KEY_LEFT_SHIFT && action != GLFW_PRESS)  || (key == GLFW_KEY_RIGHT_SHIFT && action != GLFW_PRESS))
    {
    	;//Remove previous vertex from model
    	ShiftDown = 0.0;	//Shift Key Released
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
	if(Mode==0.0){
	std::cout<<"Enter the file name (without spaces) \n";
	std::cin >> fileName;
	LoadModel=1.0;
	} else std::cout<<"Please enter Inspection mode (press I) to load the file \n";
    }
    else if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
	if(Mode==0.0){
	std::cout << "Enter the file name for saving (without spaces) \n";
	std::cin >> fileName;
	SaveModel=1.0;
	} else std::cout<<"Enter the Inspection mode (press M) to save the model \n";
    }
 }
  void mouse_callback(GLFWwindow* window, int button, int action, int mods)
  {
  	if (button == GLFW_MOUSE_BUTTON_LEFT  && action == GLFW_PRESS && Mode == 1 && ShiftDown == 0.0)
    {
    	getPos = 1;
		double X,Y;
        glfwGetCursorPos(window, &X, &Y);
    	NumPoints++;
    	cursorX = float(((X-256)/256.0)*1.0);
    	cursorY = float((-(Y-256)/256.0)*1.0);
    	cursorZ = 0;
    	getPos = 0;
    	AddPoint = 1.0;
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT  && action == GLFW_PRESS && Mode == 1 )
   	{
   		if(NumPoints>=3)
	   		ClosePolygon = 1.0;
	   	else
	   		std::cout<<"Draw at least 3 vertices \n";
   	}
   	else if(button == GLFW_MOUSE_BUTTON_LEFT  && action == GLFW_PRESS && Mode == 1 && ShiftDown == 1.0)
   	{
   			//Remove previous point
   		if(NumPoints!=0)
   		{
   			RemovePoint = 1.0;
   			NumPoints--;	//Number of points in the unclosed polygon decreases by 1
   			std::cout<<"Last Point Removed \n";
   		}
   	}
  }  	
};   
  


