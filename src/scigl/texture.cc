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
#include <cstring>
#include "texture.h"


// _____________________________________________________________________ Texture
Texture::Texture (void) : Object()
{
    texture_ = 0;
    data_ = 0;
    set_size (1.0f, 1.0f, 1.0f);
    set_position (0.0f, 0.0f, 0.0f);
    set_fg_color (0.0f, 0.0f, 0.0f, 0.1f);
    set_bg_color (1.0f, 1.0f, 1.0f, 1.0f);
    cmap_ = ColormapPtr (Colormap::Grey());
    reset_data();

    std::ostringstream oss;
    oss << "Texture_" << id_;
    set_name (oss.str());
}


// ____________________________________________________________________ ~Texture
Texture::~Texture (void)
{
    if (texture_) {
        glDeleteTextures (1, &texture_);
        texture_ = 0;
    }
}


// _______________________________________________________________________ setup
void
Texture::setup (void)
{
    if (not cdata_)
        return;
    if (texture_) {
        glDeleteTextures (1, &texture_);
        texture_ = 0;
    }
    glGenTextures (1, &texture_);
    glBindTexture (GL_TEXTURE_2D, texture_);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


// ______________________________________________________________________ update
void
Texture::update (void)
{
    if (not cdata_)
        return;
    if (not texture_)
        setup();

    glBindTexture (GL_TEXTURE_2D, texture_);
    unsigned int w = cdata_->get_width();
    unsigned int h = cdata_->get_height();
    unsigned int d = cdata_->get_depth();
    //unsigned int t = cdata_->get_type();
    GLfloat v;
    GLbyte *data = (GLbyte *) cdata_->get_data();
    unsigned int data_stride = cdata_->get_stride();

    switch (d) {
    case 1:
        for (unsigned int i=0; i<(w*h); i++) {
            v = *(GLdouble *)(data+i*data_stride);
            Color c = (*cmap_)(v);
            memcpy (&((GLfloat *)data_)[i*4], c.data, 4*sizeof(GLfloat));
        }
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, data_);
        break;
    case 3:
        for (unsigned int i=0; i<(w*h); i++) {
            //printf("%d\n", i);
            data_[i*4+0] = *(GLdouble *)(data+i*data_stride + 0*sizeof(GLdouble));
            data_[i*4+1] = *(GLdouble *)(data+i*data_stride + 1*sizeof(GLdouble));
            data_[i*4+2] = *(GLdouble *)(data+i*data_stride + 2*sizeof(GLdouble));
            data_[i*4+3] = 1.0;
        }
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, data_);
        break;
    case 4:
        for (unsigned int i=0; i<w*h; i++) {
            data_[i*4+0] = *(GLdouble *)(data+i*data_stride + 0*sizeof(GLdouble));
            data_[i*4+1] = *(GLdouble *)(data+i*data_stride + 1*sizeof(GLdouble));
            data_[i*4+2] = *(GLdouble *)(data+i*data_stride + 2*sizeof(GLdouble));
            data_[i*4+3] = *(GLdouble *)(data+i*data_stride + 3*sizeof(GLdouble));
        }
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, data_);
        break;
    default:
        break;
    }
}


// ______________________________________________________________________ render
void
Texture::render (void)
{
    if (not cdata_) {
        return;
    }
    Object::compute_visibility();
    if (!get_visible())
        return;

    if (not texture_) {
        setup();
        update();
    }

    glBindTexture (GL_TEXTURE_2D, texture_);
    glEnable (GL_TEXTURE_2D);
    glEnable (GL_BLEND);
    glPushMatrix();
    glPolygonOffset (1.0f, 1.0f);
    glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    glColor4f (get_bg_color().r,
               get_bg_color().g,
               get_bg_color().b,
               get_bg_color().a*alpha_);
    glNormal3f (0,0,1);

    glBegin (GL_QUADS);
    glTexCoord2f (0, 0);
    glVertex3f (get_position().x-get_size().x/2,
                get_position().y-get_size().y/2,
                get_position().z);
    glTexCoord2f (1, 0);
    glVertex3f (get_position().x+get_size().x/2,
                get_position().y-get_size().y/2,
                get_position().z);
    glTexCoord2f (1, 1);
    glVertex3f (get_position().x+get_size().x/2,
                get_position().y+get_size().y/2,
                get_position().z);
    glTexCoord2f (0, 1);
    glVertex3f (get_position().x-get_size().x/2,
                get_position().y+get_size().y/2,
                get_position().z);
    glEnd();
    glLineWidth (1.0f);
    glEnable (GL_LINE_SMOOTH);
    //glDepthMask (GL_FALSE);
    glDisable (GL_TEXTURE_2D);
    glDisable (GL_POLYGON_OFFSET_FILL);
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);    
    glColor4f (get_br_color().r,
               get_br_color().g,
               get_br_color().b,
               get_br_color().a*alpha_);
    glBegin (GL_QUADS);
    glVertex3f (get_position().x-get_size().x/2,
                get_position().y-get_size().y/2,
                get_position().z);
    glVertex3f (get_position().x+get_size().x/2,
                get_position().y-get_size().y/2,
                get_position().z);
    glVertex3f (get_position().x+get_size().x/2,
                get_position().y+get_size().y/2,
                get_position().z);
    glVertex3f (get_position().x-get_size().x/2,
                get_position().y+get_size().y/2,
                get_position().z);
    glEnd();
    //glDepthMask (GL_TRUE);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}


// __________________________________________________________________ set_c_data
void
Texture::set_c_data (DataPtr data)
{
    if (not data) {
        throw std::invalid_argument
            ("C data must be valid data");
    }
    if (not (data->get_type() == GL_DOUBLE)) {
        throw std::invalid_argument
            ("C data type must be GL_DOUBLE");
    }
    if ((not (data->get_width() > 1)) or
        (not (data->get_height() > 1))) {
        throw std::invalid_argument
            ("C data must be two dimensional (width >= 1, height >= 1)");
    }
    if ((not (data->get_depth() == 1)) and
        (not (data->get_depth() == 3)) and
        (not (data->get_depth() == 4))) {
        throw std::invalid_argument
            ("C data depth must be in [1,3,4]");
    }
    reset_data();
    cdata_ = data;
    data_ = new GLfloat[data->get_width()*data->get_height()*4];
    if (texture_) {
        glDeleteTextures (1, &texture_);
        texture_ = 0;
    }
}


// __________________________________________________________________ reset_data
void
Texture::reset_data (void)
{
#if defined(HAVE_BOOST)
    cdata_ = DataPtr();
#else
    cdata_ = 0;
#endif
    if (data_) {
        delete [] data_;
        data_ = 0;
    }
    if (texture_) {
        glDeleteTextures (1, &texture_);
        texture_ = 0;
    }
}


// _________________________________________________________________ get_texture
unsigned int
Texture::get_texture (void)
{
    return texture_;
}


// ________________________________________________________________ get_colormap
ColormapPtr
Texture::get_colormap (void)
{
    return cmap_;
}


// ________________________________________________________________ set_colormap
void
Texture::set_colormap (ColormapPtr colormap)
{
    cmap_ = colormap;
}
