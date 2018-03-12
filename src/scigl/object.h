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
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __OBJECT_H__
#define __OBJECT_H__

#if defined(__APPLE__)
#   include <GL/glew.h>
#   include <OpenGL/gl.h>
#else
#   include <GL/glew.h>
#   include <GL/gl.h>
#endif

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Object> ObjectPtr;
#else
    typedef class Object *                  ObjectPtr;
#endif
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstdlib>
#include "vec4f.h"


/**
 * Base class for all renderable objects.
 *
 * An object is something  that can be rendered on screen, either  in 3d mode or
 * in 2d  mode (widget) and is  defined by its position,  its size and  a set of
 * three colors  which are respectively  the foreground, the background  and the
 * border color. The exact meaning of  these attributes depends on the nature of
 * the object. For example the  position attribute of the widget class describes
 * the position of  the widget into screen 2d coordinates  while for 3d objects,
 * this same attribute describes as the  position into 3d space.  Each object is
 * susceptible to  receive some user  interaction through keyboard or  mouse and
 * the  behavior  of   the  object  is  object  dependent   (see  Scene  for  an
 * example). Each object may be shown or hidden using a fadeout effect as long a
 * the rendering of the object takes the alpha attribute into account.
 *
 */
class Object {

public:

    // _________________________________________________________________________

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Object (void);

    /**
     * Destructor
     */
    virtual ~Object (void);

    /**
     *
     */
    virtual std::string get_repr( void );
    //@}

    // _________________________________________________________________________

    /**
     *  @name Rendering pipeline
     */
    /**
     * Setup the object
     */
    virtual void setup (void);

    /**
     * Render the object
     */
    virtual void render (void);

    /**
     * Update the object
     */
    virtual void update (void);

    //@}


    // _________________________________________________________________________

    /**
     * @name Event processing
     */
    /**
     * Select callback
     *
     * @param x      x pointer coordinates (window space)
     * @param y      y pointer coordinates (window space)
     */
    virtual bool select (int x, int y);

    /**
     * Keyboard action
     *
     * @param action   action to be performed
     * @param keyname  key name
     */
    virtual bool keyboard_action (std::string action, std::string keyname);

    /**
     * Mouse action start
     *
     * If button is 1 (left button) and (x,y) is within the widget, a move
     * action will be initiated. If (x,y) is within the widget but around the
     * border, a resize action will be initiated.
     *
     * @param action action to be started
     * @param x      x pointer coordinates (window space)
     * @param y      y pointer coordinates (window space)
     */
    virtual bool mouse_action_start (std::string action, int x, int y);

    /**
     * Mouse action end
     *
     * Stop the current action.
     *
     * @param action action to be ended
     * @param x      x pointer coordinates (window space)
     * @param y      y pointer coordinates (window space)
     */
    virtual bool mouse_action_end (int x, int y);

    /**
     * Mouse action
     *
     * @param x      x pointer coordinates (window space)
     * @param y      y pointer coordinates (window space)
     */
    virtual bool mouse_action (int x, int y);
    //@}


    // _________________________________________________________________________

    /**
     * @name Name
     */
    /**
     * Set name.
     *
     * @param name Name
     */
    virtual void set_name (std::string name);

    /**
     * Get name.
     *
     * @return name
     */
    virtual std::string get_name (void) const;
    //@}


    // _________________________________________________________________________

    /**
     * @name Identification
     */
    /**
     * Get id
     *
     * @return id
     */
    virtual unsigned int get_id (void) const;

    /**
     * Check id
     *
     * @param id id to be checked
     */
    virtual bool check_id (unsigned int id);

    //@}


    // _________________________________________________________________________

    /**
     * @name Visibility
     */
    /**
     * Set visibility status
     *
     * @param visible Visibility
     */
    virtual void set_visible (bool visible);

    /**
     * Get visibility status
     *
     * @return visibility
     */
    virtual bool get_visible (void) const;

    /**
     * Progressive disappearance of the object.
     *
     * To enforce disappearance, object has to use the alpha_ attribute and
     * keep it up to date with the <code>compute_visibility</code> method.
     *
     * @param timeout time (ms) for fade out object
     */
    virtual void hide (float timeout=0);

    /**
     * Progressive appearance of the object.
     *
     * To enforce appearance, object has to use the alpha_ attribute and 
     * keep it up to date with the <code>compute_visiblity</code> method.
     *
     * @param timeout time (ms) for fade out object
     */
    virtual void show (float timeout=0);

    /**
     * Set global alpha transparency
     *
     * @param alpha alpha channel level
     */
    virtual void set_alpha (float alpha);

    /**
     * Get global alpha transparency
     *
     * @return global alpha transparency
     */
    virtual float get_alpha (void) const;
    //@}


    // _________________________________________________________________________

    /**
     * @name Position
     */
    /**
     * Set position
     *
     * @param position Position a tuple of 3 floats.
     */
    virtual void set_position (Position position);

    /**
     * Set position
     *
     * @param x position along x axis
     * @param y position along y axis
     * @param z position along z axis
     * @param w not used
     */
    virtual void set_position (float x=0,
                               float y=0,
                               float z=0,
                               float w=0);

    /**
     * Get position 
     *
     * @return position
     */
    virtual Position get_position (void) const;
    //@}


    // _________________________________________________________________________

    /**
     * @name Size
     */
    /**
     * Set size
     *
     * @param size Size as a tuple of 3 floats.
     */
    virtual void set_size (Size size);

    /**
     * Set size
     *
     * @param x size along x axis
     * @param y size along y axis
     * @param z size along z axis
     * @param w not used
     */
    virtual void set_size (float x=1,
                           float y=1,
                           float z=1,
                           float w=1);

    /**
     * Get size
     *
     */
    virtual Size get_size (void) const;
    //@}


    // _________________________________________________________________________

    /** 
     * @name Foreground color
     */
    /**
     * Set foreground color
     *
     * @param color color as a tuple of 4 floats.
     */
    virtual void set_fg_color (Color color);

    /**
     * Set foreground color
     *
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     */
    virtual void set_fg_color (float r=0,
                               float g=0,
                               float b=0,
                               float a=1);

    /**
     * Get foreground color
     *
     * @return foreground color
     */
    virtual Color get_fg_color (void) const;
    //@}


    // _________________________________________________________________________

    /**
     * @name Background color
     */
    /**
     * Set background color
     *
     * @param color color as a tuple of 4 floats.
     */
    virtual void set_bg_color (Color color);

    /**
     * Set background color
     *
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     */
    virtual void set_bg_color (float r=0,
                               float g=0,
                               float b=0,
                               float a=1);

    /**
     * Get background color
     *
     * @return background color
     */
    virtual Color get_bg_color (void) const;
    //@}


    // _________________________________________________________________________

    /**
     * @name Border color
     */
    /**
     * Set border color
     *
     * @param color Color as a tuple of 4 floats.
     */
    virtual void set_br_color (Color color);

    /**
     * Set border color
     *
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     */
    virtual void set_br_color (float r=0,
                               float g=0,
                               float b=0,
                               float a=1);

    /**
     * Get border color
     *
     * @return border color
     */
    virtual Color get_br_color (void) const;
    //@}



protected:

    // _________________________________________________________________________

    /**
     * Update visibility status
     *
     * Depending on #fade_in_delay_ or #fade_out_delay_ and #fade_time_,
     * #alpha_ and #visible_ are updated accordingly.
     */
    virtual void compute_visibility (void);


protected:

    // _________________________________________________________________________

    /**
     * Name
     */
    std::string name_;
    
    /**
     * Unique identifier
     */
    unsigned long id_;

    /**
     * Unique identifier counter
     */
    static unsigned long id_counter_;

    /**
     * Whether object is visible or not
     */
    bool visible_;

    /**
     * Position
     */
    Position position_;

    /**
     * Size
     */
    Size size_;

    /**
     * Foreground color
     */
    Color fg_color_;

    /**
     * Background color
     */
    Color bg_color_;

    /**
     * Border color
     */
    Color br_color_;

    /**
     * Global transparency level
     */
    float alpha_;

    /**
     * Last fade in/out start time
     */
    struct timeval fade_time_;

    /**
     * Fade in time delay
     */
    float fade_in_delay_;

    /**
     * Fade out time delay
     */
    float fade_out_delay_;


    /**
     *
     */
    std::string action_;
};

#endif
