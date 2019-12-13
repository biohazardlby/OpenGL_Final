///
//  Sphere.h
//
//  Prototypes for shape-drawing functions.
//
//  Created by Warren R. Carithers 2018/11/28.
//  Based on earlier versions by Warren R. Carithers and Joe Geigel.
//  Last updated 2019/04/13 by wrc.
//  Copyright 2018 Rochester Institute of Technology.  All rights reserved.
//
//  This file should not be modified by students.
///

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Shapes.h"
#include "Canvas.h"
#include "Buffers.h"
#include "Tuple.h"

#define OBJ_CONE	3
#define OBJ_CYLINDER 4
#define OBJ_CUBE	5

#define MATL_APPLE 6
#define MATL_MUFFIN 7
#define MATL_FLOWER 8
#define MATL_VASE 9
#define MATL_CANDLE 10
#define MATL_WOOD 11
#define MATL_CUP 12
#define MATL_YELLOWFLOWER 13
#define MATL_MUFFINCUP 14
#define MATL_LEAF 15

///
// makeSphere
//
// Invoked whenever a sphere must be created
//
// @param C - Canvas being used
///
void makeSphere( Canvas &C );
void makeCone(Canvas &C);
void makeCylinder(Canvas &C);
///
// drawSphere
//
// Invoked whenever the sphere must be redrawn
//
// @param pshader - shader program for Phong shading
// @param tshader - shader program for texture mapping
// @param bset    - the BufferSet containing the object's data
///
void drawShape(GLuint pshader,int obj, BufferSet &bset, Tuple scale,Tuple rotation, Tuple xlate, Tuple eye, Tuple lookat, Tuple up);
#endif
