#include "gram.h"
#include "shapes.h"

// ________________________________________________________________________ Cloud
Histogram::Histogram(void) :
	Object() {
	set_size(1.0f, 1.0f, 1.0f);
	set_position(0.0f, 0.0f, 0.0f);
	set_fg_color(0.5f, 0.5f, 0.5f, 0);
	set_bg_color(0, 0, 0, 1);
	cmap_ = Colormap::Hot();
	reset_data();
	alpha_ = 0;
	std::ostringstream oss;
	oss << "Histogram_" << id_;
	set_name(oss.str());
}

// _______________________________________________________________________ ~Cloud
Histogram::~Histogram(void) {
}

// ______________________________________________________________________ render
void Histogram::render(void) {
	if ((not xdata_) or (not ydata_) or (not zdata_) or (not rdata_)) {
		return;
	}
	compute_visibility();
	if (!get_visible()) {
		return;
	}
	unsigned int width = xdata_->get_width();
	unsigned int xdata_stride = xdata_->get_stride();
	unsigned int ydata_stride = ydata_->get_stride();
	unsigned int zdata_stride = zdata_->get_stride();
	unsigned int rdata_stride = rdata_->get_stride();
	Color c;
	double x, y, rad, z, v, r, g, b, a;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if ((cdata_)) {
		GLbyte *xdata = (GLbyte *) xdata_->get_data();
		GLbyte *ydata = (GLbyte *) ydata_->get_data();
		GLbyte *zdata = (GLbyte *) zdata_->get_data();
		GLbyte *rdata = (GLbyte *) rdata_->get_data();
		GLbyte *cdata = (GLbyte *) cdata_->get_data();
		unsigned int cdata_stride = cdata_->get_stride();

		// XYZ + C
		// -----------------------------------------------------------------
		if (cdata_->get_depth() == 1) {
			for (unsigned int i = 0; i < width; i++) {
				x = *(GLdouble *) (xdata + i * xdata_stride);
				y = *(GLdouble *) (ydata + i * ydata_stride);
				rad = *(GLdouble *) (rdata + i * rdata_stride);
				z = *(GLdouble *) (zdata + i * zdata_stride);
				v = *(GLdouble *) (cdata + i * cdata_stride + 0
						* sizeof(GLdouble));
				Color c = (*cmap_)(v);
				r = c.r;
				g = c.g;
				b = c.b;
				a = c.a;
				glColor4f(r, g, b, a * alpha_);
				cylinder(x, y, -0.5, rad, x, y, z, rad);
			}
		}
	}

}

// ________________________________________________________________ set_xyz_data
void Histogram::set_xyz_data(DataPtr xdata, DataPtr ydata, DataPtr zdata,
		DataPtr rdata) {
	if ((not xdata) or (not ydata) or (not zdata) or (not rdata)) {
		throw std::invalid_argument("X,Y,Z data must be valid data");
	}
	if ((not (xdata->get_type() == xdata->get_type()))
			or (not (xdata->get_type() == zdata->get_type()))) {
		throw std::invalid_argument("X,Y,Z data must be of same type");
	}
	if ((not (xdata->get_type() == GL_DOUBLE))) {
		throw std::invalid_argument("X,Y,Z data must be of type GL_DOUBLE");
	}
	if ((not (xdata->get_width() == xdata->get_width()))
			or (not (xdata->get_width() == zdata->get_width()))) {
		throw std::invalid_argument("X,Y,Z data must be of same size");
	}
	if ((not (xdata->get_width() > 1))) {
		throw std::invalid_argument("X,Y,Z data must have at least two points");
	}
	if ((not (xdata->get_height() == 1)) or (not (ydata->get_height() == 1))
			or (not (rdata->get_height() == 1)) or (not (zdata->get_height()
			== 1))) {
		throw std::invalid_argument("X,Y,Z data must be linear (height=1)");
	}
	reset_data();
	xdata_ = xdata;
	ydata_ = ydata;
	rdata_ = rdata;
	zdata_ = zdata;
	id_ = id_counter_;
	id_ += xdata->get_width();
}

// ______________________________________________________________ set_xyz_s_data


// ______________________________________________________________ set_xyz_c_data
void Histogram::set_xyz_c_data(DataPtr xdata, DataPtr ydata, DataPtr zdata,
		DataPtr rdata, DataPtr cdata) {
	if ((not xdata) or (not ydata) or (not zdata) or (not rdata) or (not cdata)) {
		throw std::invalid_argument("X,Y,Z,C data must be valid data");
	}
	if ((not (xdata->get_type() == xdata->get_type()))
			or (not (xdata->get_type() == zdata->get_type()))) {
		throw std::invalid_argument("X,Y,Z data must be of same type");
	}
	if ((not (xdata->get_type() == GL_DOUBLE))) {
		throw std::invalid_argument("X,Y,Z data must be of type GL_DOUBLE");
	}
	if ((not (xdata->get_width() == xdata->get_width()))
			or (not (xdata->get_width() == zdata->get_width()))
			or (not (xdata->get_width() == cdata->get_width()))) {
		throw std::invalid_argument("X,Y,Z,C data must be of same size");
	}
	if ((not (xdata->get_width() > 1))) {
		throw std::invalid_argument("X,Y,Z data must have at least two points");
	}
	if ((not (xdata->get_height() == 1)) or (not (ydata->get_height() == 1))
			or (not (rdata->get_height() == 1)) or (not (zdata->get_height()
			== 1)) or (not (cdata->get_height() == 1))) {
		throw std::invalid_argument("X,Y,Z,C data must be linear (height=1)");
	}
	if ((not (cdata->get_type() == GL_DOUBLE))) {
		throw std::invalid_argument("C data must be of type GL_DOUBLE");
	}
	if ((not (cdata->get_depth() == 1)) and (not (cdata->get_depth() == 3))
			and (not (cdata->get_depth() == 4))) {
		throw std::invalid_argument(
				"C data must have 1 (cmap value), 3 (rgb) or 4 (rgba) components");
	}
	reset_data();
	xdata_ = xdata;
	ydata_ = ydata;
	rdata_ = rdata;
	zdata_ = zdata;
	cdata_ = cdata;
	id_ = id_counter_;
	id_ += xdata->get_width();
}

// ____________________________________________________________ set_xyz_s_c_data

// __________________________________________________________________ reset_data
void Histogram::reset_data(void) {
#if defined(HAVE_BOOST)
	xdata_ = DataPtr();
	ydata_ = DataPtr();
	zdata_ = DataPtr();
	sdata_ = DataPtr();
	cdata_ = DataPtr();
#else
	if (xdata_)
		xdata_->~Data();
	if (ydata_)
		ydata_ ->~Data();
	if (cdata_)
		cdata_ ->~Data();
	if (rdata_)
		rdata_->~Data();
	if (zdata_)
		zdata_->~Data();

#endif
}

// ________________________________________________________________ get_colormap
ColormapPtr Histogram::get_colormap(void) {
	return cmap_;
}

// ________________________________________________________________ set_colormap
void Histogram::set_colormap(ColormapPtr colormap) {
	cmap_ = colormap;
}
