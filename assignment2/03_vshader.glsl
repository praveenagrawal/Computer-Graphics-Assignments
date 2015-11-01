#version 130

in vec4 vPosition;
in vec4 vColor;
out vec4 color;
uniform mat4 uModelViewMatrix;
uniform int normalizeFlag;

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  if(normalizeFlag==0)
  {
    gl_Position[0]=gl_Position[0]*gl_Position[3];
    gl_Position[1]=gl_Position[1]*gl_Position[3];
    gl_Position[2]=gl_Position[2]*gl_Position[3];

  }

  color = vColor;
}
