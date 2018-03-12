/*
 * Copyright (C) 2008 Nicolas P. Rougier
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __SHAPES_H__
#define __SHAPES_H__
#include "object.h"
#define SPH_RAD 0.01
#define TRI_H 0.01
#define QUAD_L 0.01

/**
 * Sphere at (x,y,z)
 *
 * @param x center x coordinate
 * @param y center y coordinate
 * @param z center z coordinate
 * @param r sphere radius
 */
void sphere (GLfloat x, GLfloat y, GLfloat z, GLfloat r);

/**
 * Cylinder from (x1,y1,z1) to (x2,y2,z2)
 *
 * Based on the following implementation by Joel J. Parris:
 * http://home.neo.rr.com/jparris/OpenGL - draw cylinder between 2 pts.htm
 *
 * @param x1 base x coordinate
 * @param y1 base y coordinate
 * @param z1 base z coordinate
 * @param r1 base radius
 * @param x2 top x coordinate
 * @param y2 top y coordinate
 * @param z2 top z coordinate
 * @param r2 top radius
 */
void cylinder(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat r1,
              GLfloat x2, GLfloat y2, GLfloat z2, GLfloat r2);


/**
 * Unit-sized xy plane centered on 0 with optional grid
 *
 * @param fg
 * @param bg
 * @param nx
 * @param ny
 */
void plane (Color fg, Color bg, int nx=0, int ny=0);
            
void triangle(GLfloat x1, GLfloat y1,
        GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3);

void quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3,
		GLfloat y3, GLfloat x4, GLfloat y4) ;

void polygon(GLfloat x, GLfloat y, int class_) ;
void switch_polygon(GLfloat x, GLfloat y, int polygon) ;

#endif
