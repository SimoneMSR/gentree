/*
 * histrogram.h
 *
 *  Created on: 23/feb/2011
 *      Author: simonemsr
 */

#ifndef GRAM_H_
#define GRAM_H_





#include "object.h"
#include "colormap.h"
#include "data.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Histogram> HistogramPtr;
#else
    typedef class Histogram *                  HistogramPtr;
#endif


/**
 * Represententation of a cloud
 *
 */
class Histogram : public Object {
public:

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Histogram (void);

    /**
     * Destructor.
     */
    virtual ~Histogram (void);
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
     * @param y1data data y coordinates of base
     * @param y2data data y coordinates of top
     * @param zdata data z coordinates
     */
    virtual void set_xyz_data (DataPtr xdata,
                               DataPtr y1data, DataPtr y2data,
                               DataPtr zdata);




    /**
     * Set x,y,z data + colors
     *
     * @param xdata data x coordinates
     * @param ydata data y coordinates
     * @param zdata data z coordinates
     * @param cdata data color values
     */
    virtual void set_xyz_c_data (DataPtr xdata,
                                 DataPtr ydata,
                                 DataPtr zdata,
                                 DataPtr rdata,
                                 DataPtr cdata);





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
     * Data height coordinates
     */
    DataPtr zdata_;

    /**
        * Data yr coordinates of bottom
        */
       DataPtr rdata_;


    /**
     * Data colors
     */
    DataPtr cdata_;
};

#endif /* GRAM_H_ */

