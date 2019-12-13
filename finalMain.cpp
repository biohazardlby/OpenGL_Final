///
//  textingMain.cpp
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel.
//  Updated 2018/11/28 by wrc.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Main program for lighting/shading/texturing assignment
//
//  Contributor:Boyuan Li
///

#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Buffers.h"
#include "ShaderSetup.h"
#include "Canvas.h"
#include "Shapes.h"
#include "Shape_Nonorm.h"
#include "Viewing.h"
#include "Lighting.h"
#include "Textures.h"

using namespace std;

///
// Drawing-related variables
///

// our drawing canvas
Canvas *canvas;

// dimensions of the drawing window
int w_width  = 600;
int w_height = 600;

// Camera variables

Tuple eye = { 0.0f, 1.25f, 6.5f };
Tuple lookat = { 0.0f, 0.8f, 0.0f };
Tuple up = { 0.0f, 5.0f, 0.0f };

// transform info of differnt object
Tuple apple_xlate = { 1.0f, 0.0f, 0.1f };
Tuple apple_scale = { 0.4,0.4,0.4 };
Tuple apple_rotation = { 0,0,0 };

Tuple muffin_xlate = { -0.1f, 0.0f, 0.0f };
Tuple muffin_scale = { 0.35,0.35,0.35 };
Tuple muffin_rotation = { 0,0,0 };

Tuple muffin_bot_xlate = { -0.1f, -0.15f, 0.0f };
Tuple muffin_bot_scale = { 0.4,0.2,0.4 };
Tuple muffin_bot_rotation = { 0,0,0 };

Tuple teapot_xlate = { -0.4, -0.25f, -1.0f};
Tuple teapot_scale = { 1,1.5,1 };
Tuple teapot_rotation = { 0,180,0 };

Tuple flower_xlate = { 0.4 ,1,0 };
Tuple flower_scale = { 0.35,0.3,0.35 };
Tuple flower_rotation = { 250, 0,-20 };

Tuple table_xlate = { 0,-1.75,-0.55 };
Tuple table_scale = { 3,3,3 };
Tuple table_rotation = { -90,0,0 };

Tuple vase_mid_xlate = { 0.5f, 0.4f, -0.4f };
Tuple vase_mid_scale = { 0.7,0.7,0.7 };
Tuple vase_mid_rotation = { 0,0,180 };

Tuple vase_top_xlate = { 0.5f, 0.6f, -0.4f };
Tuple vase_top_scale = { 0.5,0.5,0.5 };
Tuple vase_top_rotation = { 180,0,0 };

Tuple vase_base_xlate = { 0.5f, 0.0, -0.4f };
Tuple vase_base_scale = { 0.5,0.6,0.5 };
Tuple vase_base_rotation = { 0,0,0 };

Tuple wood_xlate = { -1.1f, -0.1, 0.0 };
Tuple wood_scale = { 0.2, 0.7 ,0.2 };
Tuple wood_rotation = { 0,0,0 };


Tuple candle_xlate = { -1.1f, 0.5, 0.0 };
Tuple candle_scale = { 0.15, 0.5 ,0.15 };
Tuple candle_rotation = { 0,0,0 };

Tuple cup_xlate = { -0.6f, -0.1, 0.0 };
Tuple cup_scale = { 0.5, 0.25 ,0.5 };
Tuple cup_rotation = { 180,0,0 };

Tuple cup_base_xlate = { -0.6f, -0.1, 0.0 };
Tuple cup_base_scale = { 0.2, 0.3 ,0.2 };
Tuple cup_base_rotation = { 0,0,0 };

Tuple leaf_xlate = { 0.5f, 0.9, -0.4f };
Tuple leaf_scale = { 0.06, 1 ,0.06 };
Tuple leaf_rotation = { -10,0,10 };

//
// We need vertex buffers and element buffers for the
// quad (texture mapped), the teapot (Phong shaded),
// and the sphere (Phong shaded, by default).
//
BufferSet quadBuffers;
BufferSet teapotBuffers;
BufferSet sphereBuffers;
BufferSet coneBuffers;
BufferSet cylinderBuffers;

// Animation flag
bool animating = false;

// do we need to do a display() call?
bool updateDisplay = true;

// Initial animation rotation angles for the objects
GLfloat angles = 0.0f;

// Current state of animation for the sphere
int sphereState = 0;

// program IDs for shader programs
GLuint pshader, tshader;

///
// createShape() - create vertex and element buffers for a shape
//
// @param obj - which shape to create
// @param B - which BufferSet to use
///
void createShape( int obj, BufferSet *B )
{
    // clear any previous shape
    canvas->clear();

    // make the shape
    switch( obj ) {
    case OBJ_QUAD:    makeQuad( *canvas );   break;
    case OBJ_SPHERE:  makeSphere( *canvas ); break;
    case OBJ_TEAPOT:  makeTeapot( *canvas ); break;
	case OBJ_CONE:	  makeCone(*canvas); break;
	case OBJ_CYLINDER: makeCylinder(*canvas); break;
    }

    // create the necessary buffers
    B->createBuffers( *canvas );
}

///
// OpenGL initialization
///
void init( void )
{
    // Create our Canvas
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
        glfwTerminate();
        exit( 1 );
    }

    // Load texture image(s)
    loadTextures();

    // Load shaders, verifying each
    ShaderError error;
    tshader = shaderSetup( "texture.vert", "texture.frag", &error );
    if( !tshader ) {
        cerr << "Error setting up texture shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }

    pshader = shaderSetup( "phong.vert", "phong.frag", &error );
    if( !pshader ) {
        cerr << "Error setting up Phong shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }

    // Other OpenGL initialization
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // Create all our objects
    createShape( OBJ_QUAD, &quadBuffers );
    createShape( OBJ_TEAPOT, &teapotBuffers );
    createShape( OBJ_SPHERE, &sphereBuffers );
	createShape( OBJ_CONE, &coneBuffers);
	createShape(OBJ_CYLINDER, &cylinderBuffers);
}

///
// Display the current image
///
void display( void )
{
    // clear and draw params..
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//draw table
	drawShape(tshader, OBJ_QUAD, quadBuffers, table_scale, table_rotation, table_xlate, eye, lookat, up);

    // draw apple
    drawShape(pshader, MATL_APPLE, sphereBuffers, apple_scale, apple_rotation, apple_xlate, eye, lookat, up);

	// draw muffin
	drawShape(pshader, MATL_MUFFIN, sphereBuffers, muffin_scale,muffin_rotation, muffin_xlate, eye, lookat, up);
	drawShape(pshader, MATL_MUFFINCUP, cylinderBuffers, muffin_bot_scale, muffin_bot_rotation, muffin_bot_xlate, eye, lookat, up);
	muffin_bot_scale = { 0.7,0.1,0.7 };
	muffin_bot_xlate.y -= 0.1;
	drawShape(pshader, MATL_CUP, cylinderBuffers, muffin_bot_scale, muffin_bot_rotation, muffin_bot_xlate, eye, lookat, up);

	//draw flowers
	drawShape(pshader, MATL_FLOWER, coneBuffers, flower_scale, flower_rotation, flower_xlate, eye, lookat, up);
	flower_xlate.x += 0.5;
	flower_xlate.y += 0.2;
	flower_rotation.z += 25;
	drawShape(pshader, MATL_FLOWER, coneBuffers, flower_scale, flower_rotation, flower_xlate, eye, lookat, up);
	flower_xlate.x -= 0.8;
	flower_xlate.z -= 0.2;
	flower_rotation.z -= 65;
	drawShape(pshader, MATL_FLOWER, coneBuffers, flower_scale, flower_rotation, flower_xlate, eye, lookat, up);
	flower_xlate.x += 0.4;
	flower_xlate.y += 0.2;
	flower_xlate.z -= 0.3;
	flower_rotation.z += 10;
	drawShape(pshader, MATL_YELLOWFLOWER, coneBuffers, flower_scale, flower_rotation, flower_xlate, eye, lookat, up);

	//draw cup
	drawShape(pshader, MATL_CUP, coneBuffers, cup_scale, cup_rotation, cup_xlate, eye, lookat, up);
	drawShape(pshader, MATL_CUP, coneBuffers, cup_base_scale, cup_base_rotation, cup_base_xlate, eye, lookat, up);

	//draw wood
	drawShape(pshader, MATL_WOOD, cylinderBuffers, wood_scale, wood_rotation, wood_xlate, eye, lookat, up);
	drawShape(pshader, MATL_CANDLE, cylinderBuffers, candle_scale, candle_rotation, candle_xlate, eye, lookat, up);

	//draw base base
	drawShape(pshader, MATL_VASE, cylinderBuffers, vase_base_scale, vase_base_rotation, vase_base_xlate, eye, lookat, up);
	//draw vase mid
	drawShape(pshader, MATL_VASE, sphereBuffers, vase_mid_scale, vase_mid_rotation, vase_mid_xlate, eye, lookat, up);
	//upper part vase
	drawShape(pshader, MATL_VASE, coneBuffers, vase_top_scale, vase_top_rotation, vase_top_xlate, eye, lookat, up);
    //draw the teapot
	drawShape(pshader, OBJ_TEAPOT, teapotBuffers , teapot_scale, teapot_rotation, teapot_xlate, eye, lookat, up);
	//draw leaf
	drawShape(pshader, MATL_LEAF, cylinderBuffers, leaf_scale, leaf_rotation, leaf_xlate, eye, lookat, up);
	leaf_xlate.x += 0.1;
	leaf_xlate.y -= .2;
	leaf_rotation.x -= 5;
	leaf_rotation.z -= 40;
	drawShape(pshader, MATL_LEAF, cylinderBuffers, leaf_scale, leaf_rotation, leaf_xlate, eye, lookat, up);
}

///
// Event callback routines
///

///
// Handle keyboard input
//
// We don't need to distinguish between (e.g.) 'a' and 'A', so
// we can use the simpler form of callback
///
void keyboard( GLFWwindow *window, int key, int scan, int action, int mods )
{
//    if( action != GLFW_PRESS )    // only activate on press, not rpt/release
//        return;
//
//    switch( key ) {
//    case GLFW_KEY_A:    // animate
//        animating = true;
//        break;
//
//    case GLFW_KEY_S:    // stop animating
//        animating = false;
//        break;
//
//    case GLFW_KEY_R:    // reset transformations
//        angles = 0.0f;
//        xlate[0] = XLATE_X;
//        xlate[1] = XLATE_Y;
//        xlate[2] = XLATE_Z;
//        sphereState = 0;
//        break;
//
//    case GLFW_KEY_ESCAPE:   // terminate the program
//    case GLFW_KEY_Q:
//        glfwSetWindowShouldClose( window, 1 );
//        break;
//    }
//
    //updateDisplay = true;
}

///
// Animation routine
///
void animate( void ) {

    //if( animating ) {

    //    // first, rotation for the objects
    //    angles += 1.0f;
    //    if( angles >= 360.0f ) {
    //        angles = 0.0f;
    //    }

    //    // next, translation for the sphere
    //    switch( sphereState ) {
    //    case 0:  // first side
    //        // horizontal move until tx reaches 0.5
    //        if( xlate[0] > 0.5f ) {
    //            xlate[0] -= 0.025f;
    //            break;
    //        } else {
    //            // fall through into state 1
    //            sphereState = 1;
    //        }

    //    case 1:  // second side
    //        // move downward to the right until ty reaches 1.4
    //        if( xlate[1] > 1.4f ) {
    //            xlate[0] += 0.0125;
    //            xlate[1] -= 0.025f;
    //            break;
    //        } else {
    //            // fall through into state 2
    //            sphereState = 2;
    //        }

    //    case 2:  // third side
    //        // move upward to the right until tx reaches 1.3
    //        if( xlate[0] < 1.3f ) {
    //            xlate[0] += 0.0125;
    //            xlate[1] += 0.025f;
    //            break;
    //        }
    //        // at that point, we fall back to state 0

    //    default:  // unknown state - fall back!
    //        // move back to state 0
    //        sphereState = 0;
    //        xlate[0] = XLATE_X;
    //        xlate[1] = XLATE_Y;

    //    }

    //    updateDisplay = true;
    //}
}

///
// Error callback for GLFW
///
void glfwError( int code, const char *desc )
{
    cerr << "GLFW error " << code << ": " << desc << endl;
    exit( 2 );
}

///
// Main program for texting assignment
///
int main( int argc, char **argv ) {

    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        cerr << "Can't initialize GLFW!" << endl;
        exit( 1 );
    }

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Lab 6 - Shading and Texturing", NULL, NULL );

    if( !window ) {
        cerr << "GLFW window create failed!" << endl;
        glfwTerminate();
        exit( 1 );
    }

    glfwMakeContextCurrent( window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
        exit( 1 );
    }

    if( !GLEW_VERSION_3_0 ) {
        cerr << "GLEW: OpenGL 3.0 not available" << endl;
        if( !GLEW_VERSION_2_1 ) {
            cerr << "GLEW: OpenGL 2.1 not available, either!" << endl;
            glfwTerminate();
            exit( 1 );
        }
    }
#endif

    // determine whether or not we can use GLSL 1.30
    int maj = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MAJOR );
    int min = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MINOR );

    cerr << "GLFW: using " << maj << "." << min << " context" << endl;
    if( maj < 3 ) {
        cerr << "*** GLSL 1.30 shaders may not compile" << endl;
    }

    init();

    glfwSetKeyCallback( window, keyboard );

    while( !glfwWindowShouldClose(window) ) {
        animate();
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( window );
        }
        glfwPollEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}
