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
#include <cmath>
#include "colormap.h"


Colormap::Colormap (void)
{
	samples_.clear();
	colors_.clear();
    values_.clear();
	resolution_ = 512;
	min_ = 0.0f;
	max_ = 1.0f;
}

Colormap::Colormap (const Colormap &other)
{
	colors_.clear();
    values_.clear();
	samples_.clear();
	resolution_ = other.resolution_;
	for (unsigned int i=0; i<other.colors_.size(); i++)
	    colors_.push_back (Color (other.colors_[i]));
	min_ = other.min_;
	max_ = other.max_;
    resample();
}

Colormap::~Colormap (void)
{}

Colormap &
Colormap::operator= (const Colormap &other)
{
    if (this == &other)
        return *this;
	colors_.clear();
    values_.clear();
	samples_.clear();
	resolution_ = other.resolution_;
	for (unsigned int i=0; i<other.colors_.size(); i++)
	    colors_.push_back (Color (other.colors_[i]));
	min_ = other.min_;
	max_ = other.max_;
    resample();
    return *this;
}

void
Colormap::clear (void)
{
	samples_.clear();
	colors_.clear();
}

void
Colormap::append (float value, Color color)
{
    bool inserted = false;

    if ((value < 0.0f) || (value > 1.0f))
        return;

    for (int i=0; i<int(values_.size()-1); i++) {
        if (value == values_[i]) {
            values_[i] = value;
            colors_[i] = color;
            inserted = true;
            break;
        } else if ((value > values_[i]) and (value < values_[i+1])) {
            values_.insert (values_.begin()+i, value);
            colors_.insert (colors_.begin()+i, color);
            inserted = true;
            break;
        }
    }
    if (not inserted) {
        values_.push_back (value);
        colors_.push_back (color);
    }
    resample();
}

Color
Colormap::operator() (const float value)
{
    float v = (value-min_)/(max_-min_);
    if (samples_.empty())
        return Color (1,1,1,1);
    if (v <= 0.0f)
        return samples_[0];
    else if (v >= 1.0f)
        return samples_[samples_.size()-1];
    int index = int (v*(samples_.size()-1));
    return samples_.at(index);
}

Color
Colormap::color (float value)
{
    value = (value-min_)/(max_-min_);
	if (colors_.empty())
        return Color(1,1,1,1);
	Color inf_color = colors_[0];
	Color sup_color = colors_[colors_.size()-1];
	if (value <= 0.0f)
		return colors_[0];
	else if (value >= 1.0f)
		return colors_[colors_.size()-1];
	else if (colors_.size () == 1)
		return colors_[0];
    for (int i=0; i < int(values_.size()-1); i++) {
        if ((value >= values_[i]) and (value <= values_[i+1])) {
            inf_color = colors_[i];
            sup_color = colors_[i+1];
            float r = fabs ((value-values_[i])/(values_[i+1]-values_[i]));
            float a = (sup_color.a + inf_color.a)/2.0;
            Color color = sup_color*r + inf_color*(1-r);
            color.a = a;
            return color;
        }
    }
	return Color (1,1,1,1);
}

void
Colormap::set_name (std::string name)
{
    name_ = name;
}

std::string
Colormap::get_name (void) const
{
    return name_;
}

void
Colormap::scale (float min, float max)
{
    if (max_ > min_) {
        min_ = min;
        max_ = max;
    }
    resample();
}

void
Colormap::set_min (float value)
{
    if (max_ > value)
        min_ = value;
    resample();
}

float
Colormap::get_min (void)
{
    return min_;
}

void
Colormap::set_max (float value)
{
    if (value > min_)
        max_ = value;
    resample();
}

float
Colormap::get_max (void)
{
    return max_;
}

void
Colormap::resample (void)
{
    samples_.clear();
    for (int i=0; i<=resolution_; i++) {
        float v = min_ + (i/float(resolution_)) * (max_-min_);
        Color c = color (v);
        samples_.push_back (c);
    }
}

ColormapPtr
Colormap::Ice (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "Ice";
        cmap->append (0.00f, Color (0.0f, 0.0f, 1.0f));
        cmap->append (0.50f, Color (0.5f, 0.5f, 1.0f));
        cmap->append (1.00f, Color (1.0f, 1.0f, 1.0f));
    }
    return cmap;
}

ColormapPtr
Colormap::Fire (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "Fire";
        cmap->append (0.00f, Color (1.0f, 1.0f, 1.0f));
        cmap->append (0.50f, Color (1.0f, 1.0f, 0.0f));
        cmap->append (1.00f, Color (1.0f, 0.0f, 0.0f));
    }
    return cmap;
}

ColormapPtr
Colormap::IceAndFire (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "IceAndFire";
        cmap->append ( 0.00f, Color (0.0f, 0.0f, 1.0f));
        cmap->append ( 0.25f, Color (0.5f, 0.5f, 1.0f));
        cmap->append ( 0.50f, Color (1.0f, 1.0f, 1.0f));
        cmap->append ( 0.75f, Color (1.0f, 1.0f, 0.0f));
        cmap->append ( 1.00f, Color (1.0f, 0.0f, 0.0f));
    }
    return cmap;
}

ColormapPtr
Colormap::Hot (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "Hot";
        cmap->append ( 0.00f, Color (0.0f, 0.0f, 0.0f));
        cmap->append ( 0.33f, Color (1.0f, 0.0f, 0.0f));
        cmap->append ( 0.66f, Color (1.0f, 1.0f, 0.0f));
        cmap->append ( 1.00f, Color (1.0f, 1.0f, 1.0f));
    }
    return cmap;
}

ColormapPtr
Colormap::Grey (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "Grey";
        cmap->append ( 0.0f, Color (0.0f, 0.00f, 0.00f));
        cmap->append ( 1.0f, Color (1.0f, 1.00f, 1.00f));
    }
    return cmap;
}

ColormapPtr
Colormap::Red (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "Red";
        cmap->append ( 0.0f, Color (0.0f, 0.00f, 0.00f));
        cmap->append ( 1.0f, Color (1.0f, 0.00f, 0.00f));
    }
    return cmap;
}

ColormapPtr
Colormap::Green (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "Green";
        cmap->append ( 0.0f, Color (0.0f, 0.00f, 0.00f));
        cmap->append ( 1.0f, Color (0.0f, 1.00f, 0.00f));
    }
    return cmap;
}

ColormapPtr
Colormap::Blue (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "Blue";
        cmap->append ( 0.0f, Color (0.0f, 0.00f, 0.00f));
        cmap->append ( 1.0f, Color (0.0f, 0.00f, 1.00f));
    }
    return cmap;
}

ColormapPtr
Colormap::LightRed (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "LightRed";
        cmap->append ( 0.0f, Color (1.00f, 1.00f, 1.00f));
        cmap->append ( 1.0f, Color (1.00f, 0.00f, 0.00f));
    }
    return cmap;
}

ColormapPtr
Colormap::LightGreen (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "LightGreen";
        cmap->append ( 0.0f, Color (1.00f, 1.00f, 1.00f));
        cmap->append ( 1.0f, Color (0.00f, 1.00f, 0.00f));
    }
    return cmap;
}

ColormapPtr
Colormap::LightBlue (void) {
    static ColormapPtr cmap;
    if (not cmap) {
        cmap = ColormapPtr (new Colormap());
        cmap->name_ = "LightBlue";
        cmap->append ( 0.0f, Color (1.00f, 1.00f, 1.00f));
        cmap->append ( 1.0f, Color (0.00f, 0.00f, 1.00f));
    }
    return cmap;
}

