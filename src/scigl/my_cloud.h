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
#ifndef __CLOUD_H__
#define __CLOUD_H__
#include "object.h"
#include "colormap.h"
#include "data.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Cloud> CloudPtr;
#else
    typedef class Cloud *                  CloudPtr;
#endif

#define QUAD_DIM 0.01

/**
 * Represententation of a cloud
 *
 */
class Cloud : public Object {
public:

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Cloud (void);

    /**
     * Destructor.
     */
    virtual ~Cloud (void);
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
     * Set x,y,c data
     *
     * @param xdata data x coordinates
     * @param ydata data y coordinates
     * @param cdata data class
     */
    virtual void set_xyc_data (DataPtr xdata,
                               DataPtr ydata,
                               DataPtr cdata);



    //@}
    /**
         * Set x,y data
         *
         * @param xdata data x coordinates
         * @param ydata data y coordinates
         */

    // ________________________________________________________________ set_xy_data
    void set_xy_data(DataPtr xdata, DataPtr ydata);

    // _________________________________________________________________________

    /**
     * @name Cloud thickness
     */
    /**
     * Get cloud thickness
     *
     * @return cloud thickness
     */
    virtual float get_thickness (void) const;

    /**
     * Set cloud thickness
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
     * Point thickness
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
     * Data colors
     */
    DataPtr cdata_;
};

#endif
