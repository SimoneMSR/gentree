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
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "my_cloud.h"
#include "shapes.h"
#include <iostream>
using namespace std;
// ________________________________________________________________________ Cloud
Cloud::Cloud(void) :
	Object() {

	set_size(1.0f, 1.0f, 1.0f);
	set_position(0.0f, 0.0f, 0.0f);
	set_fg_color(0.5f, 0.5f, 0.5f, 1.0f);
	set_bg_color(1.0f, 1.0f, 1.0f, 1.0f);
	set_thickness(1.01);
	cmap_ = Colormap::Hot();
	reset_data();

	std::ostringstream oss;
	oss << "Cloud_" << id_;
	set_name(oss.str());
}

// _______________________________________________________________________ ~Cloud
Cloud::~Cloud(void) {
}

// ______________________________________________________________________ render
void Cloud::render(void) {
	if ((not xdata_) or (not ydata_)) {
		return;
	}
	compute_visibility();
	if (!get_visible()) {
		return;
	}
	unsigned int width = xdata_->get_width();
	unsigned int xdata_stride = xdata_->get_stride();
	unsigned int ydata_stride = ydata_->get_stride();
	double x, y, z,c;


	// -------------------------------------------------------------------------
	//  Rendering using GL_POINTS
	// -------------------------------------------------------------------------
	if (thickness_ == 0) {
		glEnable(GL_BLEND);
		glEnable(GL_POINT_SMOOTH);
		glColor3f(1, 0, 0);
			glPointSize(2);


		GLbyte *xdata = (GLbyte *) xdata_->get_data();
		GLbyte *ydata = (GLbyte *) ydata_->get_data();
		glBegin(GL_POINTS);
		for (unsigned int i = 0; i < width; i++) {
			x = *(GLdouble *) (xdata + i * xdata_stride);
			y = *(GLdouble *) (ydata + i * ydata_stride);
			z = 0;
			glVertex3f(x, y, z);
		}
		glEnd();

	}

	// -------------------------------------------------------------------------
	//  Rendering using polygon
	// -------------------------------------------------------------------------
	else {
		unsigned int cdata_stride = cdata_->get_stride();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		GLbyte *xdata = (GLbyte *) xdata_->get_data();
		GLbyte *ydata = (GLbyte *) ydata_->get_data();
		GLbyte *cdata = (GLbyte *) cdata_->get_data();
		for (unsigned int i = 0; i < width; i++) {
			x = *(GLdouble *) (xdata + i * xdata_stride);
			y = *(GLdouble *) (ydata + i * ydata_stride);
			c =  *(GLdouble *) (cdata + i * cdata_stride);
			if (c>=0)
				polygon(x, y, c);
		}
	}
}

// ________________________________________________________________ set_xyz_data
void Cloud::set_xyc_data(DataPtr xdata, DataPtr ydata, DataPtr cdata) {
	if ((not xdata) or (not ydata) or (not cdata)) {
		throw std::invalid_argument("X,Y,C data must be valid data");
	}
	if ((not (xdata->get_type() == xdata->get_type()))
			or (not (xdata->get_type() == cdata->get_type()))) {
		throw std::invalid_argument("X,Y,C data must be of same type");
	}
	if ((not (xdata->get_type() == GL_DOUBLE))) {
		throw std::invalid_argument("X,Y,C data must be of type GL_DOUBLE");
	}
	if ((not (xdata->get_width() == xdata->get_width()))
			or (not (xdata->get_width() == cdata->get_width()))) {
		throw std::invalid_argument("X,Y,C data must be of same size");
	}
	if ((not (xdata->get_width() > 1))) {
		throw std::invalid_argument("X,Y,C data must have at least two points");
	}
	if ((not (xdata->get_height() == 1)) or (not (ydata->get_height() == 1))
			or (not (cdata->get_height() == 1))) {
		throw std::invalid_argument("X,Y,C data must be linear (height=1)");
	}
	reset_data();
	xdata_ = xdata;
	ydata_ = ydata;
	cdata_= cdata;
	id_ = id_counter_;
	id_ += xdata->get_width();
}

// ________________________________________________________________ set_xy_data
void Cloud::set_xy_data(DataPtr xdata, DataPtr ydata) {
	if ((not xdata) or (not ydata)) {
		throw std::invalid_argument("X,Y data must be valid data");
	}
	if ((not (xdata->get_type() == ydata->get_type()))) {
		throw std::invalid_argument("X,Y data must be of same type");
	}
	if ((not (xdata->get_type() == GL_DOUBLE))) {
		throw std::invalid_argument("X,Y data must be of type GL_DOUBLE");
	}
	if ((not (xdata->get_width() == ydata->get_width()))) {
		throw std::invalid_argument("X,Y data must be of same size");
	}
	if ((not (xdata->get_width() > 1))) {
		throw std::invalid_argument("X,Y data must have at least two points");
	}
	if ((not (xdata->get_height() == 1)) or (not (ydata->get_height() == 1))) {
		throw std::invalid_argument("X,Y data must be linear (height=1)");
	}
	reset_data();
	xdata_ = xdata;
	ydata_ = ydata;
	cdata_=0;
	id_ = id_counter_;
	id_ += xdata->get_width();
}

// __________________________________________________________________ reset_data
void Cloud::reset_data(void) {
#if defined(HAVE_BOOST)
	xdata_ = DataPtr();
	ydata_ = DataPtr();
	cdata_ = DataPtr();
#else
	if(xdata_)
	xdata_->~Data();
	if(ydata_)
	ydata_ ->~Data();
	if(cdata_)
	cdata_ ->~Data();
#endif
}


// _______________________________________________________________ get_thickness
float Cloud::get_thickness(void) const {
	return thickness_;
}

// _______________________________________________________________ set_thickness
void Cloud::set_thickness(float thickness) {
	thickness_ = thickness;
}

// ________________________________________________________________ get_colormap
ColormapPtr Cloud::get_colormap(void) {
	return cmap_;
}

// ________________________________________________________________ set_colormap
void Cloud::set_colormap(ColormapPtr colormap) {
	cmap_ = colormap;
}
