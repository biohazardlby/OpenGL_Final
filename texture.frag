#version 130

//
// Texture mapping vertex shader
//
// Contributor:  Boyuan Li
//

// INCOMING DATA
in vec3 vPosition_out;
in vec3 vNormal_out;
in vec3 light_position_out;
in vec2 texCoord;

uniform vec4 light_color;
uniform vec4 light_ambient;
uniform float ka;
uniform float kd;
uniform float ks;
uniform float specular_exponent;
uniform float color;

uniform sampler2D happy_img;
uniform sampler2D angry_img;

// ADD VARIABLES HERE for all data being sent from your vertex shader

// OUTGOING DATA

out vec4 finalColor;

///
// Main function
///

void main()
{
	vec3 vNormal = normalize(vNormal_out);
	vec3 vertex_2_light = normalize(light_position_out - vPosition_out);
	vec3 vertex_2_camera = -normalize(vPosition_out);


	vec4 Oa;
	vec4 Od;
	vec4 Os;
	//check front and back
	if (gl_FrontFacing){
		Oa = texture(happy_img,texCoord);
		Od = texture(happy_img,texCoord);
		Os = texture(happy_img,texCoord);
	} else {
		Oa = texture(angry_img,texCoord);
		Od = texture(angry_img,texCoord);
		Os = texture(angry_img,texCoord);
	}

	//calculate ambient light
	vec4 ambient = light_ambient * Oa * ka;
	//calculate diffuse light
	vec4 diffuse = Od * kd * max(dot(vNormal, vertex_2_light),0);
	//calcualte specular light
	vec3 H = (vertex_2_light + vertex_2_camera) /length(vertex_2_light + vertex_2_camera);
	vec3 R = reflect(-vertex_2_light,vNormal);
	vec4 specular = vec4(1,1,1,1) * ks* pow(max(dot(H,vNormal),0),specular_exponent);
	//using phong model to match appearance 
	specular = Os * ks* pow(max(dot(R,vertex_2_camera),0),specular_exponent);
	finalColor  = ambient + diffuse + specular;

}
