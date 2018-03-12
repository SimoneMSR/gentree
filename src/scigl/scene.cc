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
#include <iostream>
#include "trackball.h" 
#include "scene.h"

// _______________________________________________________________________ Scene
Scene::Scene(void) :
	Widget() {
	view_ = Vec4f(0, 0, 0, 1);
	set_orientation(65, 135);
	zoom_ = 1.5;
	zoom_max_ = 10;
	zoom_min_ = 0.001f;
	start_ = Position(-1, -1);
	pointer_ = Position(-1, -1);
	focus_ = 0;
	ortho_mode_ = false;

	std::ostringstream oss;
	oss << "Scene_" << id_;
	set_name(oss.str());
}

// ______________________________________________________________________ ~Scene
Scene::~Scene(void) {
}

// _______________________________________________________________________ setup
void Scene::setup(void) {
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { 2.0f, 2.0f, 2.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
}

// ______________________________________________________________________ update
void Scene::update(void) {
	for (unsigned int i = 0; i < widgets_.size(); i++)
		widgets_.at(i)->update();
	for (unsigned int i = 0; i < objects_.size(); i++)
		objects_.at(i)->update();
}

// _______________________________________________________________________ render
void Scene::render(void) {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLint scissor[4];
	glGetIntegerv(GL_SCISSOR_BOX, scissor);
	GLint scissor_active;
	GLint mode;
	glGetIntegerv(GL_RENDER_MODE, &mode);
	glGetIntegerv(GL_SCISSOR_TEST, &scissor_active);
	float height = viewport[3];

	render_start();
	if (not get_visible()) {
		render_finish();
		return;
	}
	if (mode == GL_RENDER) {
		Widget::render();
	}
	render_finish();

	glPushAttrib(GL_VIEWPORT_BIT | GL_SCISSOR_BIT);

	int border = 2;
	if ((get_br_color().alpha * alpha_) == 0) {
		border = 0;
	}

	// Set scissor test
	int x = viewport[0] + int(get_position().x + border / 2);
	int y = viewport[1] + int(height - get_position().y - get_size().y + border
			/ 2);
	int w = int(get_size().x) - border;
	int h = int(get_size().y) - border;
	if (scissor_active) {
		if (x < scissor[0])
			x = scissor[0];
		if (y < scissor[1])
			y = scissor[1];
		if ((x + w) > (viewport[0] + scissor[2]))
			w += (viewport[0] + scissor[2]) - (x + w);
		if ((y + h) > (viewport[1] + scissor[3]))
			h += (viewport[1] + scissor[3]) - (y + h);
	}
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
	// Set viewport
	glViewport(viewport[0] + int(get_position().x) + border / 2, viewport[1]
			+ int(height - get_position().y - get_size().y + border / 2),
			int(get_size().x) - border, int(get_size().y) - border);

	// Set modelview
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	if (mode == GL_SELECT) {
		GLint v[4];
		glGetIntegerv(GL_VIEWPORT, v);
		gluPickMatrix(pointer_.x, pointer_.y, 5, 5, v);
	}

	float aspect = 1.0f;
	aspect = get_size().x / get_size().y;
	float aperture = 25.0f;
	float near = 1.0f;
	float far = 100.0f;
	float top = tan(aperture * 3.14159 / 360.0) * near;
	float bottom = -top;
	float left = aspect * bottom;
	float right = aspect * top;
	if (get_ortho_mode())
		glOrtho(left, right, bottom, top, near, far);
	else
		glFrustum(left, right, bottom, top, near, far);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0, 0, -8.0f);
	glScalef(zoom_, zoom_, zoom_);
	float m[4][4];
	build_rotmatrix(m, view_.data);
	glMultMatrixf(&m[0][0]);

	//
	setup();

	// Back widgets
	glDisable(GL_DEPTH_TEST);
	for (unsigned int i = 0; i < widgets_.size(); i++)
		if (widgets_.at(i)->get_position().z < 0)
			widgets_.at(i)->render();

	// Objects
	glEnable(GL_DEPTH_TEST);
	//glColor4f(1,1,1,1);
	//glEnable (GL_LIGHT0);
	glEnable(GL_LIGHTING);
	for (unsigned int i = 0; i < objects_.size(); i++)
		objects_.at(i)->render();
	//glDisable (GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// Front widgets
	glDisable(GL_DEPTH_TEST);
	for (unsigned int i = 0; i < widgets_.size(); i++)
		if (widgets_.at(i)->get_position().z >= 0)
			widgets_.at(i)->render();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	//glDisable (GL_SCISSOR_TEST);
	glPopAttrib();
}

// ________________________________________________________________________ save
void Scene::save(std::string filename) {
	GLuint framebuffer, renderbuffer;
	GLenum status;
	glewInit();

	GLuint width = int(get_size().x), height = int(get_size().y);
	glGenFramebuffersEXT(1, &framebuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
	glGenRenderbuffersEXT(1, &renderbuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA8, width, height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			GL_RENDERBUFFER_EXT, renderbuffer);
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
		return;
	render();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	GLubyte *buffer = new GLubyte[width * height * 3];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	FILE *file = fopen(filename.c_str(), "wb");
	fprintf(file, "P6 %d %d 255\n", int(width), int(height));
	int n = fwrite(buffer, sizeof(GLubyte), width * height * 3, file);
	if (not n)
		std::cerr << "Error while saving figure\n" << std::endl;
	fclose(file);
	delete buffer;
	glDeleteRenderbuffersEXT(1, &renderbuffer);
}

// _________________________________________________________________________ add
void Scene::add(ObjectPtr object) {
	objects_.push_back(object);
}

// _________________________________________________________________________ add
void Scene::add(WidgetPtr widget) {
	unsigned int i;
	for (i = 0; i < widgets_.size(); i++) {
		if (widget->get_position().z < widgets_[i]->get_position().z) {
			widgets_.insert(widgets_.begin() + i, widget);
			return;
		}
	}
	widgets_.push_back(widget);
}

// __________________________________________________________________ get_object
ObjectPtr Scene::get_object(const std::string name) const {
	return ObjectPtr();
}

// __________________________________________________________________ operator[]
ObjectPtr Scene::operator[](const std::string name) const {
	return ObjectPtr();
}

// __________________________________________________________________ set_object
void Scene::set_object(const std::string name, const ObjectPtr object) {
}

// __________________________________________________________________ has_object
bool Scene::has_object(const std::string name) {
	return false;
}

// __________________________________________________________________ del_object
void Scene::del_object(const std::string name) {
}

// ______________________________________________________________________ select
bool Scene::select(int x, int y) {
	return false;
}

// _____________________________________________________________ keyboard_action
bool Scene::keyboard_action(std::string action, std::string keyname) {
	if (not keyname.size())
		return false;
	for (unsigned int i = 0; i < widgets_.size(); i++)
		if (widgets_[i]->keyboard_action(action, keyname))
			return true;
	for (unsigned int i = 0; i < objects_.size(); i++)
		if (objects_[i]->keyboard_action(action, keyname))
			return true;
	return false;
}

// __________________________________________________________ mouse_action_start
bool Scene::mouse_action_start(std::string action, int x, int y) {
	// Selection
	/*
	 if (button == 2) {
	 GLint viewport[4];
	 glGetIntegerv (GL_VIEWPORT, viewport);
	 pointer_.x = x;
	 pointer_.y = viewport[3]-y;
	 GLuint buffer[512];
	 GLint hits;
	 glSelectBuffer (512, buffer);
	 glRenderMode (GL_SELECT);
	 glInitNames ();
	 glPushName (0);
	 glPushMatrix();
	 glLoadIdentity();
	 render();
	 hits = glRenderMode (GL_RENDER);
	 GLuint choose = 0, depth = 0;
	 //printf("Hits: %d\n", int(hits));
	 if (hits > 0) {
	 choose = buffer[3];
	 depth  = buffer[1];
	 for (int loop=1; loop<hits; loop++) {
	 if (buffer[loop*4+1] < GLuint(depth)) {
	 choose = buffer[loop*4+3];
	 depth = buffer[loop*4+1];
	 }
	 }
	 }
	 glPopMatrix();
	 glPopName();
	 if (choose) {
	 for (unsigned int i=0; i<objects_.size(); i++) {
	 if (objects_[i]->check_id(choose)) {
	 // WidgetPtr widget = WidgetPtr (widgets_.at(i));
	 // selection_ = WidgetPtr (widget);
	 }
	 }
	 return true;
	 }
	 return true;
	 }
	 */

	focus_ = 0;
	if (not has_focus(x, y))
		return false;
	x -= get_position().x;
	y -= get_position().y;
	if (action == "move-resize") {
		for (unsigned int i = 0; i < widgets_.size(); i++)
			if (widgets_[i]->has_focus(x, y)) {
#if defined(HAVE_BOOST)
				focus_ = widgets_[i].get();
#else
				focus_ = widgets_[i];
#endif
			}
		if (focus_) {
			focus_->mouse_action_start(action, x, y);
			return true;
		}
		focus_ = this;
		focus_->Widget::mouse_action_start(action, x + get_position().x, y
				+ get_position().y);
		return true;
	}

	// Is there a scene that got focus ?
	for (unsigned int i = 0; i < widgets_.size(); i++)
		if (widgets_[i]->has_focus(x, y)) {
#if defined(HAVE_BOOST)
			Scene *scene = dynamic_cast<Scene *>(widgets_[i].get());
			if (scene != 0) {
				focus_ = widgets_[i].get();
			}
#else
			Scene *scene = dynamic_cast<Scene *> (widgets_[i]);
			if (scene != 0) {
				focus_ = widgets_[i];
			}
#endif
		}
	if (focus_) {
		focus_->mouse_action_start(action, x, y);
		return true;
	}

	if ((action == "rotate") or (action == "zoom")) {
		action_ = action;
		start_ = Position(x, y);
		return true;
	} else if ((action == "zoom-in")) {
		set_zoom(get_zoom() * 1.05);
		return true;
	} else if ((action == "zoom-out")) {
		set_zoom(get_zoom() / 1.05);
		return true;
	}

	action_ = "";
	return false;
}

// ____________________________________________________________ mouse_action_end
bool Scene::mouse_action_end(int x, int y) {
	Scene *scene = dynamic_cast<Scene *> (focus_);
	action_ = "";
	if (scene != 0) {
		scene->action_ = "";
		return false;
	}
	return false;
}

// ________________________________________________________________ mouse_action
bool Scene::mouse_action(int x, int y) {
	x -= get_position().x;
	y -= get_position().y;

	if (focus_) {
		if (focus_ != this)
			focus_->mouse_action(x, y);
		else
			focus_->Widget::mouse_action(x + get_position().x, y
					+ get_position().y);
		return true;
	}

	float w = get_size().x;
	float h = get_size().y;
	if (action_ == "rotate") {
		float d_quat[4];
		trackball(d_quat, (2.0 * start_.x - w) / w, (h - 2.0 * start_.y) / h,
				(2.0 * x - w) / w, (h - 2.0 * y) / h);
		add_quats(d_quat, view_.data, view_.data);
		start_.x = x;
		start_.y = y;
		return true;
	} else if (action_ == "zoom") {
		zoom_ = zoom_ * (1.0 - (y - start_.y) / h);
		if (zoom_ > zoom_max_) {
			zoom_ = zoom_max_;
		} else if (zoom_ < zoom_min_) {
			zoom_ = zoom_min_;
		}
		start_.x = x;
		start_.y = y;
	}
	return false;
}

// ______________________________________________________________ set_ortho_mode
void Scene::set_ortho_mode(bool mode) {
	ortho_mode_ = mode;
}

// ______________________________________________________________ get_ortho_mode
bool Scene::get_ortho_mode(void) {
	return ortho_mode_;
}

// ____________________________________________________________________ set_zoom
void Scene::set_zoom(float zoom) {
	zoom_ = zoom;
	if (zoom_ > zoom_max_)
		zoom_ = zoom_max_;
	if (zoom_ < zoom_min_)
		zoom_ = zoom_min_;
}

// ____________________________________________________________________ get_zoom
float Scene::get_zoom(void) {
	return zoom_;
}

// _____________________________________________________________ set_orientation
void Scene::set_orientation(Orientation orientation) {
	orientation_ = Orientation(orientation);
	view_ = Vec4f(0, 0, 0, 1);

	float xrot[4], zrot[4];
	float angle, sine;

	angle = orientation_.x * (M_PI / 180.0f);
	sine = sin(0.5 * angle);
	xrot[0] = 1 * sine;
	xrot[1] = 0 * sine;
	xrot[2] = 0 * sine;
	xrot[3] = cos(0.5 * angle);

	angle = orientation.y * (M_PI / 180.0f);
	sine = sin(0.5 * angle);
	zrot[0] = 0 * sine;
	zrot[1] = 0 * sine;
	zrot[2] = 1 * sine;
	zrot[3] = cos(0.5 * angle);
	add_quats(xrot, zrot, view_.data);
}

// _____________________________________________________________ set_orientation
void Scene::set_orientation(float x, float z) {
	set_orientation(Orientation(x, z));
}

// _____________________________________________________________ get_orientation
Orientation Scene::get_orientation(void) {
	double q0 = view_.x;
	double q1 = view_.y;
	double q2 = view_.z;
	double q3 = view_.w;
	double ax = atan(2 * (q0 * q1 + q2 * q3) / (1 - 2 * (q1 * q1 + q2 * q2)))
			* 180.0 / M_PI;
	//double ay = asin(2*(q0*q2-q3*q1))                     * 180.0/M_PI;
	double az = atan(2 * (q0 * q3 + q1 * q2) / (1 - 2 * (q2 * q2 + q3 * q3)))
			* 180.0 / M_PI;
	orientation_.x = -az;
	orientation_.y = ax;
	orientation_.z = 0;
	orientation_.w = 0;
	return orientation_;
}

// ___________________________________________________________________ has_focus
bool Scene::has_focus(int x, int y) {
	bool focus = false;
	for (unsigned int i = 0; i < widgets_.size(); i++)
		focus |= widgets_[i]->has_focus(x + get_position().x, y
				+ get_position().y);
	focus |= Widget::has_focus(x, y);
	return focus;
}

void Scene::set_max_zoom(double max) {
	zoom_max_ = max;
}
