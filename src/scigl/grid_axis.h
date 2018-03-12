/*
 * grid_axis.h
 *
 *  Created on: 27/mar/2011
 *      Author: simonemsr
 */

#ifndef GRID_AXIS_H_
#define GRID_AXIS_H_


#include <string>
#include <vector>
#include "font.h"
#include "object.h"


#ifdef HAVE_BOOST
typedef boost::shared_ptr<class Grid_Axis> Grid_AxisPtr;
#else
typedef class Grid_Axis *Grid_AxisPtr;
#endif

/**
 * Axis with ticks and label.
 *
 */
class Grid_Axis : public Object {
public:

    // _________________________________________________________________________

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Grid_Axis ();

    /**
     * Destructor.
     */
    virtual ~Grid_Axis (void);
    //@}

   // _________________________________________________________________________

    /**
     *  @name Rendering
     */
    /**
     * Rendering
     */
    virtual void render (void);
    //@}


    /**
     * Convenience function to set major/minor ticks and labels at once
     *
     * @param min             Value of first major ticks (x=0) (default -1)
     * @param max             Value of last major ticks (x=1) (default +1)
     * @param major_n         Number of major ticks (default 3)
     * @param minor_n         Number of minor ticks (default 11)
     * @param major_size      Size of major ticks (default .050)
     * @param minor_size      Size of minor ticks (default .025)
     * @param major_thickness Thickness of major ticks (default 2)
     * @param minor_thickness Thickness of minor ticks (default 1)
     *
     */
    void set (float min = -1, float max = 1,
              int major_n = 3, int minor_n = 11,
              float major_size = 0.10,  float minor_size = 0.05,
              float major_thickness = 2, float minor_thickness = 1);


    /**
     * Clear all ticks
     */
    void clear (void);

    /**
     * Add a new tick
     *
     * @param tick  position, size & thickness
     * @param label label
     */
    void add(Tick tick, std::string label="");

    /**
     * Set limit of axis
     */
    void set_limit(int);

    // _________________________________________________________________________

    /**
     * @name Start point
     */
    /**
     * Set start point
     *
     * @param position Position a tuple of 3 floats.
     */
    virtual void set_start (Position position);

    /**
     * Set start point
     *
     * @param x position on x axis
     * @param y position on y axis
     * @param z position on z axis
     * @param w not used
     */
    virtual void set_start (float x=0,
                            float y=0,
                            float z=0,
                            float w=0);

    /**
     * Get start point
     *
     * @return start point
     */
    virtual Position get_start (void) const;
    //@}


    // _________________________________________________________________________

    /**
     * @name End point
     */
    /**
     * Set end point
     *
     * @param position Position a tuple of 3 floats.
     */
    virtual void set_end (Position position);

    /**
     * Set end point
     *
     * @param x position on x axis
     * @param y position on y axis
     * @param z position on z axis
     * @param w not used
     */
    virtual void set_end (float x=0,
                          float y=0,
                          float z=0,
                          float w=0);

    /**
     * Get end point
     *
     * @return end point
     */
    virtual Position get_end (void) const;
    //@}


    // _________________________________________________________________________

    /**
     * @name Title font size
     */
    /**
     * Get title font size
     *
     * @return title font size
     */
    virtual float get_title_fontsize (void) const;

    /**
     * Set title font size
     *
     * @param size new title font size
     */
    virtual void set_title_fontsize (float size);
    //@}

    // _________________________________________________________________________

    /**
     * @name Ticks font size
     */
    /**
     * Get ticks font size
     *
     * @return ticks font size
     */
    virtual float get_ticks_fontsize (void) const;

    /**
     * Set ticks font size
     *
     * @param size new ticks font size
     */
    virtual void set_ticks_fontsize (float size);
    //@}

    // _________________________________________________________________________

    /**
     * @name Thickness
     */
    /**
     * Get thickness
     *
     * @return thickness
     */
    virtual float get_thickness (void) const;

    /**
     * Set thickness
     *
     * @param thickness thickness
     */
    virtual void set_thickness (float thickness);
    //@}


    // _________________________________________________________________________

    /**
     * @name Ticks & Label orientation
     */
    /**
     * Get orientation
     *
     * @return orientation
     */
    virtual float get_orientation (void) const;

    /**
     * Set orientation
     *
     * @param orientation orientation
     */
    virtual void set_orientation (float orientation);
    //@}


    // _________________________________________________________________________

    /**
     * @name Title
     */
    /**
     * Set title
     *
     * @param title new title
     */
    virtual void set_title (std::string title);

    /**
     * Get title
     *
     * @return title
     */
    virtual std::string get_title (void) const;
    //@}


protected:

    // _________________________________________________________________________

    /**
     * Axis start point
     */
    Position start_;

    /**
     * Axis end point
     */
    Position end_;

    /**
     * Ticks positions (x), size (y) & thickness (z)
     */
    std::vector<Tick> ticks_;

    /**
     * Ticks labels
     */
    std::vector<std::string> tick_labels_;

    /**
     * Title
     */
    std::string title_;

    /**
     * Ticks font size
     */
    float ticks_fontsize_;

    /**
     * Title font size
     */
    float title_fontsize_;

    /**
     * Thickness
     */
    float thickness_;

    /**
     * Orientation
     */
    float orientation_;

    /**
     *  Superior limit of axis
     */
    int limit;
};


#endif /* GRID_AXIS_H_ */
