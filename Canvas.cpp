//
//  Canvas.cpp
//
//  Routines for adding points to create a new mesh.
//
//  Created by Warren R. Carithers 2016/09/30.
//  Based on a C++ version created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  This file should not be modified by students.
//

#include <cstdlib>
#include <iostream>
#include <iomanip>

// Canvas.h includes all the OpenGL/GLFW/etc. header files for us
#include "Canvas.h"

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Canvas::Canvas( int w, int h ) : width(w), height(h) {
    currentColor.r = 0.0f;
    currentColor.g = 0.0f;
    currentColor.b = 0.0f;
    currentColor.a = 1.0f;
    currentDepth = -1.0f;
    pointArray = 0;
    normalArray = 0;
    uvArray = 0;
    colorArray = 0;
    elemArray = 0;
    numElements = 0;
}

///
// Destructor
///
Canvas::~Canvas( void ) {
    clear();
}

///
// Clear the canvas
///
void Canvas::clear( void )
{
    if( pointArray ) {
        delete [] pointArray;
        pointArray = 0;
    }
    if( normalArray ) {
        delete [] normalArray;
        normalArray = 0;
    }
    if( uvArray ) {
        delete [] uvArray;
        uvArray = 0;
    }
    if( elemArray ) {
        delete [] elemArray;
        elemArray = 0;
    }
    if( colorArray ) {
        delete [] colorArray;
        colorArray = 0;
    }
    points.clear();
    normals.clear();
    uv.clear();
    colors.clear();
    numElements = 0;
    currentColor.r = 0.0f;
    currentColor.g = 0.0f;
    currentColor.b = 0.0f;
    currentColor.a = 1.0f;
}

///
// Add a triangle to the current shape
//
// @param p0 first triangle vertex
// @param p1 second triangle vertex
// @param p2 final triangle vertex
///
void Canvas::addTriangle( Vertex p0, Vertex p1, Vertex p2 )
{
    //points.push_back( p0.x );
    //points.push_back( p0.y );
    //points.push_back( p0.z );
    //points.push_back( 1.0f );

    //points.push_back( p1.x );
    //points.push_back( p1.y );
    //points.push_back( p1.z );
    //points.push_back( 1.0f );

    //points.push_back( p2.x );
    //points.push_back( p2.y );
    //points.push_back( p2.z );
    //points.push_back( 1.0f );



    //numElements += 3;  // three vertices per triangle

	float ux = p1.x - p0.x;
	float uy = p1.y - p0.y;
	float uz = p1.z - p0.z;

	float vx = p2.x - p0.x;
	float vy = p2.y - p0.y;
	float vz = p2.z - p0.z;

	Normal nn = { (uy * vz) - (uz * vy),
				  (uz * vx) - (ux * vz),
				  (ux * vy) - (uy * vx) };

	// Attach the normal to all 3 vertices
	addTriangleWithNorms(p0, nn, p1, nn, p2, nn);

}

///
// adds a triangle to the current shape, along with (u,v) data
//
// @param p0 first triangle vertex
// @param uv0 first triangle (u,v) data
// @param p1 second triangle vertex
// @param uv1 second triangle (u,v) data
// @param p2 final triangle vertex
// @param uv2 final triangle (u,v) data
///
void Canvas::addTriangleWithUV( Vertex p0, TexCoord uv0,
        Vertex p1, TexCoord uv1, Vertex p2, TexCoord uv2 )
{
    // calculate the normal
    float ux = p1.x - p0.x;
    float uy = p1.y - p0.y;
    float uz = p1.z - p0.z;

    float vx = p2.x - p0.x;
    float vy = p2.y - p0.y;
    float vz = p2.z - p0.z;

    Normal nn = { (uy * vz) - (uz * vy),
                  (uz * vx) - (ux * vz),
                  (ux * vy) - (uy * vx) };

    // Attach the normal to all 3 vertices
    addTriangleWithNorms( p0, nn, p1, nn, p2, nn );

    // Attach the texture coordinates
    uv.push_back( uv0.u );
    uv.push_back( uv0.v );
    uv.push_back( uv1.u );
    uv.push_back( uv1.v );
    uv.push_back( uv2.u );
    uv.push_back( uv2.v );
}

///
// adds a triangle to the current shape, along with normal data
//
// @param p0 first triangle vertex
// @param n0 first triangle normal data
// @param p1 second triangle vertex
// @param n1 second triangle normal data
// @param p2 final triangle vertex
// @param n2 final triangle normal data
///
void Canvas::addTriangleWithNorms( Vertex p0, Normal n0,
             Vertex p1, Normal n1, Vertex p2, Normal n2 )
{
    points.push_back( p0.x );
    points.push_back( p0.y );
    points.push_back( p0.z );
    points.push_back( 1.0f );

    normals.push_back( n0.x );
    normals.push_back( n0.y );
    normals.push_back( n0.z );

    points.push_back( p1.x );
    points.push_back( p1.y );
    points.push_back( p1.z );
    points.push_back( 1.0f );

    normals.push_back( n1.x );
    normals.push_back( n1.y );
    normals.push_back( n1.z );

    points.push_back( p2.x );
    points.push_back( p2.y );
    points.push_back( p2.z );
    points.push_back( 1.0f );

    normals.push_back( n2.x );
    normals.push_back( n2.y );
    normals.push_back( n2.z );

    numElements += 3;  // three vertices per triangle
}

///
// Set the pixel Z coordinate
//
// @param d The depth to use
//
// @return  The old depth value
///
float Canvas::setDepth( float d )
{
    float r = currentDepth;

    currentDepth = d;
    return( r );
}

///
// Set the current drawing color
//
// @param color The desired color
///
void Canvas::setColor( Color color )
{
    currentColor = color;
}

///
// Set a pixel to the current drawing color
//
// @param x The x coord of the pixel to be set
// @param y The y coord of the pixel to be set
///
void Canvas::setPixel( float x, float y )
{
    points.push_back( x );
    points.push_back( y );
    points.push_back( currentDepth );
    points.push_back( 1.0f );

    colors.push_back( currentColor.r );
    colors.push_back( currentColor.g );
    colors.push_back( currentColor.b );
    colors.push_back( 1.0f );  // ignore the alpha channel

    numElements += 1;
}

///
// Set a pixel to the specified drawing color
//
// @param x The x coord of the pixel to be set
// @param y The y coord of the pixel to be set
// @param color The desired color
///
void Canvas::setPixelColor( float x, float y, Color color )
{
    points.push_back( x );
    points.push_back( y );
    points.push_back( currentDepth );
    points.push_back( 1.0f );

    colors.push_back( color.r );
    colors.push_back( color.g );
    colors.push_back( color.b );
    colors.push_back( 1.0f );  // ignore the alpha channel

    numElements += 1;
}

///
// Retrieve the array of element data from this Canvas
///
GLuint *Canvas::getElements( void )
{
    // delete the old element array if we have one
    if( elemArray ) {
        delete [] elemArray;
        elemArray = 0;
    }

    int n = points.size();

    if( n > 0 ) {
        // create and fill a new element array
        elemArray = new GLuint[ n ];
        if( elemArray == 0 ) {
            cerr << "element allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            elemArray[i] = i;
        }
    }

    return elemArray;
}

///
// Retrieve the array of vertex data from this Canvas
///
float *Canvas::getVertices( void )
{
    // delete the old point array if we have one
    if( pointArray ) {
        delete [] pointArray;
        pointArray = 0;
    }

    int n = points.size();

    if( n > 0 ) {
        // create and fill a new point array
        pointArray = new float[ n ];
        if( pointArray == 0 ) {
            cerr << "point allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            pointArray[i] = points[i];
        }
    }

    return pointArray;
}

///
// Retrieve the array of normal data from this Canvas
///
float *Canvas::getNormals( void )
{
    // delete the old normal array if we have one
    if( normalArray ) {
        delete [] normalArray;
        normalArray = 0;
    }

    int n = normals.size();

    if( n > 0 ) {
        // create and fill a new normal array
        normalArray = new float[ n ];
        if( normalArray == 0 ) {
            cerr << "normal allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            normalArray[i] = normals[i];
        }
    }

    return normalArray;
}

///
// Retrieve the array of texture coordinate data from this Canvas
///
float *Canvas::getUV( void )
{
    // delete the old texture coordinate array if we have one
    if( uvArray ) {
        delete [] uvArray;
        uvArray = 0;
    }

    int n = uv.size();

    if( n > 0 ) {
        // create and fill a new texture coordinate array
        uvArray = new float[ n ];
        if( uvArray == 0 ) {
            cerr << "uv allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            uvArray[i] = uv[i];
        }
    }

    return uvArray;
}

///
// Retrieve the array of color data from this Canvas
///
float *Canvas::getColors( void )
{
    // delete the old color array if we have one
    if( colorArray ) {
        delete [] colorArray;
        colorArray = 0;
    }

    int n = colors.size();

    if( n > 0 ) {
        // create and fill a new color array
        colorArray = new float[ n ];
        if( colorArray == 0 ) {
            cerr << "color allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            colorArray[i] = colors[i];
        }
    }

    return colorArray;
}


///
// Retrieve the vertex count from this Canvas
///
int Canvas::numVertices( void )
{
    return numElements;
}
