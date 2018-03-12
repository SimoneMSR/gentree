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
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <sys/time.h>
#include "object.h"

// _________________________________________________________________ id_counter_
unsigned long
Object::id_counter_ = 1;

// ______________________________________________________________________ Object
Object::Object (void)
{
    set_size (1,1,1);
    set_position (0,0,0);
    set_fg_color (0,0,0,1);
    set_br_color (0,0,0,1);
    set_bg_color (1,1,1,1);
    set_visible (true);
    set_alpha (1.0f);
    fade_in_delay_ = 0;
    fade_out_delay_ = 0;
    id_ = id_counter_++;
    action_ = "";

    std::ostringstream oss;
    oss << "Object_" << id_;
    set_name (oss.str());
}

// _____________________________________________________________________ ~Object
Object::~Object (void)
{}

// ____________________________________________________________________ get_repr
std::string
Object::get_repr( void )
{
    return ("Object");
}

// __________________________________________________________ compute_visibility
void
Object::compute_visibility (void)
{
    // Update visibility and transparency according to current time
    struct timeval now;

    // Fade out
    if (fade_out_delay_) {
        gettimeofday (&now, NULL);
        float delay = ((now.tv_sec - fade_time_.tv_sec)*1000.0 +
                       (now.tv_usec - fade_time_.tv_usec)/1000.0);
        alpha_ = 1.0f - delay/fade_out_delay_;
        if (alpha_ < 0.0f) {
            fade_out_delay_ = 0;
            visible_ = false;
            alpha_ = 0.0f;
        }

    // Fade in
    } else if (fade_in_delay_) {
        gettimeofday (&now, NULL);
        float delay = ((now.tv_sec - fade_time_.tv_sec)*1000.0 +
                       (now.tv_usec - fade_time_.tv_usec)/1000.0);
        alpha_ = delay/fade_in_delay_;
        if (alpha_ > 1.0f) {
            fade_in_delay_ = 0;
            alpha_ = 1.0f;
        }
    }
}


// _______________________________________________________________________ setup
void
Object::setup (void)
{}


// ______________________________________________________________________ update
void
Object::update (void)
{}


// ______________________________________________________________________ render
void
Object::render (void)
{}


// ______________________________________________________________________ select
bool
Object::select (int x, int y)
{
    return false;
}


// _____________________________________________________________ keyboard_action
bool
Object::keyboard_action (std::string action, std::string key)
{
    return false;
}


// __________________________________________________________ mouse_action_start
bool
Object::mouse_action_start (std::string action, int x, int y)
{
    return false;
}


// ____________________________________________________________ mouse_action_end
bool
Object::mouse_action_end (int x, int y)
{
    action_ = "";
    return false;
}

// _________________________________________________________________ mouse_action
bool
Object::mouse_action (int x, int y)
{
    return false;
}


// ____________________________________________________________________ set_name
void
Object::set_name (std::string name)
{
    name_ = name;
}


// ____________________________________________________________________ get_name
std::string
Object::get_name (void) const
{
    return name_;
}


// ______________________________________________________________________ get_id
unsigned int
Object::get_id (void) const
{
    return id_;
}


// ____________________________________________________________________ check_id
bool
Object::check_id (unsigned int id)
{
    return id == id_;
}


// _________________________________________________________________ set_visible
void
Object::set_visible (bool visible)
{
    visible_ = visible;
}


// _________________________________________________________________ get_visible
bool
Object::get_visible (void) const
{
    return visible_;
}


// ________________________________________________________________________ hide
void
Object::hide (float timeout)
{
    if (timeout) {
        gettimeofday (&fade_time_, NULL);
        fade_out_delay_ = timeout;
        fade_in_delay_ = 0;
        alpha_ = 1.0f;
    } else {
        set_visible (false);
    }
}


// ________________________________________________________________________ show
void
Object::show (float timeout)
{
    set_visible (true);
    alpha_ = 1.0f;
    if (timeout) {
        gettimeofday (&fade_time_, NULL);
        fade_out_delay_ = 0;
        fade_in_delay_ = timeout;
        alpha_ = 0.0f;
    }
}


// ___________________________________________________________________ set_alpha
void
Object::set_alpha (float alpha)
{
    alpha_ = alpha;
}


// ___________________________________________________________________ get_alpha
float
Object::get_alpha (void) const
{
    return alpha_;
}


// ________________________________________________________________ set_position
void
Object::set_position (Position position)
{
    position_ = Position (position);
}


// ________________________________________________________________ set_position
void
Object::set_position (float x, float y, float z, float w)
{
    set_position (Position (x,y,z,w));
}


// ________________________________________________________________ get_position
Position
Object::get_position (void) const
{
    return position_;
}


// ____________________________________________________________________ set_size
void
Object::set_size (Size size)
{
    size_ = Size (size);
}


// ____________________________________________________________________ set_size
void
Object::set_size (float x, float y, float z, float w)
{
    set_size (Size (x,y,z,w));
}


// ____________________________________________________________________ get_size
Size
Object::get_size (void) const
{
    return size_;
}


// ________________________________________________________________ set_fg_color
void
Object::set_fg_color (Color color)
{
    fg_color_ = color;
}


// ________________________________________________________________ set_fg_color
void
Object::set_fg_color (float r, float g, float b, float a)
{
    set_fg_color (Color (r,g,b,a));
}


// ________________________________________________________________ get_fg_color
Color
Object::get_fg_color (void) const
{
    return fg_color_;
}


// ________________________________________________________________ set_bg_color
void
Object::set_bg_color (Color color)
{
    bg_color_ = color;
}


// ________________________________________________________________ set_bg_color
void
Object::set_bg_color (float r, float g, float b, float a)
{
    set_bg_color (Color (r,g,b,a));
}


// ________________________________________________________________ get_bg_color
Color
Object::get_bg_color (void) const
{
    return bg_color_;
}


// ________________________________________________________________ set_br_color
void
Object::set_br_color (Color color)
{
    br_color_ = color;
}


// ________________________________________________________________ set_br_color
void
Object::set_br_color (float r, float g, float b, float a)
{
    set_br_color (Color (r,g,b,a));
}


// ________________________________________________________________ get_br_color
Color
Object::get_br_color (void) const
{
    return br_color_;
}
