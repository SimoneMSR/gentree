/* -*- coding: utf-8 -*- */
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
#include <iostream>
#include <sstream>
#include "font.h"
#include "colorbar.h"

Colorbar::Colorbar (void) : Widget ()
{
    cmap_ = Colormap::Hot();
    texture_ = 0;
    set_margin (0,0,0,0);
    set_radius (0);
    set_size (.75, 24);
    set_position (0,75,1);
    set_gravity (0,1);
    set_fg_color (0,0,0,1);
    set_bg_color (0,0,0,.25);
    set_br_color (0,0,0,1);
    title_ = cmap_->get_name();
    std::ostringstream oss;
    oss << "Colorbar_" << id_;
    set_name (oss.str());
}

Colorbar::~Colorbar (void)
{
    if (texture_)
        glDeleteTextures (1, &texture_);
}

void
Colorbar::setup (void)
{
    unsigned char *data = new unsigned char [512*3];
    float min = cmap_->get_min();
    float max = cmap_->get_max();

    for (int i=0; i<512; i++) {
        Color c = (*cmap_)(min+ (i/512.0f)*(max-min));
        data[i*3+0] = int(c.red*255);
        data[i*3+1] = int(c.green*255);
        data[i*3+2] = int(c.blue*255);
    }
    if (texture_)
        glDeleteTextures (1, &texture_);
    glGenTextures (1, &texture_);
    glBindTexture (GL_TEXTURE_2D, texture_);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, 512, 1, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, data);
    delete data;
}

void
Colorbar::render (void)
{
    // -------------------------------------------------------------------------
    // p = position
    // s = size
    //
    //     p.x                       TITLE
    //  p.y  +-------------------------------------------+  
    //       |                                           |  s.y
    //       +----------+----------+----------+----------+     
    //     +0.00      +0.25      +0.50      +0.75      +1.00
    //
    //       -------------------- s.x --------------------
    //
    // -------------------------------------------------------------------------
    // Note:
    // 
    // Since a colorbar is a widget, we know the exact rendering pixel size, so
    // we can adapt ticks numbers and tick labels size to fit the overall 
    // available space. This means computing various things (see below) for
    // vertical or horizontal orientation.
    //
    // Orientation is computed automatically by looking at the biggest widget
    // size along x or y.
    //
    // Colorbar itself is using a texture to make things a bit faster.
    // -------------------------------------------------------------------------

    if (not get_visible())
        return;
    if (not texture_)
        setup ();
    if (not texture_)
        return;
    render_start();

    GLint viewport[4]; 
    glGetIntegerv (GL_VIEWPORT, viewport);
    float height = viewport[3];

    int ticks_number = 11;  // Maximum number of ticks
    float tick_size = .25; // Ticks size as percentage of small axis
    std::vector<float> ticks_value;
    std::vector<std::string> ticks_label;
    Font *font;
    Position p = Position (get_position().x, height-get_position().y);
    Size     s = Size     (get_size().x, get_size().y);


    // Step 1: decide if colorbar orientation is horizontal (0) or vertical (1)
    int orientation = 0;
    if (get_size().x < get_size().y)
        orientation = 1;

    // Step 2: Find number of ticks
    for (int i=0; i<ticks_number; i++) {
        ticks_value.push_back
            (cmap_->get_max() - (i/float(ticks_number-1))*(cmap_->get_max()-cmap_->get_min()));
    }
    for (int i=0; i<ticks_number; i++) {
        std::ostringstream oss;
        oss.setf(std::ios::fixed, std::ios::floatfield);
        oss.precision (2);
        oss << ticks_value[i];
        ticks_label.push_back (oss.str());
    }
    // For horizontal orientation, we want to have at least 4 characters
    // separating each tick label (for readability).
    int c = 0, c_max = 0;
    if (orientation == 0) {
        // Count total number of characters in c (with 3 spaces between labels)
        // Since first and last label are centered on border, we take their
        // mean size.
        c = (ticks_label[0].size() + ticks_label[ticks_label.size()-1].size())/2 + 3;
        c_max = ticks_label[0].size();
        if (int(ticks_label[ticks_label.size()-1].size()) > c_max)
            c_max = ticks_label[ticks_label.size()-1].size();
        for (int i=1; i<(ticks_number-1); i++) {
            c += ticks_label[i].size() + 3;
            if (int(ticks_label[i].size()) > c_max)
                c_max = ticks_label[i].size();
        }
        int c12 = Font::Font12()->get_glyph_size().x * c;
        int c16 = Font::Font16()->get_glyph_size().x * c;
        int c24 = Font::Font24()->get_glyph_size().x * c;
        int c32 = Font::Font32()->get_glyph_size().x * c;
        // Does all tick labels fit at 12px size ?
        font = Font::Font12();
        if (c12 < get_size().x) {
            // at 16px size ?
            if (c16 < get_size().x)
                font = Font::Font16();
            // at 24px size ?
            if (c24 < get_size().x)
                font = Font::Font24();
            // at 32px size ?
            if (c32 < get_size().x)
                font = Font::Font32();
        } else {
            // Ok, Size 12px does not fit, how many character do we dispose ?
            c = get_size().x / Font::Font12()->get_glyph_size().x;
            ticks_number = c / (c_max+2); // max number of labels that fit

            // We want at least 2 ticks even if it does not fit
            if (ticks_number < 2)
                ticks_number = 2;
        }
    }
    // For vertical orientation, we want to have at least .5 character
    // separating each tick label line (for readability).
    else if (orientation == 1) {
        int c12 = Font::Font12()->get_glyph_size().y * 20;
        int c16 = Font::Font16()->get_glyph_size().y * 20;
        int c24 = Font::Font24()->get_glyph_size().y * 20;
        int c32 = Font::Font32()->get_glyph_size().y * 20;
        font = Font::Font12();
        if (c12 < get_size().y) {
            // at 16px size ?
            if (c16 < get_size().y)
                font = Font::Font16();
            // at 24px size ?
            if (c24 < get_size().y)
                font = Font::Font24();
            // at 32px size ?
            if (c32 < get_size().y)
                font = Font::Font32();
        } else {
            int c = get_size().y / Font::Font12()->get_glyph_size().y;
            ticks_number = c / 2;
            // We want at least 2 ticks even if it does not fit
            if (ticks_number < 2)
                ticks_number = 2;
        }
    }
    int fx = font->get_glyph_size().x;
    int fy = font->get_glyph_size().y;

    // Recompute ticks value & label
    ticks_value.clear();
    for (int i=0; i<ticks_number; i++) {
        ticks_value.push_back
            (cmap_->get_max() - (i/float(ticks_number-1))*(cmap_->get_max()-cmap_->get_min()));
    }
    ticks_label.clear();
    for (int i=0; i<ticks_number; i++) {
        std::ostringstream oss;
        oss.setf(std::ios::fixed, std::ios::floatfield);
        oss.precision (2);
        oss << ticks_value[i];
        ticks_label.push_back (oss.str());
    }
    
    // Step 3: Colorbar texture
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, texture_);
    glColor4f (1,1,1,alpha_);
    glBegin (GL_POLYGON);
    if (orientation == 1) {
        glTexCoord2f (1, 0);  glVertex2f (p.x,     p.y);
        glTexCoord2f (1, 0);  glVertex2f (p.x+s.x, p.y);
        glTexCoord2f (0, 0);  glVertex2f (p.x+s.x, p.y-s.y);
        glTexCoord2f (0, 0);  glVertex2f (p.x,     p.y-s.y);
    } else {
        glTexCoord2f (0, 0);  glVertex2f (p.x,     p.y);
        glTexCoord2f (1, 0);  glVertex2f (p.x+s.x, p.y);
        glTexCoord2f (1, 0);  glVertex2f (p.x+s.x, p.y-s.y);
        glTexCoord2f (0, 0);  glVertex2f (p.x,     p.y-s.y);
    }
    glEnd();
    glDisable (GL_TEXTURE_2D);

    // Step 4: Border
    glTranslatef (.375, .375, 1);
    glDepthMask (GL_FALSE);
    glColor4f (get_br_color().r,
               get_br_color().g,
               get_br_color().b,
               get_br_color().a*alpha_);
    glEnable (GL_LINE_SMOOTH);
    glBegin (GL_LINE_LOOP);
    glVertex2f (p.x,     p.y);
    glVertex2f (p.x+s.x, p.y);
    glVertex2f (p.x+s.x, p.y-s.y);
    glVertex2f (p.x,     p.y-s.y);
    glEnd();

    // Step 5: Ticks
    glColor4f (get_fg_color().r,
               get_fg_color().g,
               get_fg_color().b,
               get_fg_color().a*alpha_);
    glLineWidth (1);
    glBegin(GL_LINES);
    if (orientation == 1) {
        for (int i=1; i<(ticks_number-1); i++) {
            glVertex3i (p.x+s.x*(1.0-tick_size),
                        p.y-s.y*i/float(ticks_number-1),
                        0);
            glVertex3i (p.x+s.x ,
                        p.y-s.y*i/float(ticks_number-1),
                        0);
        }
    } else {
        for (int i=1; i<(ticks_number-1); i++) {
            glVertex3i (p.x+ i/float(ticks_number-1)*s.x,
                        p.y-s.y,
                        0);
            glVertex3i (p.x+ i/float(ticks_number-1)*s.x,
                        p.y-s.y + tick_size*s.y,
                        0);
        } 
    }
    glEnd();
    glTranslatef (-.375,-.375, 1);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glEnable (GL_TEXTURE_2D);
    glDepthMask (GL_TRUE);
    

    // Step 6: Ticks label
    if (orientation == 1) {
        for (int i=0; i<ticks_number; i++) {
            glPushMatrix();
            glTranslatef (int(p.x + s.x + fx),
                          int(p.y + fy/2 -s.y*i/float(ticks_number-1)),
                          0);
            font->render(ticks_label[i]);
            glPopMatrix();
        }
    } else {
        for (int i=0; i<ticks_number; i++) {
            int l = ticks_label[i].size()*fx;
            glPushMatrix();
            glTranslatef (int(p.x+s.x*i/float(ticks_number-1) -l/2),
                          int(p.y-s.y-.5*fy),
                          0);
            font->render(ticks_label[ticks_number-i-1]);
            glPopMatrix();
        }
    }

    // Step 7: Title
    glPushMatrix();
    if (orientation == 1) {
        int l = title_.size()*fx;
        glRotatef (90, 0,0,1);
        glTranslatef (int (p.y - s.y/2 -l/2),
                      int (-p.x +fy),
                      0);
    } else {
        int l = title_.size()*fx;
        glTranslatef (int (p.x + s.x/2 - l/2),
                      int (p.y + 3*fy/2),
                      0);
    }
    font->render (title_);
    glPopMatrix();

    render_finish();
}

void
Colorbar::set_title (std::string title)
{
    title_ = title;
}

std::string
Colorbar::get_title (void)
{
    return title_;
}

std::string
Colorbar::title (void)
{
    return get_title();
}

void
Colorbar::set_cmap (Colormap *cmap)
{
    cmap_ = cmap;
}

Colormap *
Colorbar::get_cmap (void)
{
    return cmap_;
}

Colormap *
Colorbar::cmap (void)
{
    return get_cmap();
}
