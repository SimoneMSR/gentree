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
#ifndef __LINE_H__
#define __LINE_H__
#include "object.h"
#include "colormap.h"
#include "data.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Line> LinePtr;
#else
    typedef class Line *                  LinePtr;
#endif


/**
 * Represententation of a line
 *
 */
class Line : public Object {
public:

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Line (void);

    /**
     * Destructor.
     */
    virtual ~Line (void);
    //@}


    /**
     *  @name Rendering
     */
    /**
     * 
     */
    void render	(void);
    //@}


    // _________________________________________________________________________

    /**
     *  @name Data 
     */
    /**
     * Set x,y,z data
     *
     * @param xdata data x coordinates
     * @param ydata data y coordinates
     * @param zdata data z coordinates
     */
    virtual void set_xyz_data (DataPtr xdata, DataPtr ydata, DataPtr zdata);

    /**
     * Set x,y,z data + scales
     *
     * @param xdata data x coordinates
     * @param ydata data y coordinates
     * @param zdata data z coordinates
     * @param sdata data scale values
     */
    virtual void set_xyz_s_data (DataPtr xdata, DataPtr ydata, DataPtr zdata,
                                 DataPtr sdata);


    /**
     * Set x,y,z data + colors
     *
     * @param xdata data x coordinates
     * @param ydata data y coordinates
     * @param zdata data z coordinates
     * @param cdata data color values
     */
    virtual void set_xyz_c_data (DataPtr xdata, DataPtr ydata, DataPtr zdata,
                                 DataPtr cdata);

    /**
     * Set x,y,z data + scales + colors
     *
     * @param xdata data x coordinates
     * @param ydata data y coordinates
     * @param zdata data z coordinates
     * @param sdata data scale values
     * @param cdata data color values
     */
    virtual void set_xyz_s_c_data (DataPtr xdata, DataPtr ydata, DataPtr zdata,
                                   DataPtr sdata, DataPtr cdata);
    //@}


    // _________________________________________________________________________

    /**
     * @name Line thickness
     */
    /**
     * Get line thickness
     *
     * @return line thickness
     */
    virtual float get_thickness (void) const;

    /**
     * Set line thickness
     *
     * @param thickness lien thicnkness
     */
    virtual void set_thickness (float thickness);
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
     * Reset all data
     */
    void reset_data (void);    


protected:

    // _________________________________________________________________________


    /**
     * Line thickness
     */
    float thickness_;

    /**
     * Colormap
     */
    ColormapPtr cmap_;

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
     * Data scales
     */
    DataPtr sdata_;

    /**
     * Data colors
     */
    DataPtr cdata_;
};

#endif
