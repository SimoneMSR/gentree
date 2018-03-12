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

#ifndef __CUBE_H__
#define __CUBE_H__
#include "object.h"

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class Cube> CubePtr;
#else
    typedef class Cube * CubePtr;
#endif


/**
 * A simple face sorted cube object.
 *
 */
class Cube : public Object {

public:

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Cube (void);

    /**
     * Destructor
     */
    virtual ~Cube (void);
    //@}


    /**
     *  @name Rendering
     */
    /**
     * Render the cube
     */
    virtual void render (void);
    //@}

};

#endif
