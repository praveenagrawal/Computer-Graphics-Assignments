#version 330

in vec4 vPosition;
in vec4 vColor;
in vec4 vNormal;
in vec2 vTexture;

varying vec2 tex;

out vec3 fragNormal;
out vec4 fragColor;
out vec4 fragVert;

uniform mat4 uModelMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;
void main (void) 
{
  gl_Position = viewMatrix*uModelMatrix*vPosition;
  fragVert=uModelMatrix *vPosition;
  fragNormal =(normalMatrix * normalize(vec3(vNormal))); 
  fragColor = vColor; 
  tex = vTexture;
 }
