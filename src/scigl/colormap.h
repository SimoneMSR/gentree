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
#ifndef __COLORMAP_H__
#define __COLORMAP_H__

#include <vector>
#include <string>
#include "vec4f.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Colormap> ColormapPtr;
#else
    typedef class Colormap *                  ColormapPtr;
#endif

/**
 * A colormap represents a value range using colors.
 *
 * A colormap is made of couple of (value,colors) where value range from min_
 * to max_ Any value between min_ and max_ is then linearly interpolated to
 * get the corresponding color. The scale of the colormap can be changed using
 * the ::scale method that allows to represent an arbitrary range. Any value
 * above maximum value will be associated with the color of max. Any value
 * below minimum value will be associated with the color of min. 
 *
 * - Ice <br>
 * \htmlonly <img src="ice.png"/> \endhtmlonly
 *
 * - Fire <br>
 * \htmlonly <img src="fire.png"/> \endhtmlonly
 *
 * - Hot <br>
 * \htmlonly <img src="hot.png"/> \endhtmlonly
 *
 * - IceAndFire <br>
 * \htmlonly <img src="ice_and_fire.png"/> \endhtmlonly
 *
 * - Grey <br>
 * \htmlonly <img src="grey.png"/> \endhtmlonly
 *
 * - Red <br>
 * \htmlonly <img src="red.png"/> \endhtmlonly
 *
 * - LightRed <br>
 * \htmlonly <img src="light_red.png"/> \endhtmlonly
 *
 * - Green <br>
 * \htmlonly <img src="green.png"/> \endhtmlonly
 *
 * - LightGreen <br>
 * \htmlonly <img src="light_green.png"/> \endhtmlonly
 *
 * - Blue <br>
 * \htmlonly <img src="blue.png"/> \endhtmlonly
 *
 * - LightBlue <br>
 * \htmlonly <img src="light_blue.png"/> \endhtmlonly
 *
 */
class Colormap {
public:

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor.
     */
    Colormap (void);

    /**
     * Copy constructor.
     *
     * @param other colormap to copy
     */
    Colormap (const Colormap &other);            

    /**
     * Destructor.
     */
    virtual ~Colormap (void);
    //@}

    /**
     * @name Main
     */
    /**
     * Affectation operator.
     *
     * @param other colormap to copy
     * @return the current copied colormap
     */
    Colormap &operator= (const Colormap &other);

    /**
     * Clear the colormap.
     */
    void clear (void);

    /**
     * Append a new (value,color).
     *
     * @param value value between 0 and 1
     * @param color associated color
     */
    void append (float value, Color color);

    /**
     * Get color associated with value.
     *
     * Color is extracted from the available samples.
     *
     * @param value value to get associated color from
     * @return color associated with value
     */
    Color operator() (const float value);
    //@}


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

    /**
     *  @name Value scale.
     */
    /**
     *
     */
    void scale (float min, float max);

    /**
     * Set minimum value.
     *
     * @param value minimum value
     */
    virtual void set_min (float value);

    /**
     * Get minimum value.
     *
     * @return minimum value
     */
    virtual float get_min (void);

    /**
     * Set maximum value.
     *
     * @param value maximum value
     */
    virtual void set_max (float value);

    /**
     * Get maximum value.
     *
     * @return maximum value
     */
    virtual float get_max (void);
    //@}

    /**
     *  @name Standard colormaps
     */
    /**
     * Colormap that goes from blue to white.
     *
     * @return a colormap
     */
    static ColormapPtr Ice (void);

    /**
     * Colormap that goes white, yellow and red.
     *
     * @return a colormap
     */
    static ColormapPtr Fire (void);

    /**
     * Colormap that goes blue, white, yellow and red.
     *
     *
     *
     * @return a colormap
     */
    static ColormapPtr IceAndFire (void);

    /**
     * Colormap that goes from black, red, yellow and white.
     *
     * @return a colormap
     */
    static ColormapPtr Hot (void);

    /**
     * Colormap that goes from black to white.
     *
     * @return a colormap
     */
    static ColormapPtr Grey (void);

    /**
     * Colormap that goes from black to red
     *
     * @return a colormap
     */
    static ColormapPtr Red (void);

    /**
     * Colormap that goes from black to green.
     *
     * @return a colormap
     */
    static ColormapPtr Green (void);

    /**
     * Colormap that goes from black to blue.
     *
     * @return a colormap
     */
    static ColormapPtr Blue (void);

    /**
     * Colormap that goes from white to red
     *
     * @return a colormap
     */
    static ColormapPtr LightRed (void);

    /**
     * Colormap that goes from white to green.
     *
     * @return a colormap
     */
    static ColormapPtr LightGreen (void);

    /**
     * Colormap that goes from white to blue.
     *
     * @return a colormap
     */
    static ColormapPtr LightBlue (void);
    // @}

protected:
    /**
     * Compute the exact color associated with the value.
     * 
     * @param value value to get associated color from
     * @return exact color associated with value
     */
    Color color (float value);

    /**
     * Recompute all color samples.
     */
    void resample (void);

protected:
    /**
     * Colors composing the colormap.
     */
    std::vector<Color> colors_;

    /**
     * Value corresponding to colors.
     */
    std::vector<float> values_;

    /**
     * Color samples for fast access to the colormap.
     */
    std::vector<Color> samples_;

    /**
     * Samples resolution.
     */
    int resolution_;

    /**
     * Name of the colormap.
     */
    std::string name_;

    /**
     * Minimum value of the colormap.
     */
    float min_;

    /**
     * Maximum value of the colormap.
     */
    float max_;

};

#endif
