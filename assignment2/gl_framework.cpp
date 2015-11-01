//Citation: Most of the part of this code segment is taken from the color_cube demo file uploaded for CS475/CS675 course in IITB.
#include "gl_framework.hpp"

extern GLfloat xrot,yrot,zrot,xfrot,yfrot,zfrot,xpos,ypos,zpos;
extern GLuint mode, rotateFrustum, activateClip;

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
    {
      if(rotateFrustum==0)	
        yrot -= 0.05;
      else
        yfrot -=0.05;  
    }
    else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action != GLFW_RELEASE))
    {
      if(rotateFrustum==0)	
        yrot += 0.05;
      else
        yfrot +=0.05;
    }
    else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action != GLFW_RELEASE))
    {
      if(rotateFrustum==0)	
        xrot -= 0.05;
      else
        xfrot -=0.05;
    }
    else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action != GLFW_RELEASE))
    {
      if(rotateFrustum==0)	
        xrot += 0.05;
      else
        xfrot +=0.05;
    }
    else if (key == GLFW_KEY_PAGE_UP && (action == GLFW_PRESS || action != GLFW_RELEASE))
    {
      if(rotateFrustum==0)	
        zrot += 0.05;
      else
        zfrot +=0.05;
    }
    else if (key == GLFW_KEY_PAGE_DOWN && (action == GLFW_PRESS || action != GLFW_RELEASE))
    {
      if(rotateFrustum==0)	
        zrot -= 0.05;
      else
        zfrot -=0.05;
    }
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

    else if (key == GLFW_KEY_1 && (action == GLFW_PRESS))
    {
	xpos=0; ypos=0; zpos=0;
	xrot=0; yrot=0; zrot=0;
	xfrot=0; yfrot=0; zfrot=0;
	mode=1;	//VCS
    }	
    else if (key == GLFW_KEY_2 && (action == GLFW_PRESS))
    {
	xpos=0; ypos=0; zpos=0;
	xrot=0; yrot=0; zrot=0;
	xfrot=0; yfrot=0; zfrot=0;	
	mode=2;	//CCS
    }
    else if (key == GLFW_KEY_3 && (action == GLFW_PRESS))
    {
	xpos=0; ypos=0; zpos=0;
	xrot=0; yrot=0; zrot=0;
	xfrot=0; yfrot=0; zfrot=0;	
	mode=3;	//NDCS
    }
    else if (key == GLFW_KEY_4 && (action == GLFW_PRESS))
    {
	xpos=0; ypos=0; zpos=0;
	xrot=0; yrot=0; zrot=0;
	xfrot=0; yfrot=0; zfrot=0;	
	mode=4;	//DCS
    }
    else if (key == GLFW_KEY_F && (action == GLFW_PRESS))
    {
    if(rotateFrustum == 0) rotateFrustum=1;
    else rotateFrustum = 0;
    }  
    else if (key == GLFW_KEY_C && (action == GLFW_PRESS))
    {
	if(activateClip==0) activateClip=1;
	else activateClip=0;
    }   
 }
  	
};   
  


