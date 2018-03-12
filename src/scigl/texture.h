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
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "object.h"
#include "colormap.h"
#include "data.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Texture> TexturePtr;
#else
    typedef class Texture *                  TexturePtr;
#endif

/**
 * Represententation of a data source as a texture.
 */
class Texture : public Object {
public:

    // ------------------------------------------------------------------------

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Texture (void);

    /**
     * Destructor.
     */
    virtual ~Texture (void);
    //@}

    // ------------------------------------------------------------------------

    /**
     *  @name Rendering
     */
    /**
     * 
     */
    void setup (void);

    /**
     * 
     */
    void update (void);

    /**
     * 
     */
    void render	(void);
    //@}


    // _________________________________________________________________________

    /**
     *  @name Data source
     */
    /**
     * Set color data
     */
    virtual void set_c_data (DataPtr data);
    //@}


    // _________________________________________________________________________

    /**
     *  @name Colormap
     */
    /**
     * Get colormap
     */
    virtual ColormapPtr get_colormap (void);

    /**
     * Set colormap
     */
    virtual void set_colormap (ColormapPtr colormap);
    //@}


    // _________________________________________________________________________

    /**
     *  @name OpenGL texture name
     */
    /**
     * Get OpenGL texture name
     */
    virtual unsigned int get_texture (void);
    //@}


protected:
    
    // _________________________________________________________________________

    /**
     * Reset all data
     */
    void reset_data (void);    


protected:

    // _________________________________________________________________________

    /**
     * Data source
     */
    DataPtr cdata_;
    
    /**
     * Internal buffer
     */
    GLfloat *data_;

    /**
     * Colormap for source data of depth = 1
     */
    ColormapPtr cmap_;

    /**
     * Associated GL texture.
     */
    GLuint texture_;
};

#endif
