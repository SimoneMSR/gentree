/*
 * Copyright (C) 2008 Nicolas P. Rougier
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either  version 3 of the  License, or (at your  option) any later
 * version.
 *
 * This program is  distributed in the hope that it will  be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR  A  PARTICULAR PURPOSE.  See  the GNU  General  Public  License for  more
 * details.
 *
 * You should have received a copy  of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "object.h"
#include "colormap.h"
#include "texture.h"
#include "data.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Surface> SurfacePtr;
#else
    typedef class Surface *                  SurfacePtr;
#endif


/**
 * Vertex structure
 */
struct Vertex {
    union {
        float data[12];
        struct {
            float x,y,z;
            float u0,v0;
            float nx,ny,nz;
            float r,g,b,a;
        };
    };
};


/**
 *
 *
 *
 *
 *
 *
 */
class Surface : public Object {
public:

    // _________________________________________________________________________

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Surface (void);

    /**
     * Destructor.
     */
    virtual ~Surface (void);
    //@}


    // _________________________________________________________________________

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
     *  @name Data
     */

    virtual void set_z_data (DataPtr zdata);
    virtual void set_z_c_data (DataPtr zdata,
                               DataPtr cdata);
    //    virtual void set_z_t_data (DataPtr zdata, TexturePtr texture);
    //    virtual void set_xyz_data (DataPtr xdata,
    //                               DataPtr ydata,
    //                               DataPtr zdata,
    //    virtual void set_xyz_c_data (DataPtr xdata,
    //                                 DataPtr ydata,
    //                                 DataPtr zdata,
    //                                 DataPtr cdata,
    //    virtual void set_xyz_t_data (DataPtr xdata,
    //                                 DataPtr ydata,
    //                                 DataPtr zdata,
    //                                 TexturePtr texture,

    void reset_data (void);
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


protected:

    // _________________________________________________________________________


    /**
     * Data x coordinates
     */
    DataPtr xdata_;

    /**
     * Data y coordinates
     */
    DataPtr ydata_;

    /**
     * Data z coordinates
     */
    DataPtr zdata_;

    /**
     * Data colors
     */
    DataPtr cdata_;

    /**
     *
     */
    TexturePtr texture_;

    /**
     *
     */
    ColormapPtr colormap_;



    /**
     * Number of vertices.
     */
    int nvertices_;	

    /**
     * Number of indices for surface
     */
    int surface_nindices_;

    /**
     * Number of indices for grid
     */
    int grid_nindices_;

    /**
     * Number of indices for border
     */
    int border_nindices_;

    /**
     * Vertex object buffer names
     */
    GLuint vertex_name_;
    GLuint surface_index_name_;
    GLuint grid_index_name_;
    GLuint border_index_name_;

};

#endif
