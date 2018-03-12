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
#ifndef __LIGHT_H__
#define __LIGHT_H__
#include <string>
#include "object.h"


/**
 *
 */
class Light : public Object {
public:


    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Light (void);

    /**
     * Destructor
     */
    virtual ~Light (void);
    //@}

    
    /**
     * @name Rendering
     */
    //@}


    /** 
     * @name Diffuse color
     */
    /**
     * Set diffuse color
     *
     * @param color color as a tuple of 4 floats.
     */
    virtual void set_diffuse_color (Color color);

    /**
     * Set diffuse color
     *
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     */
    virtual void set_diffuse_color (float r=0, float g=0, float b=0, float a=1);

    /**
     * Get diffuse color
     *
     * @return diffuse color
     */
    virtual Color get_diffuse_color (void) const;
    //@}

    /** 
     * @name Ambient color
     */
    /**
     * Set ambient color
     *
     * @param color color as a tuple of 4 floats.
     */
    virtual void set_ambient_color (Color color);

    /**
     * Set ambient color
     *
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     */
    virtual void set_ambient_color (float r=0, float g=0, float b=0, float a=1);

    /**
     * Get ambient color
     *
     * @return ambient color
     */
    virtual Color get_ambient_color (void) const;
    //@}


    /** 
     * @name Specular color
     */
    /**
     * Set specular color
     *
     * @param color color as a tuple of 4 floats.
     */
    virtual void set_specular_color (Color color);

    /**
     * Set specular color
     *
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     */
    virtual void set_specular_color (float r=0, float g=0, float b=0, float a=1);

    /**
     * Get specular color
     *
     * @return specular color
     */
    virtual Color get_specular_color (void) const;
    //@}



    /**
     * @name Position
     */
    /**
     * Set position of light source
     *
     *
     * @param position position
     */
    virtual void set_position (Position position);

    /**
     * Set position
     *
     * Position is set relatively to gravity_ and it is possible to use
     * relative position using coordinate between 0 and 1.
     *
     * @param x position along x axis
     * @param y position along y axis
     * @param z position along z axis
     */
    virtual void set_position (float x=0, float y=0, float z=0);
    //@}

    /**
     * @name Direction
     */
    /**
     * Set direction of light
     *
     *
     * @param direction direction
     */
    virtual void set_direction (Direction direction);

    /**
     * Set direction of light
     *
     *
     * @param x direction along x axis
     * @param y direction along y axis
     * @param z direction along z axis
     */
    virtual void set_direction (float x=0, float y=0, float z=0);

    /**
     * Get direction
     *
     * @return direction
     */
    virtual Direction get_direction (void) const;
    
    //@}

};

#endif
