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
#ifndef __WIDGET_H__
#define __WIDGET_H__
#include "object.h"

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class Widget> WidgetPtr;
#else
    typedef class Widget * WidgetPtr;
#endif


/**
 * Base class for all 2d renderable objects.
 *
 * A widget is a special kind of object that is rendered in orthographic mode
 * using a one to one pixel correspondance between window coordinates and the
 * (x,y) plane. A widget implements the #button_press, #button_release and
 * #mouse_motion callbacks for moving or resizing the widget through user
 * interaction.
 *
 * \image html widget-gravity.png
 *
 */
class Widget : public Object {
public:


    // _________________________________________________________________________

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Widget (void);

    /**
     * Destructor
     */
    virtual ~Widget (void);
    //@}

    
    // _________________________________________________________________________

    /**
     * @name Rendering
     */
    /**
     * Render
     */
    virtual void render (void);

    /**
     * Initiate "2d" (ortho) projection mode
     */
    virtual void render_start (void);

    /**
     * Terminate "2d" (ortho) projection mode
     */
    virtual void render_finish (void);
    //@}


    // _________________________________________________________________________

    /**
     * @name Event processing
     */
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
     * Mouse action process
     *
     * @param x      x pointer coordinates (window space)
     * @param y      y pointer coordinates (window space)
     */
    virtual bool mouse_action (int x, int y);
    //@}


    // _________________________________________________________________________

    /**
     * @name Position
     */
    /**
     * Set position in window space.
     *
     * Position is set relatively to gravity_ and it is possible to use
     * relative position using coordinate between 0 and 1.
     *
     * @param position position in window space
     */
    virtual void set_position (Position position);

    /**
     * Set position in window space.
     *
     * Position is set relatively to gravity_ and it is possible to use
     * relative position using coordinate between 0 and 1.
     *
     * @param x position along x axis
     * @param y position along y axis
     * @param z -1 for back, +1 for front
     */
    virtual void set_position (float x=0, float y=0, float z=1);
    //@}


    // _________________________________________________________________________

    /**
     * @name Size
     */
    /**
     * Set size in window space.
     *
     * It is possible to use relative size using coordinates between 0 and 1.
     * Negative sizes relates to the complement of the related dimension. For
     * example, if x is -10, then x size will be window_width-10. If x is -.25,
     * then x size will be (1-.25)*window_width.
     *
     * @param size size
     */
    virtual void set_size (Size size);

    /**
     * Set size in window space.
     *
     * It is possible to use relative size using coordinates between 0 and 1.
     * Negative sizes relates to the complement of the related dimension. For
     * example, if x is -10, then x size will be window_width-10. If x is -.25,
     * then x size will be (1-.25)*window_width. 
     *
     * @param x position along x axis
     * @param y position along y axis
     * @param z -1 for back, +1 for front
     */
    virtual void set_size (float x=1, float y=1, float z=1);
    //@}


    // _________________________________________________________________________

    /**
     * @name Gravity
     */
    /**
     * Set gravity.
     *
     * Note: gravity is a couple (x,y) where x,y are in {-1,0,1}
     *
     * @param gravity gravity
     */
    virtual void set_gravity (Gravity gravity);

    /**
     * Set gravity.
     *
     * Note: gravity is a couple (x,y) where x,y are in {-1,0,1}
     *
     * @param x x gravity
     * @param y y gravity
     */
    virtual void set_gravity (float x=1, float y=1);

    /**
     * Get gravity.
     *
     * @return gravity
     */    
    virtual Gravity get_gravity (void);
    //@}


    // _________________________________________________________________________

    /**
     * @name Radius
     */
    /**
     * Set border radius.
     *
     * @param radius border radius
     */    
    virtual void set_radius (int radius);

    /**
     * Get radius.
     *
     * @return radius
     */    
    virtual int get_radius (void);
    //@}


    // _________________________________________________________________________

    /**
     * @name Margin
     */
    /**
     * Set inner margin.
     *
     * @param margin margin as a (up,down,left,right) tuple
     */    
    virtual void set_margin (Margin margin);

    /**
     * Set inner margin.
     *
     * @param u up margin
     * @param d down margin
     * @param l left margin
     * @param r right margin
     */    
    virtual void set_margin (float u=0, float d=0, float l=0, float r=0);

    /**
     * Get margin.
     *
     * @return margin.
     */    
    virtual Margin get_margin (void);
    //@}


    // _________________________________________________________________________

    /**
     * @name Focusable property
     */
    /**
     * Set focusable property
     *
     * @param focusable whether widget is focusable
     */
    virtual void set_focusable (bool focusable);

    /**
     * Get focusable.
     *
     * @return focusable property.
     */    
    virtual bool get_focusable (void);

    /**
     * Has widget focus ? 
     *
     * @return Whether widget has focus or not
     */    
    virtual bool has_focus (int x, int y);
    //@}


    // _________________________________________________________________________

    /**
     * @name Sizeable property
     */
    /**
     * Set sizeable property
     *
     * @param sizeable whether widget is sizeable
     */
    virtual void set_sizeable (bool sizeable);

    /**
     * Get sizeable.
     *
     * @return sizeable property.
     */    
    virtual bool get_sizeable (void);
    //@}


    // _________________________________________________________________________

    /**
     * @name Moveable property
     */
    /**
     * Set moveable property
     *
     * @param moveable whether widget is moveable
     */
    virtual void set_moveable (bool moveable);

    /**
     * Get moveable property.
     *
     * @return moveable property.
     */    
    virtual bool get_moveable (void);
    //@}


protected:

    // _________________________________________________________________________

    /**
     * Compute size_ according to size_request_.
     */
    virtual void compute_size (void);

    /**
     * Compute position_ according to position_request_ and gravity_.
     */
    virtual void compute_position (void);

    /**
     * Render an arc circle in the (x,y) plane between theta1 and theta2.
     *
     * @param x x center coordinate
     * @param y y center coordinate
     * @param radius arc circle radius
     * @param theta1 start angle (degrees)
     * @param theta2 end angle (degrees)
     */
    virtual void arc_circle (float x, float y, float radius,
                             float theta1, float theta2);

    /**
     * Render a rounded rectangle in the (x,y) plane.
     *
     * @param x x position
     * @param y y position
     * @param w width
     * @param h height
     * @param radius corner radius
     */
    virtual void round_rectangle (float x, float y, float w, float h,
                                  float radius);


protected:

    // _________________________________________________________________________

    /**
     * Gravity governs position_request_ to position_ behavior.
     */
    Gravity gravity_;

    /**
     * Position request (translated in position_).
     */
    Position position_request_;

    /**
     * Size request (translated in size_)
     */
    Size size_request_;

    /**
     * Whether widget is focusable (through user interactions)
     */
    bool focusable_;

    /**
     * Whether widget is resizeable (through user interactions)
     */
    bool sizeable_;

    /**
     * Whether widget is moveable (through user interactions)
     */
    bool moveable_;

    /**
     * Inner margins (up,down,left,right)
     */
    Margin margin_; 

    /**
     * Border radius
     */
    int radius_;

    /**
     * Current user action
     */
    //WidgetAction action_; 

    /**
     * Pointer position at event start
     */
    Position action_start_;

    /**
     * Widget position at event start
     */
    Position saved_position_;

    /**
     * Widget size at event start
     */
    Size saved_size_;

    /**
     * Minimum widget size
     */
    static Size min_size_;

    /**
     * Maximum widget size
     */
    static Size max_size_;
};

#endif
