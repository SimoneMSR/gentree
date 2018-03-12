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
#include <stdexcept>
#include "data.h"


// ________________________________________________________________________ Data
Data::Data (void)
{
    width_ = height_ = depth_ = 0;
    type_ = 0;
    data_ = 0;
    stride_ = 0;
}


// _______________________________________________________________________ ~Data
Data::~Data (void)
{}


// _________________________________________________________________________ set
void
Data::set (void *data, unsigned int width, unsigned int height,
           unsigned int depth, unsigned int type, unsigned int stride)
{
    set_data(data);
    set_width(width);
    set_height(height);
    set_depth(depth);
    set_type(type);
    set_stride(stride);
}

// ___________________________________________________________________ get_width
unsigned int
Data::get_width (void) const
{
    return width_;
}


// ___________________________________________________________________ set_width
void
Data::set_width (const unsigned int width)
{
    width_ = width;
}


// ___________________________________________________________________ get_height
unsigned int
Data::get_height (void) const
{
    return height_;
}


// ___________________________________________________________________ set_height
void
Data::set_height (const unsigned int height)
{
    height_ = height;
}


// ___________________________________________________________________ get_depth
unsigned int
Data::get_depth (void) const
{
    return depth_;
}


// ___________________________________________________________________ set_depth
void
Data::set_depth (const unsigned int depth)
{
    if (depth > 0) {
        depth_ = depth;
        return;
    }
    throw std::invalid_argument ("Data depth must be at least 1");    
}


// ___________________________________________________________________ get_type
unsigned int
Data::get_type (void) const
{
    return type_;
}


// ___________________________________________________________________ set_type
void
Data::set_type (const unsigned int type)
{
    if ((type == GL_BYTE) or
        (type == GL_SHORT) or
        (type == GL_INT) or
        (type == GL_FLOAT) or
        (type == GL_DOUBLE)) {
        type_ = type;
        return;
    }
    throw std::invalid_argument
        ("Data type must be GL_[BYTE,SHORT,INT,FLOAT,DOUBLE]");
}


// __________________________________________________________________ get_stride
unsigned int
Data::get_stride (void) const
{
    if (stride_ != 0)
        return stride_;
    else if (type_ == GL_BYTE)
        return depth_*sizeof (GLbyte);
    else if (type_ == GL_SHORT)
        return depth_*sizeof (GLshort);
    else if (type_ == GL_INT)
        return depth_*sizeof (GLint);
    else if (type_ == GL_FLOAT)
        return depth_*sizeof (GLfloat);
    else if (type_ == GL_DOUBLE)
        return depth_*sizeof (GLdouble);
    return 0;
}


// __________________________________________________________________ set_stride
void
Data::set_stride (const unsigned int stride)
{
    stride_ = stride;
} 

// ___________________________________________________________________ get_data
const void *
Data::get_data (void) const
{
    return data_;
}


// ___________________________________________________________________ set_data
void
Data::set_data (const void *data)
{
    data_ = data;
}
