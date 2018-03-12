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
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __DATA_H__
#define __DATA_H__
#include "object.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Data> DataPtr;
#else
    typedef class Data *                  DataPtr;
#endif


/**
 * Encapsulation of raw data
 *
 */
class Data {
public:

    // _________________________________________________________________________

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Data (void);

    /**
     * Destructor
     */
    virtual ~Data (void);
    //@}


    // _________________________________________________________________________
    /**
     * @name Set whole data structure
     */
    /**
     * Set data structure
     *
     * @param data   Specifies a pointer to the data array.
     *
     * @param width  Specified the width of the data array. 
     *
     * @param height Specified the height of the data array. 
     *
     * @param depth  Specifies the number of component per data. The initial
     *               value is 1.
     *
     * @param type   Specifies the type of each data in the array. Symbolic
     *               constants GL_BYTE, GL_SHORT, GL_INT, GL_FLOAT, and
     *               GL_DOUBLE are accepted. The initial value is GL_FLOAT.
     *
     * @param stride Specifies the byte offset between consecutive data. If
     *               stride is 0, the data are understood to be tightly
     *               packed in the array. The initial value is 0.
     */
    virtual void set (void *data,
                      unsigned int width = 1,
                      unsigned int height = 1,
                      unsigned int depth = 1,
                      unsigned int type = GL_FLOAT,
                      unsigned int stride = 0);
    //@}

    // _________________________________________________________________________

    /**
     * @name Width
     */
    /**
     * Get width
     *
     * @return width of the data array
     */
    virtual unsigned int get_width (void) const;

    /**
     * Set width
     *
     * @param width Specifies the height of the data array. 
     */
    virtual void set_width (const unsigned int width);
    //@}

    
    // _________________________________________________________________________

    /**
     * @name Height
     */
    /**
     * Get height
     *
     * @return height of the data array
     */
    virtual unsigned int get_height (void) const;

    /**
     * Set height
     *
     * @param height Specifies the height of the data array. 
     */
    virtual void set_height (const unsigned int height);
    //@}
    

    // _________________________________________________________________________

    /**
     * @name Depth
     */
    /**
     * Get depth
     *
     * @return number of components per data.
     */
    virtual unsigned int get_depth (void) const;

    /**
     * Set depth
     *
     * @param depth  Specifies the number of components per data. The initial
     *               value is 1.
     */
    virtual void set_depth (const unsigned int depth);
    //@}


    // _________________________________________________________________________

    /**
     * @name Data pointer
     */
    /**
     * Get data pointer.
     *
     * @return data pointer to the data array
     */
    virtual const void *get_data (void) const;

    /**
     * Set data pointer
     *
     * @param data Specifies a pointer to the data array.
     */
    virtual void set_data (const void *data);
    //@}


    // _________________________________________________________________________

    /**
     * @name Data type
     */
    /**
     * Get data type
     *
     * @return type of each data in the array as a symbolic constant between
     *         GL_BYTE, GL_SHORT, GL_INT, GL_FLOAT, or GL_DOUBLE.
     */
    virtual unsigned int get_type (void) const;

    /**
     * Set data type
     *
     * @param type   Specifies the type of each data in the array. Symbolic
     *               constants GL_BYTE, GL_SHORT, GL_INT, GL_FLOAT, and
     *               GL_DOUBLE are accepted. The initial value is GL_FLOAT.
     *
     */
    virtual void set_type (const unsigned int type = GL_FLOAT);
    //@}

    // _________________________________________________________________________

    /**
     * @name Data stride
     */
    /**
     * Get data stride
     *
     * @return byte offset between consecutive data
     */
    virtual unsigned int get_stride (void) const;

    /**
     * Set data stride
     *
     * @param stride Specifies the byte offset between consecutive data. If
     *               stride is 0, the data are understood to be tightly
     *               packed in the array. The initial value is 0.
     */
    virtual void set_stride (const unsigned int stride = 0);
    //@}


protected:

    // _________________________________________________________________________

    /**
     * Pointer to actual data
     */
    const void * data_;

    /**
     * Type
     */
    unsigned int type_;

    /**
     * Width
     */
    unsigned int width_;

    /**
     * Height
     */
    unsigned int height_;

    /**
     * Depth
     */
    unsigned int depth_;

    /**
     * Stride
     */
    unsigned int stride_;
};

#endif
