///
//  Textures
//
//  Simple module for setting up texture mapping parameters.
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Last updated 2019/04/13 by wrc.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Boyuan Li
//
//  This code can be compiled as either C or C++.
///

#ifdef __cplusplus
#include <iostream>
#else
#include <stdio.h>
#endif

#include "Textures.h"
#include "Shapes.h"

// this is here in case you are using SOIL;
// if you're not, it can be deleted.
#include <SOIL.h>
#include<iostream>

#ifdef __cplusplus
using namespace std;
#endif

// Add any global definitions and/or variables you need here.
GLuint table_img_loc;


///
// This function loads texture data for the GPU.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
///
void loadTextures( void )
{
	//use SOIL to import image files
	glEnable(GL_TEXTURE_2D);
	table_img_loc = SOIL_load_OGL_texture(
		"table.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_TEXTURE_REPEATS
	);


	if (table_img_loc == 0) {
		printf("SOIL loading error: '%s'\n",
			SOIL_last_result());
	}
}

///
// This function sets up the parameters for texture use.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param obj - The object type of the object being drawn
///
void setUpTextures( GLuint program, int obj )
{
	glUseProgram(program);
	//bind texture 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, table_img_loc);
	
	glActiveTexture(GL_TEXTURE2);

	//get sampler location
	GLint happy_loc = glGetUniformLocation(program, "happy_img");

	if (happy_loc == -1) {
		std::cout << "can't find happy loc, obj id=" << obj << endl;
	}

	//assign sampler with binded texture
	glUniform1i(happy_loc, 0);

	//get ka kd ks location in shader
	GLint ka_loc = glGetUniformLocation(program, "ka");
	GLint kd_loc = glGetUniformLocation(program, "kd");
	GLint ks_loc = glGetUniformLocation(program, "ks");
	GLint specular_exponent_loc = glGetUniformLocation(program, "specular_exponent");

	//pass light properties to shader
	GLint light_color_loc = glGetUniformLocation(program, "light_color");
	GLint light_position_loc = glGetUniformLocation(program, "light_position");
	GLint light_ambient_loc = glGetUniformLocation(program, "light_ambient");
	glUniform4f(light_color_loc, 1.0, 1.0, 1.0, 1.0);
	glUniform3f(light_position_loc, 3.0, 9.0, 2.0);
	glUniform4f(light_ambient_loc, 0.5, 0.5, 0.5, 1.0);

	//pass ka kd ks variable to shader
	/*
	Reflective characteristics of the quad:
	---------------------------------------
	Ambient reflection coefficient (ka) = 0.7
	Diffuse reflection coefficient (kd) = 0.7
	Specular reflection coefficient (ks) = 1.0
	Specular exponent = 40.0
	*/
	// Since textures are only for quad, we ignore the obj input
	glUniform1f(ka_loc, 0.7);
	glUniform1f(kd_loc, 0.7);
	glUniform1f(ks_loc, 1.0);
	glUniform1f(specular_exponent_loc, 40);


}
