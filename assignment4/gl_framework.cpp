//Citation: Most of the part of this code segment is taken from the color_cube demo file uploaded for CS475/CS675 course in IITB.
#include "gl_framework.hpp"

extern GLuint mode , selectedNodeID, selectedModelID, selectedLightID, saveFrame, recordingMode;
extern glm::vec4 camera_direction, delta_camera_location;

extern std::string fileName;

extern std::vector<std::vector<glm::vec3> >tParameters;
extern std::vector<std::vector<glm::vec3> >rParameters;

extern std::vector<std::vector<glm::vec3> >tFlags;
extern std::vector<std::vector<glm::vec3> >rFlags;

extern std::vector<std::string> modelNames;
extern std::vector<std::vector<std::string> > nodeNames;
extern std::vector<std::string> lightNames;
extern std::vector<Light> gLights;
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
    {
		  glfwSetWindowShouldClose(window, GL_TRUE);
	}
    else if (key == GLFW_KEY_M && action == GLFW_PRESS && recordingMode==1)
    {
		std::cout<<"Model Control Mode entered. Please enter the model number \n";
		for(int i=0;i<modelNames.size();i++)
		{
			std::cout<<"Enter "<<i<<" to select "<<modelNames[i]<<std::endl;
		}

		while(1)
		{
			std::cin>>selectedModelID;
			if(selectedModelID < modelNames.size())
			{
				break;
			}
			else
			{
				std::cout<<"Invalid ID, please enter again \n";
			}
		}
		
		std::cout<<modelNames[selectedModelID]<<" selected. Please select the node to control"<<std::endl;

		for(int i=0;i<nodeNames[selectedModelID].size();i++)
		{
			std::cout<<"Enter "<<i<<" to select "<<nodeNames[selectedModelID][i]<<std::endl;
		}
	
		while(1)
		{
			std::cin>>selectedNodeID;
			if(selectedNodeID<nodeNames[selectedModelID].size())
			{
				break;
			}
			else
			{
				std::cout<<"Invalid ID, please enter again \n";
			}
		}
		std::cout<<nodeNames[selectedModelID][selectedNodeID]<< "selected"<<std::endl;
		
		mode=0;

	}
    else if (key == GLFW_KEY_C && action == GLFW_PRESS && recordingMode==1)
    {
		std::cout<<"Camera Control Mode entered\n";
	
		mode=1;

	}
    else if (key == GLFW_KEY_L && action == GLFW_PRESS && recordingMode==1)
    {
		std::cout<<"Lights Control Mode entered. Please select the light to control \n";
		for(int i=0;i<lightNames.size();i++)
		{
			std::cout<<"Enter "<<i<<" to select "<<lightNames[i]<<std::endl;
		}

		while(1)
		{
			std::cin>>selectedLightID;
			if(selectedLightID < lightNames.size())
			{
				break;
			}
			else
			{
				std::cout<<"Invalid ID, please enter again \n";
			}
		}

		std::cout<<lightNames[selectedLightID]<<" selected"<<std::endl;

		mode=2;

	}
    else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
    {
		if(mode==0)
		{
			if(rFlags[selectedModelID][selectedNodeID][1]==1.0)
				rParameters[selectedModelID][selectedNodeID][1] -=0.05;
		}
		else if(mode==1)
		{
			camera_direction[1] -=0.05;
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), -0.05f, glm::vec3(0.0f,1.0f,0.0f));
				gLights[selectedLightID].coneDirection=rotMat*gLights[selectedLightID].coneDirection;
			}
			else
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), -0.05f, glm::vec3(0.0f,1.0f,0.0f));
				gLights[selectedLightID].position=rotMat*gLights[selectedLightID].coneDirection;
			}
		}
    }
    else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
    {
		if(mode==0)
		{
			if(rFlags[selectedModelID][selectedNodeID][1]==1.0)
				rParameters[selectedModelID][selectedNodeID][1] +=0.05;
		}
		else if(mode==1)
		{
			camera_direction[1] +=0.05;
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), 0.05f, glm::vec3(0.0f,1.0f,0.0f));
				gLights[selectedLightID].coneDirection=rotMat*gLights[selectedLightID].coneDirection;
			}
			else
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), 0.05f, glm::vec3(0.0f,1.0f,0.0f));
				gLights[selectedLightID].position=rotMat*gLights[selectedLightID].coneDirection;
			}
		}
    }
    else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
    {
		if(mode==0)
		{
			if(rFlags[selectedModelID][selectedNodeID][0]==1.0)
				rParameters[selectedModelID][selectedNodeID][0] -=0.05;
		}
		else if(mode==1)
		{
			camera_direction[0] -=0.05;
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), -0.05f, glm::vec3(1.0f,0.0f,0.0f));
				gLights[selectedLightID].coneDirection=rotMat*gLights[selectedLightID].coneDirection;
			}
			else
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), -0.05f, glm::vec3(1.0f,0.0f,0.0f));
				gLights[selectedLightID].position=rotMat*gLights[selectedLightID].coneDirection;
			}
		}
    }
    else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
    {
		if(mode==0)
		{
			if(rFlags[selectedModelID][selectedNodeID][0]==1.0)
				rParameters[selectedModelID][selectedNodeID][0] +=0.05;
		}
		else if(mode==1)
		{
			camera_direction[0] +=0.05;
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), 0.05f, glm::vec3(1.0f,0.0f,0.0f));
				gLights[selectedLightID].coneDirection=rotMat*gLights[selectedLightID].coneDirection;
			}
			else
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), 0.05f, glm::vec3(1.0f,0.0f,0.0f));
				gLights[selectedLightID].position=rotMat*gLights[selectedLightID].coneDirection;
			}
		}
    }
    else if (key == GLFW_KEY_PAGE_UP && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
    {
		if(mode==0)
		{
			if(rFlags[selectedModelID][selectedNodeID][2]==1.0)
				rParameters[selectedModelID][selectedNodeID][2] +=0.05;
		}
		else if(mode==1)
		{
			camera_direction[2] +=0.05;
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), 0.05f, glm::vec3(0.0f,0.0f,1.0f));
				gLights[selectedLightID].coneDirection=rotMat*gLights[selectedLightID].coneDirection;
			}
			else
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), 0.05f, glm::vec3(0.0f,0.0f,1.0f));
				gLights[selectedLightID].position=rotMat*gLights[selectedLightID].coneDirection;
			}
		}      

    }
    else if (key == GLFW_KEY_PAGE_DOWN && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
    {
      
		if(mode==0)
		{
			if(rFlags[selectedModelID][selectedNodeID][2]==1.0)
				rParameters[selectedModelID][selectedNodeID][2] -=0.05;
		}
		else if(mode==1)
		{
			camera_direction[2] -=0.05;
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), -0.05f, glm::vec3(0.0f,0.0f,1.0f));
				gLights[selectedLightID].coneDirection=rotMat*gLights[selectedLightID].coneDirection;
			}
			else
			{
				glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), -0.05f, glm::vec3(0.0f,0.0f,1.0f));
				gLights[selectedLightID].position=rotMat*gLights[selectedLightID].coneDirection;
			}
		}
    }
    else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
    {
		if(mode==0)
		{
			if(tFlags[selectedModelID][selectedNodeID][1]==1.0)
				tParameters[selectedModelID][selectedNodeID][1] +=0.05;
		}
		else if(mode==1)
		{
			delta_camera_location= glm::vec4(0.0f,-0.05f,0.0f, 0.0f);
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				gLights[selectedLightID].position.y +=0.05;
			}
		}
	}
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
	{
		if(mode==0)
		{
			if(tFlags[selectedModelID][selectedNodeID][1]==1.0)
				tParameters[selectedModelID][selectedNodeID][1] -=0.05;
		}
		else if(mode==1)
		{
			delta_camera_location= glm::vec4(0.0f,0.05f,0.0f, 0.0f);
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				gLights[selectedLightID].position.y -=0.05;
			}
		}
	}
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
	{
		if(mode==0)
		{
			if(tFlags[selectedModelID][selectedNodeID][0]==1.0)
				tParameters[selectedModelID][selectedNodeID][0] -=0.05;
		}
		else if(mode==1)
		{
			delta_camera_location= glm::vec4(0.05f,0.0f,0.0f, 0.0f);
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				gLights[selectedLightID].position.x -=0.05;
			}
		}
	}
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
	{
		if(mode==0)
		{
			if(tFlags[selectedModelID][selectedNodeID][0]==1.0)
				tParameters[selectedModelID][selectedNodeID][0] +=0.05;
		}
		else if(mode==1)
		{
			delta_camera_location= glm::vec4(-0.05f,0.0f,0.0f, 0.0f);
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				gLights[selectedLightID].position.x +=0.05;
			}
		}
	}
    else if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
	{
		if(mode==0)
		{
			if(tFlags[selectedModelID][selectedNodeID][2]==1.0)
				tParameters[selectedModelID][selectedNodeID][2] +=0.05;
		}
		else if(mode==1)
		{
			delta_camera_location= glm::vec4(0.0f,0.0f,0.05f, 0.0f);
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				gLights[selectedLightID].position.z +=0.05;
			}
		}
	}
    else if (key == GLFW_KEY_X && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
	{
		if(mode==0)
		{
			if(tFlags[selectedModelID][selectedNodeID][2]==1.0)
				tParameters[selectedModelID][selectedNodeID][2] -=0.05;
		}
		else if(mode==1)
		{
			delta_camera_location= glm::vec4(0.0f,0.0f,-0.05f, 0.0f);
		}
		else if(mode==2)
		{
			if(gLights[selectedLightID].position.w==1.0)
			{
				gLights[selectedLightID].position.z -=0.05;
			}
		}
	}
    else if (key == GLFW_KEY_O && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
	{
		if(mode==2)
		{
			if(gLights[selectedLightID].on==1)
			{
				gLights[selectedLightID].on=0;
			}
			else gLights[selectedLightID].on=1;
		}
	}
    else if (key == GLFW_KEY_K && (action == GLFW_PRESS || action != GLFW_RELEASE) && recordingMode==1)
	{
		std::cout<<"Saving Keyframe \n";
		saveFrame=1;
	}
    else if (key == GLFW_KEY_P && (action == GLFW_PRESS || action != GLFW_RELEASE))
	{
		if (recordingMode==1)
		{
			std::cout<<"Entering Playback Mode \n";
			recordingMode=0;
		}
		else
		{
			std::cout<<"Entering Recording Mode \n";
			recordingMode=1;			
		}
	}


 }
  	
};   
  


