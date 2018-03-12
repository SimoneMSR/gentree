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
#include "light.h"

// _______________________________________________________________________ Light
Light::Light (void) : Object()
{
    set_direction (2,2,2);
    set_ambient_color (0.1f, 0.1f, 0.1f, 1.0f);
    set_diffuse_color (1.0f, 1.0f, 1.0f, 1.0f);
    set_specular_color (0.0f, 0.0f, 0.0f, 1.0f);
}


// ______________________________________________________________________ ~Light
Light::~Light (void)
{}



// ________________________________________________________________ set_position
void
Light::set_position (Position position)
{
    Object::set_position (position.x, position.y, position.z, 1.0f);
}


// ________________________________________________________________ set_position
void
Light::set_position (float x, float y, float z)
{
    Object::set_position (x,y,z, 1.0f);
}


// ________________________________________________________________ set_direction
void
Light::set_direction (Position position)
{
    Object::set_position (position.x, position.y, position.z, 0.0f);
}


// ________________________________________________________________ set_direction
void
Light::set_direction (float x, float y, float z)
{
    Object::set_position (x,y,z,0.0f);
}


// ________________________________________________________________ get_direction
Direction
Light::get_direction (void) const
{
    return Object::get_position ();
}


// ___________________________________________________________ set_diffuse_color
void
Light::set_diffuse_color (Color color)
{
    set_fg_color (color);
}


// ___________________________________________________________ set_diffuse_color
void
Light::set_diffuse_color (float r, float g, float b, float a)
{
    set_diffuse_color (Color (r,g,b,a));
}


// ___________________________________________________________ get_diffuse_color
Color
Light::get_diffuse_color (void) const
{
    return get_fg_color();
}


// ___________________________________________________________ set_ambient_color
void
Light::set_ambient_color (Color color)
{
    set_bg_color (color);
}


// ___________________________________________________________ set_ambient_color
void
Light::set_ambient_color (float r, float g, float b, float a)
{
    set_ambient_color (Color (r,g,b,a));
}


// ___________________________________________________________ get_ambient_color
Color
Light::get_ambient_color (void) const
{
    return get_bg_color();
}


// __________________________________________________________ set_specular_color
void
Light::set_specular_color (Color color)
{
    set_br_color (color);
}


// __________________________________________________________ set_specular_color
void
Light::set_specular_color (float r, float g, float b, float a)
{
    set_specular_color (Color (r,g,b,a));
}


// __________________________________________________________ get_specular_color
Color
Light::get_specular_color (void) const
{
    return get_br_color();
}
