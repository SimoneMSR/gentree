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
#ifndef __COLORBAR_H__
#define __COLORBAR_H__
#include "widget.h"
#include "colormap.h"

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class Colorbar> ColorbarPtr;
#else
    typedef class Colorbar * ColorbarPtr;
#endif


/**
 * Representation of a colormap.
 *
 * A colorbar is a widget that is used to display a specific colormap.
 *
 */
class Colorbar: public Widget {
public:

    /**
     * @name Creation/Destruction
     */

    /**
     * Default constructor
     */
    Colorbar (void);

    /**
     * Destructor
     */    
    virtual ~Colorbar (void);
    //@}



    /**
     *  @name Rendering
     */
    /**
     * Build the colorbar texture.
     */
    virtual void setup (void);

    /**
     * Render the colorbar.
     */
    virtual void render (void);
    //@}


    /**
     *  @name Title
     */
    /**
     * Set title.
     *
     * @param title title
     */
    virtual void set_title (std::string title);

    /**
     * Get title.
     *
     * @return title
     */
    virtual std::string get_title (void);

    /**
     * Get title.
     *
     * @return title
     */
    virtual std::string title (void);
    //@}



    /**
     *  @name Colormap
     */
    /**
     * Set colormap.
     *
     * @param cmap colormap
     */
    virtual void      set_cmap (Colormap *cmap);

    /**
     * Get colormap.
     *
     * @return colormap
     */
    virtual Colormap *get_cmap (void);

    /**
     * Get colormap.
     *
     * @return colormap
     */
    virtual Colormap *cmap (void);
    //@}

protected:
    /**
     * Colorbar that is displayed.
     */
    Colormap *cmap_;

    /**
     * OpenGL texture representing the colormap.
     */
    GLuint texture_;

    /**
     * Colorbar title.
     */
    std::string title_;
};

#endif
