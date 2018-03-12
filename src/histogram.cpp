/*
 * histogram.cpp
 *
 *  Created on: 13/mar/2011
 *      Author: simonemsr
 */

#include "histogram.h"

extern Interface interface;

void histogram_display() {
	glutSetWindow(interface->histogram);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	interface->histogram_scene->render();
	glutSwapBuffers();
}

void histogram_on_mouse_button(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			interface->histogram_scene->mouse_action_start("move-resize", x, y);
		} else if (button == GLUT_RIGHT_BUTTON) {
			interface->histogram_scene->mouse_action_start("rotate", x, y);
		} else if (button == GLUT_MIDDLE_BUTTON) {
			interface->histogram_scene->set_zoom(0.7);
			interface->histogram_scene->set_position(-750, -750, 0);
			Vec4f ori = Vec4f();
			ori.x = 90;
			ori.y = 90;
			ori.z = 0;
			interface->histogram_scene->set_orientation(ori);
		}
	} else {
	}
	glutPostRedisplay();
}

void histogram_on_mouse_move(int x, int y) {
	interface->histogram_scene->mouse_action(x, y);
	glutPostRedisplay();
}

void histogram_keyboard(unsigned char c, int x, int y) {
	if (c == '+')
		interface->histogram_scene->set_zoom(
				interface->histogram_scene->get_zoom() + 0.2);
	if (c == '-')
		interface->histogram_scene->set_zoom(
				interface->histogram_scene->get_zoom() - 0.2);
	glutPostRedisplay();
	if (c == 'q') {
		glutSetWindow(interface->histogram);
		glutHideWindow();
		glutSetWindow(interface->menu);
	}
	if (c == 's') {
		char filename[LINE];
		sprintf(filename, "%s/histogram_%i.ppm", interface->working_dir,
				interface->win_save_nonce);
		interface->histogram_scene->save(filename);
		interface->win_save_nonce++;
	}
}

void histogram() {
	int max_val = -1;
	int n = interface->gtm->C, *num = new int[interface->gtm->C], i;
	int point = interface->num_points;
	double xx, yy, **X = new_matrix(n, 2);

	window_rename(interface->histogram);
	glutSetWindow(interface->histogram);
	glutShowWindow();

	for (int i = 0; i < n; i++)
		num[i] = 0;
	for (int i = 0; i < point; i++)
		num[interface->indexes[i]]++;
	for (int i = 0; i < n; i++) {
		if (num[i] > max_val)
			max_val = num[i];
	}
	xx = 1 - (1.0 / sqrt(n));
	yy = (1.0 / sqrt(n)) - 1;
	for (i = 0; i < n; i++) {
		X[i][0] = xx;
		X[i][1] = yy;
		yy += 2.0 / sqrt(n);
		if (yy > 1) {
			yy = (1.0 / sqrt(n)) - 1;
			xx -= 2.0 / sqrt(n);
		}
	}

	GLdouble *data;
	data = new double[interface->gtm->C * (1 + 2 + 1 + 1)];
	free_vector(interface->histogram_data);
	interface->histogram_data = data;
	for (int i = 0; i < interface->gtm->C; i++) {
		data[i * 5 + 0] = X[i][0] / 2; //x
		data[i * 5 + 1] = X[i][1] / 2; //y
		data[i * 5 + 2] = double(num[i]) / max_val - 0.5; //height
		data[i * 5 + 3] = 1.0 / n; //r
		data[i * 5 + 4] = double(i + 1) / n;//color
	}
	DataPtr x = DataPtr(new Data());
	x->set(&data[0], n, 1, 1, GL_DOUBLE, 5 * sizeof(GLdouble));

	DataPtr y = DataPtr(new Data());
	y->set(&data[1], n, 1, 1, GL_DOUBLE, 5 * sizeof(GLdouble));

	DataPtr z = DataPtr(new Data());
	z->set(&data[2], n, 1, 1, GL_DOUBLE, 5 * sizeof(GLdouble));

	DataPtr r = DataPtr(new Data());
	r->set(&data[3], n, 1, 1, GL_DOUBLE, 5 * sizeof(GLdouble));

	DataPtr c = DataPtr(new Data());
	c->set(&data[4], n, 1, 1, GL_DOUBLE, 5 * sizeof(GLdouble));
	interface->histogram_widget->set_xyz_c_data(x, y, z, r, c);
	interface->histogram_scale-> set (max_val,0,11);
	delete (num);

}
