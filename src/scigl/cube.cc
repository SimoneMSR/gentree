/*
 * Copyright (C) 2008 Nicolas P. Rougier
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.

 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "cube.h"

Cube::Cube (void) : Object()
{
    set_visible (true);
    set_size (1,1,1);
    set_position (0,0,0);
    set_fg_color (0,0,0,1);
    set_br_color (0,0,0,1);
    set_bg_color (1,1,1,.5);
    alpha_ = 1.0f;
    fade_in_delay_ = 0;
    fade_out_delay_ = 0;

    std::ostringstream oss;
    oss << "Cube_" << id_;
    set_name (oss.str());
}

Cube::~Cube (void)
{}

void
Cube::render (void)
{
    compute_visibility();
    if (not get_visible()) return;

    GLint mode;
    glGetIntegerv (GL_RENDER_MODE, &mode);

    struct face {
        float vertices[4*3];
        float normal[3];
        float z;
        static int compare (const void *a, const void *b) {
            const face *da = (const face *) a;
            const face *db = (const face *) b;
            return ((*da).z > (*db).z) - ((*da).z < (*db).z);
        }
    };

    face faces[6] = {
        {{-1,-1, 1,  1,-1, 1,  1, 1, 1, -1, 1, 1}, { 0, 0, 1}, 0},
        {{-1,-1,-1, -1, 1,-1,  1, 1,-1,  1,-1,-1}, { 0, 0,-1}, 0},
        {{-1, 1,-1, -1, 1, 1,  1, 1, 1,  1, 1,-1}, { 0, 1, 0}, 0},
        {{-1,-1,-1,  1,-1,-1,  1,-1, 1, -1,-1, 1}, { 0,-1, 0}, 0},
        {{ 1,-1,-1,  1, 1,-1,  1, 1, 1,  1,-1, 1}, { 1, 0, 0}, 0},
        {{-1,-1,-1, -1,-1, 1, -1, 1, 1, -1, 1,-1}, {-1, 0, 0}, 0}
    };

    /* Cube faces are z sorted for correct translucency */
    /* z sorting is done on faces center */
	float m[16];
	glGetFloatv (GL_MODELVIEW_MATRIX, m);
    for (int i=0; i<6; i++)
        faces[i].z = faces[i].normal[0]*m[2]
                  +  faces[i].normal[1]*m[6]
                  +  faces[i].normal[2]*m[10]
                  + m[14]; 
    qsort (faces, 6, sizeof (face), face::compare);
    
    glPushAttrib (GL_ENABLE_BIT);
    glDisable (GL_TEXTURE_2D);
    glDisable (GL_LIGHTING);
    glEnable (GL_BLEND);
    //glEnable (GL_DEPTH_TEST);
    glEnable (GL_LINE_SMOOTH);


    if (mode == GL_SELECT) {
        glLoadName (id_);
    }

    glPushMatrix();
    glScalef (get_size().x, get_size().y, get_size().z);
    glTranslatef (get_position().x, get_position().y, get_position().z);

    for (int i=0; i<6; i++) {
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        glColor4fv (get_bg_color().data);
        glPolygonOffset (1, 1);
        glEnable (GL_POLYGON_OFFSET_FILL);
        glBegin (GL_QUADS); {
            glNormal3f (-faces[i].normal[0], -faces[i].normal[1], -faces[i].normal[2]);
            glVertex3f (faces[i].vertices[0], faces[i].vertices[1], faces[i].vertices[2]);
            glVertex3f (faces[i].vertices[3], faces[i].vertices[4], faces[i].vertices[5]);
            glVertex3f (faces[i].vertices[6], faces[i].vertices[7], faces[i].vertices[8]);
            glVertex3f (faces[i].vertices[9], faces[i].vertices[10], faces[i].vertices[11]);
        } glEnd();

        glColor4fv (get_br_color().data);
        glDisable (GL_POLYGON_OFFSET_FILL);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth (0.5);
        glDepthMask (GL_FALSE);
        glBegin (GL_QUADS);
            glNormal3f (-faces[i].normal[0], -faces[i].normal[1], -faces[i].normal[2]);
            glVertex3f (faces[i].vertices[0], faces[i].vertices[1], faces[i].vertices[2]);
            glVertex3f (faces[i].vertices[3], faces[i].vertices[4], faces[i].vertices[5]);
            glVertex3f (faces[i].vertices[6], faces[i].vertices[7], faces[i].vertices[8]);
            glVertex3f (faces[i].vertices[9], faces[i].vertices[10], faces[i].vertices[11]);
        glEnd();
        glDepthMask (GL_TRUE);
    }
    glPopMatrix();
    if (mode == GL_SELECT) {
        glLoadName (0);
    }
    glPopAttrib();
}
