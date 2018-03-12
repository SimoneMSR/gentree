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
#ifndef __FRAME_H__
#define __FRAME_H__
#include "object.h"

#ifdef HAVE_BOOST
typedef boost::shared_ptr<class Frame> FramePtr;
#else
typedef class Frame *                  FramePtr;
#endif


/**
 * Frame
 */
class Frame : public Object {

public:
    // _________________________________________________________________________

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Frame (void);

    /**
     * Destructor
     */
    virtual ~Frame (void);
    //@}


    // _________________________________________________________________________

    /**
     *  @name Rendering pipeline
     */

    /**
     * Render the object
     */
    virtual void render (void);
    //@}


    // _________________________________________________________________________

    /**
     * @name Visibility
     */
    /**
     * Progressive disappearance of the object.
     *
     * To enforce disappearance, object has to use the alpha_ attribute and
     * keep it up to date with the <code>compute_visibility</code> method.
     *
     * @param timeout time (ms) for fade out object
     */
    virtual void hide (float timeout=0);

    /**
     * Progressive appearance of the object.
     *
     * To enforce appearance, object has to use the alpha_ attribute and 
     * keep it up to date with the <code>compute_visiblity</code> method.
     *
     * @param timeout time (ms) for fade out object
     */
    virtual void show (float timeout=0);
    //@}



};

#endif
