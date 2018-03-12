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
#ifndef __SCENE_H__
#define __SCENE_H__
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "object.h"
#include "widget.h"
#include "GL/glew.h"


#ifdef HAVE_BOOST
  typedef boost::shared_ptr<class Scene> ScenePtr;
#else
    typedef class Scene * ScenePtr;
#endif


/**
 * Container for objects and widgets.
 *
 * A scene describes a set of objects and widgets and manage the view (zoom and
 * orientation). Rendering of elements is done in the following order:
 *
 * -# Widgets with a negative z
 * -# Objects
 * -# Widgets with a positive z
 *
 * \image html scene.png
 * \image latex scene.png
 */
class Scene : public Widget {
public:




    //__________________________________________________________________________
    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor.
     */
    Scene (void);

    /**
     * Destructor
     */
    virtual ~Scene (void);
    //@}


    //__________________________________________________________________________
    /**
     * @name Rendering
     */
    /**
     * Setup scene.
     *
     * This method recursively call objects and widgets build method.
     */
    virtual void setup (void);

    /**
     * Update scene.
     *
     * This method recursively call objects and widgets update method.
     */
    virtual void update (void);

    /**
     * Render the scene.
     *
     * Rendering is done in the following order:
     * -# Widgets with a negative z
     * -# Objects
     * -# Widgets with a positive z
     */
    virtual void render (void);

    /**
     * Save the scene as a ppm image into a file
     *
     * @param filename filename where to save image
     */
    virtual void save (std::string filename);
    //@}


    //__________________________________________________________________________
    /**
     * @name Scene management
     */
    /**
     * Add a new object to the scene.
     *
     * @param object object to be added.
     */
    virtual void add (ObjectPtr object);

    /**
     * Add a new widget to the scene.
     *
     * @param widget widget to be added.
     */
    virtual void add (WidgetPtr widget);

    /**
     *
     */
    virtual ObjectPtr get_object( const std::string name ) const;

    /**
     *
     */
    virtual ObjectPtr operator[]( const std::string name ) const ;

    /**
     *
     */
    virtual void      set_object( const std::string name, const ObjectPtr object );

    /**
     *
     */
    virtual bool      has_object( const std::string name );

    /**
     *
     */
    virtual void      del_object( const std::string name );

    //    virtual Object *get (std::string name);
    //    virtual Object *operator() (std::string name);
    //@}


    //__________________________________________________________________________
    /**
     * @name Event processing
     */
    /**
     * Select callback.
     *
     * @param x x pointer coordinates (window space)
     * @param y y pointer coordinates (window space)
     */
    virtual bool select (int x, int y);

    /**
     * Keyboard action
     *
     * @param keyname  key name
     */
    virtual bool keyboard_action (std::string action, std::string keyname);

    /**
     * Mouse action start
     *
     * Starts a new mouse action
     *
     * @param action action to be started
     * @param x      x pointer coordinates (window space)
     * @param y      y pointer coordinates (window space)
     */
    virtual bool mouse_action_start (std::string action, int x, int y);

    /**
     * Mouse action end
     *
     * Ends the current action.
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


    //__________________________________________________________________________
    /**
     * @name View mode
     */
    /**
     * Set view mode
     *
     * @param mode view mode
     */
    virtual void set_ortho_mode (bool mode);

    /**
     * Get view mode
     *
     * @return view mode
     */
    virtual bool get_ortho_mode (void);
    //@}


    //__________________________________________________________________________
    /**
     * @name Zoom level
     */
    /**
     * Set zoom level.
     *
     * @param zoom zoom level
     */
    virtual void set_zoom (float zoom);

    /**
     * Get zoom level.
     *
     * @return zoom level
     */
    virtual float get_zoom (void);
    //@}


    //__________________________________________________________________________
    /**
     * @name Orientation
     */
    /**
     * Set scene orientation along x and z axis.
     *
     * @param orientation orientation vector (x and z)
     */
    virtual void set_orientation (Orientation orientation);

    /**
     * Set scene orientation along x and z axis.
     *
     * @param x rotation around x axis (degrees)
     * @param z rotation around z axis (degrees)
     */
    virtual void set_orientation (float x=0, float z=0);

    /**
     * Get scene orientation along x and z axis.
     *
     * @return orientation vector (x and z)
     */
    virtual Orientation get_orientation (void);
    //@}


    //__________________________________________________________________________
    /**
     * @name Focusable property
     */
    /**
     * Has scene or any child has focus ? 
     *
     * @return Whether scene or any child has focus or not
     */    
    virtual bool has_focus (int x, int y);
    //@}


    //__________________________________________________________________________
    void set_max_zoom(double max);

protected:

    /**
       * Current pointer position
       */
      Position pointer_;

    /**
     * List of widgets to render.
     */
    std::vector<WidgetPtr> widgets_;

    /**
     * List of objects to render.
     */
    std::vector<ObjectPtr> objects_;

    /**
     * List of lights to use
     */
    //std::vector<LightPtr> lights_;

    /**
     * Focused widget
     */
    Widget * focus_;

    /**
     * orthograpic mode
     */
    bool ortho_mode_;

    /**
     * Zoom level.
     */
    float zoom_;

    /**
     * Zoom maximum level.
     */
    float zoom_max_;

    /**
     * Zoom minimum level.
     */
    float zoom_min_;

    /**
     * Current view vector.
     */
    Vec4f view_;

    /**
     * Initial view orientation.
     */
    Orientation orientation_;

    /**
     * Pointer position at event start.
     */
    Position start_;  
 
};

#endif
