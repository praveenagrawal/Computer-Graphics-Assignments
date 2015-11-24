#version 330

in vec3 fragNormal;
in vec4 fragColor;

//vertex position in world
in vec4 fragVert;

varying vec2 tex;

uniform sampler2D texture;
uniform vec3 cameraPosition;

uniform mat3 normalMatrix;

uniform int numLights;
uniform int useTexture;

uniform struct Light {
   int on;
   vec4 position;
   vec4 intensities;
   float attenuation;
   float ambientCoefficient;
   float coneAngle;
   vec4 coneDirection;
} allLights[10];

out vec4 frag_color;

void main () 
{
  vec4 color=vec4(0.0f);
  if(useTexture==1)
  {
	color=texture2D(texture, tex);
  }
  else
  {
	color=fragColor;
  }
  //Defining Materials
  vec4 materialSpecular = vec4(1.0, 1.0, 1.0, 1.0);
  float shininess = 10.0;

  vec3 surfacePos = vec3(fragVert);
  vec3 surfaceToCamera = normalize(cameraPosition-surfacePos);

  vec3 newColor = vec3(0);


  vec3 normal=normalize(fragNormal);
  for(int i = 0; i < numLights; i++)
  {
    float attenuation = 1.0;
	vec3 surfaceToLight = vec3(1.0,1.0,1.0);

    if(allLights[i].position.w == 0.0) {
        //directional light
		//should we multiply by view matrix??
		surfaceToLight = (allLights[i].position.xyz);
		surfaceToLight = normalize((surfaceToLight)); 

        attenuation = 1.0; //no attenuation for directional lights
    } else {
        //point light
        surfaceToLight = normalize(allLights[i].position.xyz - surfacePos);
        float distanceToLight = length(allLights[i].position.xyz - surfacePos);
        attenuation = 1.0 / (1.0 + allLights[i].attenuation * pow(distanceToLight, 2));

        //cone restrictions (affects attenuation)
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(vec3(allLights[i].coneDirection)))));
		
        if(lightToSurfaceAngle < allLights[i].coneAngle){
            attenuation = attenuation*pow(cos(radians((lightToSurfaceAngle/allLights[i].coneAngle)*60.0)),4);
        }else{
            attenuation = attenuation*pow(cos(radians(((lightToSurfaceAngle-allLights[i].coneAngle)/(90.0-allLights[i].coneAngle))*30.0 +60.0)),4);
        }
    }

	vec3 ambient = allLights[i].ambientCoefficient * color.rgb * vec3(allLights[i].intensities);

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * color.rgb * vec3(allLights[i].intensities);
    
    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), shininess);
    vec3 specular = specularCoefficient * vec3(materialSpecular) * vec3(allLights[i].intensities);

    //linear color (color before gamma correction)
    newColor=newColor+ allLights[i].on*(ambient + attenuation*(diffuse + specular));

  }
 
  frag_color = vec4(newColor,1.0);

}
