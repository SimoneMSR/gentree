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
#include "shapes.h"
#include "my_frame.h"

// _______________________________________________________________________ Frame
Frame::Frame (void) : Object()
{
    set_size      (1, 1);
    set_position  (0,0);
    set_fg_color  (0,0,0,1);
    set_bg_color  (1,1,1,.5);
    set_br_color  (0,0,0,1);

    std::ostringstream oss;
    oss << "Frame_" << id_;
    set_name (oss.str());
}

// ______________________________________________________________________ ~Frame
Frame::~Frame (void)
{}

// ______________________________________________________________________ render
void
Frame::render (void)
{
    Object::compute_visibility ();
    if (!get_visible())  return;

    glPushAttrib (GL_ENABLE_BIT);
    glDisable (GL_LIGHTING);
    glEnable (GL_BLEND);
    glDisable (GL_TEXTURE_2D);
	float m[16];

    struct face {
        int index;
        float vertices[4*3];
        float normal[3];
        float z;
        static int compare (const void *a, const void *b)
        {
            const struct face *da = (const face *) a;
            const struct face *db = (const face *) b;
            return ((*da).z > (*db).z) - ((*da).z < (*db).z);
        }
    } faces[4] = {
        {0, {-.5,-.5,-.5,   .5,-.5,-.5,   .5,-.5, .5,   -.5,-.5, .5}, {0,-1,0}, 0},
        {1, {-.5,-.5,-.5,  -.5, .5,-.5,   .5, .5,-.5,    .5,-.5,-.5}, {0,0,-1}, 0},
        {2, {-.5,-.5,-.5,  -.5,-.5, .5,  -.5, .5, .5,   -.5, .5,-.5}, {-1,0,0}, 0},
        {3, { .5,-.5,-.5,   .5,-1.0,-.5,   .5,-1.0, .5,  .5,-.5, .5}, { 1,0,0}, 0},
    };
	glGetFloatv (GL_MODELVIEW_MATRIX, m);
    for (int i=0; i<4; i++)
        faces[i].z = faces[i].normal[0]*m[2] + faces[i].normal[1]*m[6]
                   + faces[i].normal[2]*m[10] + m[14]; 
    qsort (faces, 4, sizeof (face), face::compare);

    Color fg = get_fg_color();
    fg.alpha *= get_alpha();
    Color bg = get_bg_color();
    bg.alpha *= get_alpha();
    for (int i=0; i<4; i++) {
        // YZ plane
        if (faces[i].index == 0) {
          //  if ((faces[3].index != 3) and (faces[3].index != 0))
            //    z_axis_->render();
            glPushMatrix();
            glRotatef (90, 1, 0, 0);
            glTranslatef (0, 0, .5);
            plane(fg,bg,9,9);
            glPopMatrix();
           // if ((faces[3].index == 3) or (faces[3].index == 0))
             //   z_axis_->render();
        // XY plane
        } else if (faces[i].index == 1) {
            glPushMatrix();
            glTranslatef (0.0f, 0.0f, -0.5f);
            plane(fg,bg,9,9);
            glTranslatef (0.0f, 0.5f, 0.0f);
            glPopMatrix ();
            //x_axis_->render();
            //y_axis_->render();
        // XZ plane
        } else if (faces[i].index == 2) {
            glPushMatrix();
            glRotatef (90, 0, 1, 0);
            glTranslatef (0.0f, 0.0f, -0.5f);
            plane(fg,bg,9,9);
            glPopMatrix();
        } else if (faces[i].index == 3) {
            //z_axis_->render();
        }
    }
    glPopAttrib ();
}

// ________________________________________________________________________ hide
void
Frame::hide (float timeout)
{
    Object::hide (timeout);

}

// ________________________________________________________________________ show
void
Frame::show (float timeout)
{
    Object::show (timeout);

}
