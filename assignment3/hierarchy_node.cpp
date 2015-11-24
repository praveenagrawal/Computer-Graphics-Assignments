#include "hierarchy_node.hpp"

#include <iostream>

extern GLuint vPosition,vColor,vNormal,vTexture, uModelMatrix,viewMatrix, normalMatrix, cameraLocation;
extern std::vector<glm::mat4> matrixStack;
extern glm::mat4 view_matrix;

extern  std::vector<Light> gLights;
extern std::vector<std::vector<GLuint> > gLightsID;
extern GLuint numLights,useTexture; 
extern glm::vec4 camera_location;

namespace csX75
{

	HNode::HNode(HNode* a_parent, GLuint num_v, glm::vec4* a_vertices, glm::vec4* a_colours,glm::vec4* a_normals,glm::vec2* a_textures, std::size_t v_size, std::size_t c_size,std::size_t n_size,std::size_t t_size, GLuint NuseTexture, std::string texFileName,std::vector<int> texFileSize){

		num_vertices = num_v;
		vertex_buffer_size = v_size;
		color_buffer_size = c_size;
		normal_buffer_size = n_size;
		texture_buffer_size = t_size;

		use_texture=NuseTexture;
		// initialize vao and vbo of the object;

	    tex=LoadTexture(texFileName.c_str(),texFileSize[0],texFileSize[1]);

		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		
		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size+normal_buffer_size+texture_buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size, color_buffer_size, a_colours );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size+color_buffer_size, normal_buffer_size, a_normals );
		//if(NuseTexture==1)
	      	glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size+color_buffer_size+normal_buffer_size, 
												texture_buffer_size, a_textures );		

		//setup the vertex array as per the shader
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));

		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size+color_buffer_size));

		//if(NuseTexture==1)
		//{
			glEnableVertexAttribArray( vTexture );
		    glVertexAttribPointer( vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size+color_buffer_size+normal_buffer_size));
		//}
		// set parent

		if(a_parent == NULL){
			parent = NULL;
		}
		else{
			parent = a_parent;
			parent->add_child(this);
		}

		//initial parameters are set to 0;

		tx=ty=tz=rx=ry=rz=0;

		update_matrices();
	}

	void HNode::update_matrices(){

		rotation = glm::rotate(glm::mat4(1.0f), (rx), glm::vec3(1.0f,0.0f,0.0f));
		rotation = glm::rotate(rotation, (ry), glm::vec3(0.0f,1.0f,0.0f));
		rotation = glm::rotate(rotation, (rz), glm::vec3(0.0f,0.0f,1.0f));

		translation = glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,tz));


	}

	void HNode::add_child(HNode* a_child){
		children.push_back(a_child);

	}

	void HNode::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz){
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz;

		update_matrices();
	}

	void HNode::render(){

		//matrixStack multiply
		glBindTexture(GL_TEXTURE_2D, tex);

		glm::mat4* ms_mult = multiply_stack(matrixStack);

		glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));

		//remove the scaling and translation from the model matrix to get normal matrix
		glm::mat3 normal_matrix = glm::transpose (glm::inverse(glm::mat3(*ms_mult)));
    	glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

		glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));

		glUniform1i(numLights,gLights.size());

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

		glBindVertexArray (vao);
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);
		//std::cout<<num_vertices<<std::endl;
		// for memory 
		delete ms_mult;

	}

	void HNode::render_tree(){
		
		matrixStack.push_back(translation);
		matrixStack.push_back(rotation);

		render();
		for(int i=0;i<children.size();i++){
			children[i]->render_tree();
		}
		matrixStack.pop_back();
		matrixStack.pop_back();

	}

	void HNode::inc_rx(){
		rx++;
		update_matrices();
	}


	void HNode::inc_ry(){
		ry++;
		update_matrices();
	}

	void HNode::inc_rz(){
		rz++;
		update_matrices();
	}

	void HNode::dec_rx(){
		rx--;
		update_matrices();
	}

	void HNode::dec_ry(){
		ry--;
		update_matrices();
	}

	void HNode::dec_rz(){
		rz--;
		update_matrices();
	}


	glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
	
		for(int i=0;i<matStack.size();i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}

}
