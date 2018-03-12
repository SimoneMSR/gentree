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
#include "surface.h"


// _____________________________________________________________________ Surface
Surface::Surface (void) : Object()
{
	nvertices_          = 0;
 	surface_nindices_   = 0;
 	grid_nindices_      = 0;
 	border_nindices_    = 0;
    vertex_name_        = 0;
    surface_index_name_ = 0;
    grid_index_name_    = 0;
    border_index_name_  = 0;
    colormap_ = Colormap::Hot();
    reset_data();
    set_size (1.0f, 1.0f, 1.0f);
    set_position (0.0f, 0.0f, 0.0f);
    set_fg_color (0.0f, 0.0f, 0.0f, 0.1f);
    set_bg_color (1.0f, 1.0f, 1.0f, 1.0f);
    set_alpha (1.0);

    std::ostringstream oss;
    oss << "Surface_" << id_;
    set_name (oss.str());
}


// ____________________________________________________________________ ~Surface
Surface::~Surface (void)
{
    reset_data();
}

// _______________________________________________________________________ setup
void
Surface::setup (void)
{
    if (not zdata_) {
        return;
    }

    if (not vertex_name_) {
        glewInit();
        glGenBuffers(1, &vertex_name_);
        glGenBuffers(1, &surface_index_name_);
        glGenBuffers(1, &grid_index_name_);
        glGenBuffers(1, &border_index_name_);
    }

	nvertices_ = int (zdata_->get_width()*zdata_->get_height());
    surface_nindices_ = int((zdata_->get_width()-1)*(zdata_->get_height()-1))*4;
    grid_nindices_ = int((zdata_->get_width()-1)*(zdata_->get_height()-1))*4;
    border_nindices_ = int(zdata_->get_width()*2 + zdata_->get_height()*2);

    // Surface indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface_index_name_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (surface_nindices_*sizeof(int)), NULL, GL_STREAM_DRAW);
    int *indices = (int *) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (indices == NULL)
        throw std::runtime_error("Cannot map vertex buffer object (indices)"); 
    int j = 0;
    
	for (int y=0; y < int(zdata_->get_height()-1); y++) {
        for (int x=0; x < int(zdata_->get_width()-1); x++) {
            int index = int (y*zdata_->get_width() + x);
            indices[j++] = index;
            indices[j++] = index+1;
            indices[j++] = index+int(zdata_->get_width()+1);
            indices[j++] = index+int(zdata_->get_width());
        }
    }
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // Grid indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid_index_name_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (grid_nindices_*sizeof(int)), NULL, GL_STREAM_DRAW);
    indices = (int *) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (indices == NULL)
        throw std::runtime_error("Cannot map vertex buffer object (indices)"); 
    j = 0;    
	for (int y=0; y < int(zdata_->get_height()-1); y++) {
        for (int x=0; x < int(zdata_->get_width()-1); x++) {
            int index = int (y*zdata_->get_width() + x);
            indices[j++] = index;
            indices[j++] = index+1;
            indices[j++] = index+int(zdata_->get_width()+1);
            indices[j++] = index+int(zdata_->get_width());
        }
    }
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // Border indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_index_name_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (border_nindices_*sizeof(int)), NULL, GL_STREAM_DRAW);
    indices = (int *) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (indices == NULL)
        throw std::runtime_error("Cannot map vertex buffer object (indices)"); 
    j = 0;
    for (int x=0; x < int(zdata_->get_width()); x++)
        indices[j++] = int (x);
    for (int y=0; y < int(zdata_->get_height()); y++)
        indices[j++] = int (y*zdata_->get_width() + zdata_->get_width()-1);
    for (int x=int(zdata_->get_width())-1; x >=0; x--)
        indices[j++] = (int(zdata_->get_height())-1)*zdata_->get_width() + x;
    for (int y=int(zdata_->get_height())-1; y>=0; y--)
        indices[j++] = int (y*zdata_->get_width());
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // Vertices, normals & tex coords
    glBindBuffer(GL_ARRAY_BUFFER, vertex_name_);
    glBufferData(GL_ARRAY_BUFFER, (nvertices_*sizeof(Vertex)), NULL, GL_STREAM_DRAW);
    Vertex *vertices = (Vertex *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (vertices == NULL)
        throw std::runtime_error("Cannot map vertex buffer object (vertices)"); 
	for (int y=0; y < int(zdata_->get_height()); y++) {
        for (int x=0; x < int(zdata_->get_width()); x++) {
            int index = int (x+y*zdata_->get_width());
            vertices[index].x  = (x/float(zdata_->get_width()-1)) - .5;
            vertices[index].y  = (y/float(zdata_->get_height()-1)) - .5;
            vertices[index].u0 = 1-(x/float(zdata_->get_width()));
            vertices[index].v0 =   (y/float(zdata_->get_height()));
        }
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// ______________________________________________________________________ update
void
Surface::update (void)
{
    if (not vertex_name_) {
        setup();
        if (not vertex_name_) {
            return;
        }
    }

    unsigned int width = zdata_->get_width();
    unsigned int height = zdata_->get_height();

    glBindBuffer(GL_ARRAY_BUFFER, vertex_name_);
    Vertex *vertices = (Vertex *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (vertices == NULL)
        throw std::runtime_error("Cannot map vertex buffer object (vertices)"); 

    GLbyte *zdata = (GLbyte *) zdata_->get_data();
    unsigned int zdata_stride = zdata_->get_stride();


    // Z
    // -------------------------------------------------------------------------
    if (not cdata_) {
        for (unsigned int j=0; j<height; j++) {
            for (unsigned int i=0; i<width; i++) {
                vertices[j*width+i].z = * (GLdouble *) (zdata+(j*width+i)*zdata_stride);
            }
        }
    } else if (cdata_) {
        GLbyte *cdata = (GLbyte *) cdata_->get_data();
        unsigned int cdata_stride = cdata_->get_stride();

        // Z + C
        // -------------------------------------------------------------------------
        if ((cdata_->get_depth() == 1)) {
            for (unsigned int j=0; j<height; j++) {
                for (unsigned int i=0; i<width; i++) {
                    unsigned int k = j*width+i;
                    vertices[k].z = * (GLdouble *) (zdata+(k)*zdata_stride);
                    Color c = (*colormap_)(vertices[k].z);
                    vertices[k].r = c.r;
                    vertices[k].g = c.g;
                    vertices[k].b = c.b;
                    vertices[k].a = c.a;
                }
            }

        // Z + RGB
        // -------------------------------------------------------------------------
        } else if ((cdata_->get_depth() == 3)) {
            for (unsigned int j=0; j<height; j++) {
                for (unsigned int i=0; i<width; i++) {
                    unsigned int k = j*width+i;
                    vertices[k].z = * (GLdouble *)(zdata+(k)*zdata_stride);
                    vertices[k].r = * (GLdouble *)(cdata+(k)*cdata_stride + 0*sizeof(GLdouble));
                    vertices[k].g = * (GLdouble *)(cdata+(k)*cdata_stride + 1*sizeof(GLdouble));
                    vertices[k].b = * (GLdouble *)(cdata+(k)*cdata_stride + 2*sizeof(GLdouble));
                    vertices[k].a = 1;
                }
            }        

        // Z + RGBA
        // -------------------------------------------------------------------------
        } else if ((cdata_->get_depth() == 4)) {
            for (unsigned int j=0; j<height; j++) {
                for (unsigned int i=0; i<width; i++) {
                    unsigned int k = j*width+i;
                    vertices[k].z = * (GLdouble *)(zdata+(k)*zdata_stride);
                    vertices[k].r = * (GLdouble *)(cdata+(k)*cdata_stride + 0*sizeof(GLdouble));
                    vertices[k].g = * (GLdouble *)(cdata+(k)*cdata_stride + 1*sizeof(GLdouble));
                    vertices[k].b = * (GLdouble *)(cdata+(k)*cdata_stride + 2*sizeof(GLdouble));
                    vertices[k].a = * (GLdouble *)(cdata+(k)*cdata_stride + 3*sizeof(GLdouble));
                }
            }        
        }
    }


    // zeroify normals
    for (int i = 0; i<nvertices_; i++) {
        vertices[i].nx = 0;
        vertices[i].ny = 0;
        vertices[i].nz = 0;
    }

    // Compute normals
    for(int y = 0; y < int(zdata_->get_height() -1); y++){
        for(int x = 0; x < int(zdata_->get_width() -1); x++){
            Vec4f p1, p2, p3, v1, v2, tmp;
            int v[4];
            // calculate the positions int the vertex buffer of the adjacent vertices
            // v[0] is current vertex
            v[0] = int(y*zdata_->get_width()+x);
            v[1] = int(y*zdata_->get_width()+x+1);
            v[2] = int((y+1)*zdata_->get_width()+x);
            v[3] = int((y+1)*zdata_->get_width()+x+1);
            // store the positions of first triangle into 3 usable vertices
            p1 = Vec4f (vertices[v[0]].data);
            p2 = Vec4f (vertices[v[3]].data);
            p3 = Vec4f (vertices[v[2]].data);
            // calculate the triangle sides
            v1 = p2 - p1;
            v2 = p3 - p1;
            // calculate normal
            tmp = v1.cross(v2);

            // add normal to the normal of all the vertices in the triangle
            vertices[v[0]].nx = vertices[v[0]].nx + tmp.x;
            vertices[v[0]].ny = vertices[v[0]].ny + tmp.y;
            vertices[v[0]].nz = vertices[v[0]].nz + tmp.z;

            vertices[v[3]].nx = vertices[v[3]].nx + tmp.x;
            vertices[v[3]].ny = vertices[v[3]].ny + tmp.y;
            vertices[v[3]].nz = vertices[v[3]].nz + tmp.z;

            vertices[v[2]].nx = vertices[v[2]].nx + tmp.x;
            vertices[v[2]].ny = vertices[v[2]].ny + tmp.y;
            vertices[v[2]].nz = vertices[v[2]].nz + tmp.z;

            // store the positions of second triangle into 3 usable vertices
            p1 = Vec4f(vertices[v[1]].data);
            p2 = Vec4f(vertices[v[3]].data);
            p3 = Vec4f(vertices[v[0]].data);
            // calculate the triangle sides
            v1 = p2 - p1;
            v2 = p3 - p1;
            // calculate normal
            tmp = v1.cross(v2);            
            // add normal to the normal of all the vertices in the triangle
            vertices[v[1]].nx = vertices[v[1]].nx + tmp.x;
            vertices[v[1]].ny = vertices[v[1]].ny + tmp.y;
            vertices[v[1]].nz = vertices[v[1]].nz + tmp.z;

            vertices[v[3]].nx = vertices[v[3]].nx + tmp.x;
            vertices[v[3]].ny = vertices[v[3]].ny + tmp.y;
            vertices[v[3]].nz = vertices[v[3]].nz + tmp.z;

            vertices[v[0]].nx = vertices[v[0]].nx + tmp.x;
            vertices[v[0]].ny = vertices[v[0]].ny + tmp.y;
            vertices[v[0]].nz = vertices[v[0]].nz + tmp.z;
        }
    }

    /*
    // Do we have to normalize normals ?
    for (int i = 0; i < nvertices_; i++)
        vertices[i].normalize();
    */

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (texture_)
        texture_->update();
}


// ______________________________________________________________________ render
void
Surface::render (void)
{
    Object::compute_visibility();
    if (!get_visible())
        return;

    if (not vertex_name_) {
        setup();
        if (not vertex_name_) {
            return;
        }
        update();
    }

    glPushAttrib (GL_ENABLE_BIT);
    glDisable (GL_TEXTURE_2D);
    glEnable (GL_BLEND);

    glPushMatrix();
    glTranslatef (get_position().x, get_position().y, get_position().z);
    glScalef (get_size().x, get_size().y, get_size().z);
    //glScalef (1.0f, 1.0f, get_size().z);
    //    printf("%f %f %f\n", get_size().x, get_size().y, get_size().z);
    
    // Actual surface
    // -------------------------------------------------------------------------
    glColor4f (get_bg_color().r,
               get_bg_color().g,
               get_bg_color().b,
               get_bg_color().a*alpha_);
    glPolygonOffset (1.0f, 1.0f);
    glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_name_);
    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);
    /*
    if (texture_ and texture_->get_texture()) {
        glEnableClientState (GL_TEXTURE_COORD_ARRAY);
        glBindTexture (GL_TEXTURE_2D, texture_->get_texture());
        glEnable (GL_TEXTURE_2D);
    } else {
        glDisable (GL_TEXTURE_2D);        
    }
    */
    if (cdata_) {
        glEnableClientState (GL_COLOR_ARRAY);
        glColorPointer (4, GL_FLOAT, sizeof(Vertex), (void *)(8*sizeof(float)));
        glDisable (GL_TEXTURE_2D);
    }
    glVertexPointer (3, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer (2, GL_FLOAT, sizeof(Vertex), (void *)(3*sizeof(float)));
    glNormalPointer (GL_FLOAT, sizeof(Vertex), (void *)(5*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface_index_name_);
    glDrawElements(GL_QUADS, surface_nindices_, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableClientState (GL_TEXTURE_COORD_ARRAY);
    glDisableClientState (GL_NORMAL_ARRAY);
    glDisableClientState (GL_COLOR_ARRAY);
    // -------------------------------------------------------------------------


    // Surface grid
    // -------------------------------------------------------------------------
    glDisable (GL_TEXTURE_2D);
    glDepthMask (GL_FALSE);
    glEnable (GL_BLEND);
    glEnable (GL_LINE_SMOOTH);
    glDisable (GL_LIGHTING);
    glLineWidth (0.5f);
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    glDisable (GL_POLYGON_OFFSET_FILL);
    glColor4fv (get_fg_color().data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid_index_name_);
    glDrawElements(GL_QUADS, grid_nindices_, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // -------------------------------------------------------------------------


    // Border
    // -------------------------------------------------------------------------
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    glColor4fv (get_br_color().data);
    glLineWidth (1.5f);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_index_name_);
    glDrawElements(GL_LINE_LOOP, border_nindices_, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // -------------------------------------------------------------------------

    glDisableClientState (GL_VERTEX_ARRAY);
    glDepthMask (GL_TRUE);
    glPopMatrix();
    glPopAttrib ();
}



// __________________________________________________________________ set_z_data
void
Surface::set_z_data (DataPtr zdata)
{
    if (not zdata) {
        throw std::invalid_argument
            ("Z data must be valid data");
    }
    if ((not (zdata->get_type() == GL_DOUBLE))) {
        throw std::invalid_argument
            ("Z data must be of type GL_DOUBLE");
    }
    if ((not (zdata->get_width() > 1)) or
        (not (zdata->get_height() > 1)) or
        (not (zdata->get_depth() == 1))) {
        throw std::invalid_argument
            ("Z data shape must be of type (n,n,1) with n > 1");
    }
    reset_data();
    zdata_ = zdata;
}


// ________________________________________________________________ set_z_c_data
void
Surface::set_z_c_data (DataPtr zdata, DataPtr cdata)
{
    if ((not zdata) or (not cdata)) {
        throw std::invalid_argument
            ("Z,C data must be valid data");
    }
    if ((not (zdata->get_type() == GL_DOUBLE)) or
        (not (zdata->get_type() == GL_DOUBLE))) {
        throw std::invalid_argument
            ("Z,C data must be of type GL_DOUBLE");
    }
    if ((not (zdata->get_width() > 1)) or
        (not (zdata->get_height() > 1)) or
        (not (zdata->get_depth() == 1))) {
        throw std::invalid_argument
            ("Z data shape must be of type (n,n) with n > 1");
    }
    if ((not (zdata->get_width() == cdata->get_width())) or
        (not (zdata->get_height()== cdata->get_height()))) {
        throw std::invalid_argument
            ("Z,C data must be of same shape");
    }
    if ((not (cdata->get_depth() == 1)) and
        (not (cdata->get_depth() == 3)) and
        (not (cdata->get_depth() == 4))) {
        throw std::invalid_argument
            ("C data must have 1 (cmap value), 3 (rgb) or 4 (rgba) components");
    }
    reset_data();
    zdata_ = zdata;
    cdata_ = cdata;
}


// __________________________________________________________________ reset_data
void
Surface::reset_data (void)
{
#if defined(HAVE_BOOST)
    xdata_ = DataPtr();
    ydata_ = DataPtr();
    zdata_ = DataPtr();
    cdata_ = DataPtr();
    texture_ = TexturePtr();
#else
    xdata_ = 0;
    ydata_ = 0;
    zdata_ = 0;
    cdata_ = 0;
    texture_ = 0;
#endif
    if (vertex_name_)
        glDeleteBuffers(1, &vertex_name_);
    if (surface_index_name_)
        glDeleteBuffers(1, &surface_index_name_);
    if (grid_index_name_)
        glDeleteBuffers(1, &grid_index_name_);
    if (border_index_name_)
        glDeleteBuffers(1, &border_index_name_);
	nvertices_          = 0;
 	surface_nindices_   = 0;
 	grid_nindices_      = 0;
 	border_nindices_    = 0;
    vertex_name_        = 0;
    surface_index_name_ = 0;
    grid_index_name_    = 0;
    border_index_name_  = 0;
}


// ________________________________________________________________ get_colormap
ColormapPtr
Surface::get_colormap (void)
{
    return colormap_;
}


// ________________________________________________________________ set_colormap
void
Surface::set_colormap (ColormapPtr colormap)
{
    colormap_ = colormap;
}
