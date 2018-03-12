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

 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __VEC4F_H__
#define __VEC4F_H__

#include <cmath>

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Vec4f> Vec4fPtr;
#else
    typedef class Vec4f *                  Vec4fPtr;
#endif


/**
 * Vector of 4 floats.
 *
 * The Vec4f structure is basically made of four consecutive floats than can be
 * adressed indifferently as:
 *  - an array of four floats
 *  - (x, y, z, w) 
 *  - (dx, dy, dz, dw)
 *  - (r, g, b, a)
 *  - (red,green,blue,alpha)
 *  - (min, max, major, minor)
 *  - (width, height, depth)
 *  - (up, down, left, right)
 *
 * Note that Vec4f is using several different names depending on the context
 *
 * \verbatim
 typedef Vec4f Position;
 typedef Vec4f Size;
 typedef Vec4f Color;
 typedef Vec4f Orientation;
 typedef Vec4f Margin;
 typedef Vec4f Gravity;
 typedef Vec4f Range;
 typedef Vec4f Shape;
\endverbatim
 */

class Vec4f {
public:
    union {
        float data[4];    /*!< All four components. */

        struct {

            union {
                float x;     /*!< First component. */  
                float dx;    /*!< First component. */  
                float min;   /*!< First component. */  
                float width; /*!< First component. */  
                float r;     /*!< First component. */  
                float red;   /*!< First component. */  
                float up;    /*!< First component. */  
                float top;   /*!< First component. */  
				float length;/*!< First component. */
            };

            union {
                float y;      /*!< Second component. */
                float dy;     /*!< Second component. */
                float max;    /*!< Second component. */
                float height; /*!< Second component. */
                float g;      /*!< Second component. */
                float green;  /*!< Second component. */
                float down;   /*!< Second component. */
                float bottom; /*!< Second component. */
				float theta;  /*!< Second component. */
                float size;   /*!< Second component. */
            };

            union {
                float z;     /*!< Third component. */  
                float dz;    /*!< Third component. */  
                float major; /*!< Third component. */  
                float depth; /*!< Third component. */  
                float b;     /*!< Third component. */  
                float blue;  /*!< Third component. */  
                float left;  /*!< Third component. */
				float phi;   /*!< Third component. */
				float thickness;/*!< Third component. */
            };

            union {
                float w;     /*!< Fourth component. */
                float dw;    /*!< Fourth component. */
                float minor; /*!< Fourth component. */
                float a;     /*!< Fourth component. */
                float alpha; /*!< Fourth component. */
                float right; /*!< Fourth component. */
            };
        };
    };

    /**
     * Default constructor.
     *
     * @param x_ x component
     * @param y_ y component
     * @param z_ z component
     * @param w_ w component
     */
    Vec4f (float x_=0, float y_=0, float z_=0, float w_=1)
    {
        x = x_; y = y_; z = z_; w = w_;
    }

    /**
     * Constructor.
     *
     * @param c  4 float components
     */
    Vec4f (float c[4])
    {
        for (int i=0; i<4; i++)
            data[i] = c[i];
    }

    /**
     * Copy constructor.
     *
     * @param other vector to copy
     */
    Vec4f (const Vec4f &other)
    {
        for (int i=0; i<4; i++)
            data[i] = other.data[i];
    }

    /**
     * Destructor.
     */
    ~Vec4f (void)
    { }

    /**
     * Affectation operator.
     *
     * @param other vector to copy
     * @return the current copied vector
     */
    Vec4f &operator= (const Vec4f &other)
    {
        if (this == &other)
            return *this;
        for (int i=0; i<4; i++)
            data[i] = other.data[i];        
        return *this;
    }

    /**
     * Normalize vector along x,y,z (not w).
     */
    void normalize (void) {
        float n = x*x+y*y+z*z;
        if (n) { x = x/sqrt(n); y = y/sqrt(n); z = z/sqrt(n); w = 0; }
    }

    /**
     * Addition
     *
     * @param other vector to add
     * @return addition of to the two vectors
     */
    Vec4f operator+ (const Vec4f &other) {
        return Vec4f (x+other.x, y+other.y, z+other.z, w+other.w);
    }

    /**
     * Substraction
     *
     * @param other vector to substract
     * @return substraction of to the two vectors
     */
    Vec4f operator- (const Vec4f &other) {
        return Vec4f (x-other.x, y-other.y, z-other.z, w-other.w);
    }

    /**
     * Scaling
     *
     * @param s scaling factor
     * @return scaled vector
     */
    Vec4f operator* (const float s) {
        return Vec4f (x*s, y*s, z*s, w*s);
    }

    /**
     * Cross product along x,y,z (not w).
     *
     * @param other vector to cross
     * @return cross product of this and other
     */
    Vec4f cross (const Vec4f &other) {
        return Vec4f (y*other.z-z*other.y,
                      z*other.x-x*other.z,
                      x*other.y-y*other.x,
                      0);
    }
};

typedef Vec4f Position;
typedef Vec4f Direction;
typedef Vec4f Size;
typedef Vec4f Color;
typedef Vec4f Orientation;
typedef Vec4f Margin;
typedef Vec4f Border;
typedef Vec4f Gravity;
typedef Vec4f Range;
typedef Vec4f Shape;
typedef Vec4f Point;
typedef Vec4f Tick;
typedef Vec4f Segment;
#endif
