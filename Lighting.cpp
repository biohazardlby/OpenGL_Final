///
//  Lighting
//
//  Simple module for setting up Phong illumination/shading.
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

#include "Lighting.h"
#include "Shapes.h"
#include "Shape_Nonorm.h"
#include <iostream>
// Add any global definitions and/or variables you need here.

///
// This function sets up the lighting, material, and shading parameters
// for the Phong shader.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param obj - The object type of the object being drawn
///


void setUpPhong(GLuint program, int obj)
{
	/*
	in vec3 normal;
	uniform vec4 ambient_color;
	uniform vec4 diffuse_color;
	uniform vec4 specular_color;
	uniform float ka;
	uniform float kd;
	uniform float ks;
	uniform float specular_exponent;
	*/
	//get location of uniform variables
	GLint Oa_loc = glGetUniformLocation(program, "Oa");
	GLint Od_loc = glGetUniformLocation(program, "Od");
	GLint Os_loc = glGetUniformLocation(program, "Os");
	GLint ka_loc = glGetUniformLocation(program, "ka");
	GLint kd_loc = glGetUniformLocation(program, "kd");
	GLint ks_loc = glGetUniformLocation(program, "ks");
	GLint specular_exponent_loc = glGetUniformLocation(program, "specular_exponent");

	/*
	Properties of the light source:
	Position = ( 3.0, 9.0, 2.0, 1.0 )
	Color = ( 1.0, 1.0, 1.0, 1.0 )
	Ambient light in the scene:
	Color = ( 0.5, 0.5, 0.5, 1.0 )
	*/

	//set default light
	GLint light_color_loc = glGetUniformLocation(program, "light_color");
	GLint light_position_loc = glGetUniformLocation(program, "light_position");
	GLint light_ambient_loc = glGetUniformLocation(program, "light_ambient");
	glUniform4f(light_color_loc, 1.0, 1.0, 1.0, 1.0);
	glUniform3f(light_position_loc, 3.0, 9.0, 2.0);
	glUniform4f(light_ambient_loc, 0.5, 0.5, 0.5, 1.0);
	switch (obj) {

	case OBJ_TEAPOT:
		glUniform4f(Oa_loc, 0.1, 0.1, 0.1, 1.0);
		glUniform4f(Od_loc, 0.6, 0.6, 0.6, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.5);
		glUniform1f(kd_loc, 0.7);
		glUniform1f(ks_loc, 1.0);
		glUniform1f(specular_exponent_loc, 90);
		break;

	case OBJ_SPHERE:
		glUniform4f(Oa_loc, 0.5, 0.5, 0.5, 1.0);
		glUniform4f(Od_loc, 0.49, 0.99, 0.0, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.5);
		glUniform1f(kd_loc, 0.8);
		glUniform1f(ks_loc, 1.0);
		glUniform1f(specular_exponent_loc, 50.0);
		break;
	case MATL_MUFFIN:
		glUniform4f(Oa_loc, 0.3,0.3,0.3, 1.0);
		glUniform4f(Od_loc, 0.75,0.5,0.1, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.8);
		glUniform1f(kd_loc, 0.8);
		glUniform1f(ks_loc, 0.1);
		glUniform1f(specular_exponent_loc, 2);
		break;
	case MATL_MUFFINCUP:
		glUniform4f(Oa_loc, 0.5, 0.3, 0.05, 1.0);
		glUniform4f(Od_loc, 0.75, 0.5, 0.1, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.8);
		glUniform1f(kd_loc, 0.1);
		glUniform1f(ks_loc, 0.01);
		glUniform1f(specular_exponent_loc, 2);
		break;
	case MATL_APPLE:
		glUniform4f(Oa_loc, 0.2, 0.2, 0.2, 1.0);
		glUniform4f(Od_loc, 1, 0.0, 0.0, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.8);
		glUniform1f(kd_loc, 0.8);
		glUniform1f(ks_loc, 1.0);
		glUniform1f(specular_exponent_loc, 100.0);
		break;
	case MATL_FLOWER:
		glUniform4f(Oa_loc, 0.2, 0.2, 0.2, 1.0);
		glUniform4f(Od_loc, 0.8, 0.0, 0.0, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.8);
		glUniform1f(kd_loc, 0.8);
		glUniform1f(ks_loc, 0.1);
		glUniform1f(specular_exponent_loc,2.0);
		break;
	case MATL_YELLOWFLOWER:
		glUniform4f(Oa_loc, 0.2, 0.5, 0.2, 1.0);
		glUniform4f(Od_loc, 0.7, 0.7, 0.0, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.3);
		glUniform1f(kd_loc, 0.8);
		glUniform1f(ks_loc, 0.1);
		glUniform1f(specular_exponent_loc, 2.0);
		break;
	case MATL_WOOD:
		glUniform4f(Oa_loc, 0.5, 0.3, 0.05, 1.0);
		glUniform4f(Od_loc, 0.75, 0.5, 0.1, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.8);
		glUniform1f(kd_loc, 0.1);
		glUniform1f(ks_loc, 0.01);
		glUniform1f(specular_exponent_loc, 1);
		break;
	case MATL_VASE:
		glUniform4f(Oa_loc, 0.1, 0.25, 0.1, 1.0);
		glUniform4f(Od_loc, 0.5, 0.5, 0.5, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.5);
		glUniform1f(kd_loc, 0.8);
		glUniform1f(ks_loc, 0.6);
		glUniform1f(specular_exponent_loc, 100);
		break;
	case MATL_CUP:
		glUniform4f(Oa_loc, 0.8, 0.8, 0.8, 1.0);
		glUniform4f(Od_loc, 0.5, 0.5, 0.5, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.2);
		glUniform1f(kd_loc, 0.8);
		glUniform1f(ks_loc, 0.6);
		glUniform1f(specular_exponent_loc, 100);
		break;
	case MATL_CANDLE:
		glUniform4f(Oa_loc, 0.8, 0.8, 0.8, 1.0);
		glUniform4f(Od_loc, 0.5, 0.5, 0.75, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.5);
		glUniform1f(kd_loc, 0.3);
		glUniform1f(ks_loc, 0.1);
		glUniform1f(specular_exponent_loc, 100);
		break;
	case MATL_LEAF:
		glUniform4f(Oa_loc, 0.35, 0.5, 0.25, 1.0);
		glUniform4f(Od_loc, 0.35, 0.8, 0.25, 1.0);
		glUniform4f(Os_loc, 1.0, 1.0, 1.0, 1.0);
		glUniform1f(ka_loc, 0.3);
		glUniform1f(kd_loc, 0.4);
		glUniform1f(ks_loc, 0.05);
		glUniform1f(specular_exponent_loc, 1);
		break;
	}
}
