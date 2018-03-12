/*
 * Copyright (C) 2008 Nicolas P. Rougier
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either  version 3 of the  License, or (at your  option) any later
 * version.
 *
 * This program is  distributed in the hope that it will  be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR  A  PARTICULAR PURPOSE.  See  the GNU  General  Public  License for  more
 * details.
 *
 * You should have received a copy  of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <sstream>
#include "font.h"
#include "axis.h"


// ________________________________________________________________________ Axis
Axis::Axis (void) : Object()
{
    set_title ("X Axis");
    set_visible (true);
    set_fg_color (0,0,0,1);
    set_bg_color (1,1,1,.5);
    set_br_color (0,0,0,1);
    set_alpha(1.0f);
    set_start (Position (-0.5f, 0.0f, 0.0f));
    set_end   (Position (+0.5f, 0.0f, 0.0f));
    set_thickness(2.0f);
    set_orientation (0);
    set_ticks_fontsize (0.002);
    set_title_fontsize (0.002);
    set (-0.5,0.5);

    std::ostringstream oss;
    oss << "Axis_" << id_;
    set_name (oss.str());
}

// _______________________________________________________________________ ~Axis
Axis::~Axis (void)
{}

// ______________________________________________________________________ render
void
Axis::render (void)
{
    Object::compute_visibility();
    if (!get_visible())   return;

    float vx = end_.x - start_.x;
    float vy = end_.y - start_.y;
    float vz = end_.z - start_.z;
    if(vx == 0) vx = .00000001;
    float v = sqrt(vx*vx+vy*vy+vz*vz);
    float az = 57.2957795*acos(vx/v);
    if (vx < 0.0) az = -az;
    float rz = vx*vy;
    float ry = -vx*vz;

    glLineWidth (thickness_);
    glDisable (GL_TEXTURE_2D);
    glEnable (GL_LINE_SMOOTH);
    glEnable (GL_BLEND);
    
    /*
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3fv(start_.data);
    glColor3f(0,0,1);
    glVertex3fv(end_.data);
    glEnd();
    */
    
    glColor4f (get_fg_color().r,
               get_fg_color().g,
               get_fg_color().b,
               get_fg_color().a*get_alpha());
    glPushMatrix();
    glTranslatef(start_.x, start_.y, start_.z);
    glRotatef (az, 0, ry, rz);
    glRotatef (orientation_, 1, 0, 0); // Set ticks & labels orientation

    // Main axis
    // -------------------------------------------------------------------------
    glBegin(GL_LINES);
    glVertex3f (0,0,0);
    glVertex3f (v,0,0);
    glEnd();

    // Ticks
    // -------------------------------------------------------------------------
    float mts = 0;
    for (int i=0; i<int(ticks_.size()); i++) {
        Tick tick = ticks_[i];
        glLineWidth (tick.thickness);
        glBegin(GL_LINES);
        glVertex3f (tick.x*v, 0.0f,      0.0f);
        glVertex3f (tick.x*v, tick.size, 0.0f);
        glEnd();
        if (tick.size > mts)
            mts = tick.size;
    }

    // Tick labels
    // -------------------------------------------------------------------------
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glEnable (GL_TEXTURE_2D);
    FontPtr font = FontPtr (Font::Font32());
    float scale = ticks_fontsize_;
    glPushMatrix();
    glScalef (-scale,-scale,scale);
    glTranslatef (0,-mts/scale, 0);
    for (int i=0; i<int(tick_labels_.size()); i++) {
        std::string label = tick_labels_[i];
        if (label.size()) {
            float x = ticks_[i].x;
            float l = (label.size()) * font->get_glyph_size().x;
            glPushMatrix();
            glTranslatef (-x*v/scale-l/2, 0.0f, 0.0f);
            font->render (label);
            glPopMatrix();
        }
    }
        
    // Title
    // -------------------------------------------------------------------------
    glTranslatef (-.5*v/scale, -font->get_glyph_size().y, 0);
    glScalef (title_fontsize_/scale,title_fontsize_/scale,title_fontsize_/scale);
    glPushMatrix();
    font->render_ansi_string (title_, get_alpha(), 0);
    glPopMatrix();
    glPopMatrix();

    glDisable (GL_TEXTURE_2D);
    glPopMatrix();
}

// _________________________________________________________________________ set
void
Axis::set (float min, float max, int major_n, int minor_n,
           float major_size, float minor_size, 
           float major_thickness, float minor_thickness)
{
    ticks_.clear();
    tick_labels_.clear();

    // Major ticks first
    for (int i=0; i<major_n; i++) {
        float x = i/float(major_n-1);
        ticks_.push_back (Tick(x, major_size, major_thickness));
        std::ostringstream oss;
        oss.setf(std::ios::fixed, std::ios::floatfield);
        oss.precision (2);
        oss << min + (max-min)*(i/float(major_n-1));
        tick_labels_.push_back (oss.str());
    }

    // Minor ticks, removing those corresponding to a major one
    for (int i=0; i<minor_n; i++) {
        float x = i/float(minor_n-1);
        bool addup = true;
        for (int j=0; j<int(ticks_.size()); j++) {
            if (ticks_[i].x == x)
                addup = false;
        }
        if (addup) {
            ticks_.push_back (Tick(x, minor_size, minor_thickness));
            tick_labels_.push_back ("");
        }
    }
}

// _______________________________________________________________________ clear
void
Axis::clear (void)
{
    ticks_.clear();
    tick_labels_.clear();    
}

// ____________________________________________________________________ add_tick
void
Axis::add (Tick tick, std::string label)
{
    ticks_.push_back (tick);
    tick_labels_.push_back (label);
}

// ___________________________________________________________________ set_title
void
Axis::set_title (std::string title)
{
    title_ = title;
}

// ___________________________________________________________________ get_title
std::string
Axis::get_title (void) const
{
    return title_;
}

// ___________________________________________________________________ set_start
void
Axis::set_start (Position start)
{
    start_ = Position (start);
}

// ___________________________________________________________________ set_start
void
Axis::set_start (float x, float y, float z, float w)
{
    set_start (Position (x,y,z,w));
}

// ___________________________________________________________________ get_start
Position
Axis::get_start (void) const
{
    return start_;
}

// _____________________________________________________________________ set_end
void
Axis::set_end (Position end)
{
    end_ = Position (end);
}

// _____________________________________________________________________ set_end
void
Axis::set_end (float x, float y, float z, float w)
{
    set_end (Position (x,y,z,w));
}

// _____________________________________________________________________ get_end
Position
Axis::get_end (void) const
{
    return end_;
}

// __________________________________________________________ get_ticks_fontsize
float
Axis::get_ticks_fontsize (void) const
{
    return ticks_fontsize_;
}

// ________________________________________________________________ set_fontsize
void
Axis::set_ticks_fontsize (float size)
{
    ticks_fontsize_ = size;
}

// __________________________________________________________ get_title_fontsize
float
Axis::get_title_fontsize (void) const
{
    return title_fontsize_;
}

// __________________________________________________________ set_title_fontsize
void
Axis::set_title_fontsize (float size)
{
    title_fontsize_ = size;
}

// _______________________________________________________________ get_thickness
float
Axis::get_thickness (void) const
{
    return thickness_;
}

// _______________________________________________________________ set_thickness
void
Axis::set_thickness (float thickness)
{
    thickness_ = thickness;
}

// _____________________________________________________________ get_orientation
float
Axis::get_orientation (void) const
{
    return orientation_;
}

// _____________________________________________________________ set_orientation
void
Axis::set_orientation (float orientation)
{
    orientation_ = orientation;
}
