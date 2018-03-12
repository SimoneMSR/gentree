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
#include "widget.h"

Size Widget::min_size_ = Size (20, 20);
Size Widget::max_size_ = Size (4096, 4096);

// ______________________________________________________________________ Widget
Widget::Widget (void) : Object()
{
    set_focusable (true);
    set_visible   (true);
    set_sizeable  (true);
    set_moveable  (true);
    set_margin    (5,5,5,5);
    set_radius    (0);
    set_size      (1,1);
    set_position  (0,0,1);
    set_gravity   (1,1);
    set_fg_color  (0,0,0,1);
    set_bg_color  (1,1,1,1);
    set_br_color  (0,0,0,0);
    fade_in_delay_ = 0;
    fade_out_delay_ = 0;
    action_ = "";

    std::ostringstream oss;
    oss << "Widget_" << id_;
    set_name (oss.str());
}


// _____________________________________________________________________ ~Widget
Widget::~Widget (void)
{}


// ________________________________________________________________ compute_size
void
Widget::compute_size (void)
{
    GLint viewport[4]; 
    glGetIntegerv (GL_VIEWPORT, viewport);
    float width  = viewport[2];
    float height = viewport[3];

    if (size_request_.x < 0) {
        if (size_request_.x > -1.0f) {
            size_.x = (1.0 + size_request_.x)*width;
        } else {
            size_.x = width + size_request_.x;
        }
    } else if (size_request_.x > 0) {
        if (size_request_.x <= 1.0f) {
            size_.x = size_request_.x*width;
        } else {
            size_.x = size_request_.x;
        }
    } else {
        size_.x = width;
    }
    if (size_request_.y < 0) {
        if (size_request_.y > -1.0f) {
            size_.y = (1.0 + size_request_.y)*height;
        } else {
            size_.y = height + size_request_.y;
        }
    } else if (size_request_.y > 0) {
        if (size_request_.y <= 1.0f) {
            size_.y = size_request_.y*height;
        } else {
            size_.y = size_request_.y;
        }
    } else {
        size_.y = height;
    }
    size_.x = int(size_.x);
    size_.y = int(size_.y);
    if (size_.x < min_size_.x) size_.x = min_size_.x;
    if (size_.y < min_size_.y) size_.y = min_size_.y;
    if (size_.x > max_size_.x) size_.x = max_size_.x;
    if (size_.y > max_size_.y) size_.y = max_size_.y;
}


// ____________________________________________________________ compute_position
void
Widget::compute_position (void)
{
    GLint viewport[4]; 
    glGetIntegerv (GL_VIEWPORT, viewport);
    float width  = viewport[2];
    float height = viewport[3];

    // Left
    if (gravity_.x == 1)
        // Absolute
        if ((position_request_.x >= 1.0f) or (position_request_.x <= -1.0f))
            position_.x = position_request_.x;
        // Relative
        else
            position_.x = position_request_.x*width;
    // Center
    else if (gravity_.x == 0)
        // Absolute
        if ((position_request_.x >= 1.0f) or (position_request_.x <= -1.0f))
            position_.x = width/2 - size_.x/2 - position_request_.x ;
        // Relative
        else
            position_.x = width/2 - size_.x/2 - position_request_.x*width;
    // Right
    else
        // Absolute
        if ((position_request_.x >= 1.0f) or (position_request_.x <= -1.0f))
            position_.x = width - position_request_.x - size_.x;
        // Relative
        else
            position_.x = (1 - position_request_.x)*width - size_.x;

    // Top
    if (gravity_.y == 1)
        // Absolute
        if ((position_request_.y >= 1.0f) or (position_request_.y <= -1.0f))
            position_.y = position_request_.y;
        // Relative
        else
            position_.y = position_request_.y*height;
    // Center
    else if (gravity_.y == 0)
        // Absolute
        if ((position_request_.y >= 1.0f) or (position_request_.y <= -1.0f))
            position_.y = height/2 - size_.y/2 - position_request_.y;
        // Relative
        else
            position_.y = height/2 - size_.y/2 - position_request_.y*height;
    // Bottom
    else
        // Absolute
        if ((position_request_.y >= 1.0f) or (position_request_.y <= -1.0f))
            position_.y = height - position_request_.y - size_.y;
        // Relative
        else
            position_.y = (1 - position_request_.y)*height - size_.y;
    position_.x = int (position_.x);
    position_.y = int (position_.y);
    position_.z = int (position_request_.z);
}


// __________________________________________________________________ arc_circle
void
Widget::arc_circle (float x, float y, float radius, float theta1, float theta2)
{
    const GLfloat delta_theta = 25.0f ;
    if(theta2 > theta1) {
        for(GLfloat theta = theta1; theta <= theta2; theta += delta_theta) {
            GLfloat theta_rad = theta * 3.14159f / 180.0f ;
            glVertex2f(x+radius*cos(theta_rad), y-radius*sin(theta_rad)) ;
        }
    } else {
        for(GLfloat theta = theta1; theta >= theta2; theta -= delta_theta) {
            GLfloat theta_rad = theta * 3.14159f / 180.0f ;
            glVertex2f(x+radius*cos(theta_rad), y-radius*sin(theta_rad)) ;
        }
    }
}


// _____________________________________________________________ round_rectangle
void
Widget::round_rectangle (float x, float y, float w, float h, float radius)
{
    if (radius) {
        glVertex2f  (x,            y-1-radius);
        glVertex2f  (x,            y-h+radius);
        arc_circle  (x+radius,     y-h+radius, radius, -180.0f, -270.0f);
        glVertex2f  (x+radius,     y-h);
        glVertex2f  (x+w-1-radius, y-h);
        arc_circle  (x+w-1-radius, y-h+radius, radius, 90.0f, 0.0f);
        glVertex2f  (x+w-1,        y-h+radius);
        glVertex2f  (x+w-1,        y-1-radius);
        arc_circle  (x+w-1-radius, y-1-radius, radius, 0.0f, -90.0f);
        glVertex2f  (x+w-1-radius, y-1);
        glVertex2f  (x+radius,     y-1);
        arc_circle  (x+radius,     y-1-radius, radius, -90.0f, -180.0f);
    } else {
        glVertex2f (x,     y-1);
        glVertex2f (x+w-1, y-1);
        glVertex2f (x+w-1, y-h);
        glVertex2f (x,     y-h);
    }
}


// ________________________________________________________________ render_start
void
Widget::render_start (void)
{
    compute_visibility();
    compute_size ();
    compute_position ();

    GLint viewport[4]; 
    glGetIntegerv (GL_VIEWPORT, viewport);
    glPushAttrib (GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_SCISSOR_BIT);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
    glOrtho (0, viewport[2], 0, viewport[3], -1000, 1000);
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity();
    glDisable (GL_TEXTURE_2D);
    glDisable (GL_LIGHTING);
    glEnable (GL_BLEND);
    glLineWidth (1.0f);
    glClear (GL_DEPTH_BUFFER_BIT);
}


// _______________________________________________________________ render_finish
void
Widget::render_finish (void)
{
    glMatrixMode (GL_PROJECTION);
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
    glPopMatrix ();
    glPopAttrib ();
}


// ______________________________________________________________________ render
void
Widget::render (void)
{
    GLint viewport[4]; 
    glGetIntegerv (GL_VIEWPORT, viewport);
    float height = viewport[3];
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glPolygonOffset (1, 1);
    glEnable (GL_POLYGON_OFFSET_FILL);
    glEnable (GL_LINE_SMOOTH);
    glLineWidth (1.0f);
    glColor4f (get_bg_color().r,
               get_bg_color().g,
               get_bg_color().b,
               get_bg_color().a*alpha_);
    glBegin (GL_POLYGON);
    round_rectangle (get_position().x, height-get_position().y+1,
                     get_size().x+1, get_size().y+1, radius_);
    glEnd();

    if (get_br_color().a*alpha_) {
        // This translation (half a pixel) is necessary to get nice antialiasing
        glTranslatef (0.315f, 0.315f, 0.0f);
        glTranslatef (0,0,1);
        glColor4f (get_br_color().r,
                   get_br_color().g,
                   get_br_color().b,
                   get_br_color().a*alpha_);
        glDepthMask (GL_FALSE);
        glBegin (GL_LINE_LOOP);
        round_rectangle (get_position().x, height-get_position().y,
                         get_size().x, get_size().y, radius_);
        glEnd ();
        glTranslatef (-0.315f, -0.315f, 1.0f);
        glDepthMask (GL_TRUE);
    }
    glDisable (GL_POLYGON_OFFSET_FILL);
}


// ________________________________________________________________ button_press
bool
Widget::mouse_action_start (std::string action, int x, int y)
{
    if (not get_visible())
        return false;
    if (action != "move-resize") 
        return false;

    float x1 = get_position().x;
    float x2 = x1+get_size().x;
    float y1 = get_position().y;
    float y2 = y1+get_size().y;
    float limit = 10;
    action_ = "";
    if ((x >= x1) && (x < x2) && (y >= y1) && (y < y2)) {
        if (get_sizeable()) {
            if (x < (x1 + limit)) {
                if (y > (y2 - limit)) {
                    action_ = "resize-bottom-left";
                } else if (y < (y1 + limit)) {
                    action_ = "resize-top-left";
                } else {
                    action_ = "resize-left";
                }
            } else if (x > (x2 - limit)) {
                if (y > (y2 - limit)) {
                    action_ = "resize-bottom-right";
                } else if (y < (y1 + limit)) {
                    action_ = "resize-top-right";
                } else {
                    action_ = "resize-right";
                }
            } else if (y < (y1 + limit)) {
                action_ = "resize-top";
            } else if (y > (y2 - limit)) {
                action_ = "resize-bottom";
            } else if (get_moveable()) {
                action_ = "move";
            }
        } else if (get_moveable()) {
            action_ = "move";
        }
    }
    action_start_.x = x;
    action_start_.y = y;
    saved_position_.x = position_.x;
    saved_position_.y = position_.y;
    saved_size_.x = size_.x;
    saved_size_.y = size_.y;
    if (action_ != "")
        return true;
    return false;
}


// ____________________________________________________________ mouse_action_end
bool
Widget::mouse_action_end (int x, int y)
{
    action_start_.x = 0;
    action_start_.y = 0;
    action_ = "";
    return true;
}


// ________________________________________________________________ mouse_action
bool
Widget::mouse_action (int x, int y)
{
    if (not get_visible())
        return false;

    float dx = x - action_start_.x;
    float dy = y - action_start_.y;

    if (action_ ==  "") {
        return false;
    } else if (action_ ==  "move") {
        gravity_ = Gravity(1,1);
        position_request_.x = saved_position_.x + dx;
        position_request_.y = saved_position_.y + dy;
    } else if (action_ ==  "resize-left") {
        if ((saved_size_.x - dx) > 30) {
            position_request_.x = saved_position_.x + dx;
            size_request_.x = saved_size_.x - dx;
        }
    } else if (action_ ==  "resize-right") {
        if ((saved_size_.x + dx) > 30) {
            size_request_.x = saved_size_.x + dx;
        }
    } else if (action_ ==  "resize-bottom") {
        if ((saved_size_.y + dy) > 30) {
            size_request_.y = saved_size_.y + dy;
        }
    } else if (action_ ==  "resize-top") {
        if ((saved_size_.y - dy) > 30) {
            position_request_.y = saved_position_.y + dy;
            size_request_.y = saved_size_.y - dy;
        }
    } else if (action_ ==  "resize-bottom-left") {
        if ((saved_size_.x - dx) > 30) {
            position_request_.x  = saved_position_.x + dx;
            size_request_.x = saved_size_.x - dx;
        }
        if ((saved_size_.y + dy) > 30) {
            size_request_.y = saved_size_.y + dy;
        }
    } else if (action_ ==  "resize-top-left") {
        if ((saved_size_.x - dx) > 30) {
            position_request_.x  = saved_position_.x + dx;
            size_request_.x = saved_size_.x - dx;
        }
        if ((saved_size_.y - dy) > 30) {
            position_request_.y  = saved_position_.y + dy;
            size_request_.y = saved_size_.y - dy;
        }
    } else if (action_ ==  "resize-bottom-right") {
        if ((saved_size_.x + dx) > 30) {
            size_request_.x = saved_size_.x + dx;
        }
        if ((saved_size_.y + dy) > 30) {
            size_request_.y = saved_size_.y + dy;
        }
    } else if (action_ ==  "resize-top-right") {
        if ((saved_size_.x + dx) > 30) {
            size_request_.x = saved_size_.x + dx;
        }
        if ((saved_size_.y - dy) > 30) {
            position_request_.y  = saved_position_.y + dy;
            size_request_.y = saved_size_.y - dy;
        }
    } else {
        return false;
    };
    return true;
}


// ________________________________________________________________ set_position
void
Widget::set_position (Position position)
{
    position_request_ = Position (position);
}


// ________________________________________________________________ set_position
void
Widget::set_position (float x, float y, float z)
{
    set_position (Position (x,y,z));
}


// ____________________________________________________________________ set_size
void
Widget::set_size (Size size)
{
    size_request_ = Size (size);
}


// ____________________________________________________________________ set_size
void
Widget::set_size (float x, float y, float z)
{
    set_size (Size (x,y,z));
}


// _________________________________________________________________ set_gravity
void
Widget::set_gravity (Gravity gravity)
{
    if (gravity.x == 0)       gravity_.x = 0;
    else if (gravity.x == -1) gravity_.x = -1;
    else                      gravity_.x = 1;
    if (gravity.y == 0)       gravity_.y = 0;
    else if (gravity.y == -1) gravity_.y = -1;
    else                      gravity_.y = 1;
}


// _________________________________________________________________ set_gravity
void
Widget::set_gravity (float x, float y)
{
    set_gravity (Gravity (x,y));
}


// _________________________________________________________________ get_gravity
Gravity
Widget::get_gravity (void)
{
    return gravity_;
}

// __________________________________________________________________ set_radius
void
Widget::set_radius (int radius)
{
    radius_ = radius;
}


// __________________________________________________________________ get_radius
int
Widget::get_radius (void)
{
    return radius_;
}


// __________________________________________________________________ set_margin
void
Widget::set_margin (Margin margin)
{
    margin_ = Margin (margin);
}


// __________________________________________________________________ set_margin
void
Widget::set_margin (float u, float d, float l, float r)
{
   set_margin (Margin (u,d,l,r));
}


// __________________________________________________________________ get_margin
Margin
Widget::get_margin (void)
{
    return margin_;
}


// _______________________________________________________________ set_focusable
void
Widget::set_focusable (bool focusable)
{
    focusable_ = focusable;
}


// _______________________________________________________________ get_focusable
bool
Widget::get_focusable (void)
{
    return focusable_;
}


// ___________________________________________________________________ has_focus
bool
Widget::has_focus (int x, int y)
{
    if (not get_visible()) return false;
    float x1 = get_position().x;
    float x2 = x1+get_size().x;
    float y1 = get_position().y;
    float y2 = y1+get_size().y;
    if ((x >= x1) && (x < x2) && (y >= y1) && (y < y2))
        return true;
    return false;
}


// ________________________________________________________________ set_sizeable
void
Widget::set_sizeable (bool sizeable)
{
    sizeable_ = sizeable;
}


// ________________________________________________________________ get_sizeable
bool
Widget::get_sizeable (void)
{
    return sizeable_;
}


// ________________________________________________________________ set_moveable
void
Widget::set_moveable (bool moveable)
{
    moveable_ = moveable;
}


// ________________________________________________________________ get_moveable
bool
Widget::get_moveable (void)
{
    return moveable_;
}
