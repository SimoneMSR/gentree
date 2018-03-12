/*
 * map.cpp
 *
 *  Created on: 13/mar/2011
 *      Author: simonemsr
 */

#include "map.h"

extern Interface interface;

void map_scale(int x, int y, double *xx, double *yy) {
	*xx = interface->map_origin[0] + (x / MAR_FACT)
			/ interface->map_scene->get_zoom();
	*yy = interface->map_origin[1] - (y / MAR_FACT)
			/ interface->map_scene->get_zoom();
}

void map_scale_inverse(double xx, double yy, int *x, int *y) {
	*x = (xx - interface->map_origin[0]) * MAR_FACT
			* interface->map_scene->get_zoom();
	*y = (interface->map_origin[1] - yy) * MAR_FACT
			* interface->map_scene->get_zoom();
}

void map_syncronize() {
	double zoom = 2 * MAR_Z / interface->grid_dim;
	double old_zoom = interface->map_scene->get_zoom();
	int a, b;
	interface->map_scene->set_zoom(zoom);
	zoom = interface-> map_scene->get_zoom();
	interface->map_origin[0] = interface->map_origin[0] / (zoom / old_zoom);
	interface->map_origin[1] = interface->map_origin[1] / (zoom / old_zoom);

	interface->temp_origin[0] = interface->grid_origin[0] - interface->grid_dim
			/ 2;
	interface->temp_origin[1] = interface->grid_origin[1] + interface->grid_dim
			/ 2;
	map_scale_inverse(interface->temp_origin[0], interface->temp_origin[1], &a,
			&b);
	interface->map_scene->mouse_action_start("move-resize", 0, 0);
	interface->map_scene->mouse_action(-a, -b);
	interface->map_scene->mouse_action_end(-a, -b);
	interface->map_origin[0] = interface->temp_origin[0];
	interface->map_origin[1] = interface->temp_origin[1];
	if (interface->map_show) {
		glutSetWindow(interface->map);
		glutShowWindow();
	}

}

void project() {
	interface->map_show = true;
	window_rename(interface->map);
	glutSetWindow(interface->map);
	glutPostRedisplay();
	glutShowWindow();

	interface->map_origin[0] = -1;
	interface->map_origin[1] = 1;
	int n = interface->num_points;
	GLdouble *data;
	data = new double[(n + 1) * 3];
	free_vector(interface->map_data);
	interface->map_data = data;
	for (int i = 0; i < n; i++) {
		data[i * 3 + 0] = interface->points[i * 2] / 2;
		data[i * 3 + 1] = interface->points[i * 2 + 1] / 2;
		data[i * 3 + 2] = interface->set->trees[interface->ids[i]]->label;
	}
	data[n * 3 + 2] = -1;

	DataPtr x = DataPtr(new Data());
	x->set(&data[0], n + 1, 1, 1, GL_DOUBLE, 3 * sizeof(GLdouble));

	DataPtr y = DataPtr(new Data());
	y->set(&data[1], n + 1, 1, 1, GL_DOUBLE, 3 * sizeof(GLdouble));

	if (interface->classes) {
		DataPtr c = DataPtr(new Data());
		c->set(&data[2], n + 1, 1, 1, GL_DOUBLE, 3 * sizeof(GLdouble));
		interface->map_widget->set_thickness(1.01);
		interface->map_widget->set_xyc_data(x, y, c);
	} else {
		interface->map_widget->set_thickness(0);
		interface->map_widget->set_xy_data(x, y);
	}

	interface->map_scene->set_zoom(MAR_Z);
	interface->map_scene->set_position(WIN_OFF, WIN_OFF, 0);
	interface->map_scene->set_orientation(0, 0);
}

void map_on_mouse_on(int x, int y) {
	double a, b;
	char line[LINE];
	map_scale(x, y, &a, &b);
	if (a < -1 || a > 1 || b < -1 || b > 1)
		return;
	int tree, t, n;
	tree = select_tree(a, b, &t, &n);
	if (t >= 0) {
		sprintf(line, "%s: (%f,%f)", interface->set->trees[t]->get_name(),
				interface->points[2 * tree], interface->points[2 * tree + 1]);
		interface->tree_info->set_buffer(line);
		interface->tree_info->set_position(10 + x
				- interface->map_scene->get_position().x, y
				- interface->map_scene->get_position().y);
		interface->tree_info->show();
	} else {
		interface->tree_info->hide();
	}
	glutPostRedisplay();
}

void map_on_mouse_button(int button, int state, int x, int y) {
	double a, b;
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			map_scale(x, y, &a, &b);
			if (a < -1 - MOUSE_POINT_TOLLERANCE || a > 1
					+ MOUSE_POINT_TOLLERANCE || b < -1 - MOUSE_POINT_TOLLERANCE
					|| b > 1 + MOUSE_POINT_TOLLERANCE)
				return;
			int t_id, n_id;
			select_tree(a, b, &t_id, &n_id);
			if (t_id >= 0)
				image(t_id, n_id);
		} else if (button == GLUT_RIGHT_BUTTON) {
			interface->map_scene->mouse_action_start("move-resize", x, y);
			map_scale(x, y, &a, &b);
			interface->temp_origin[0] = a;
			interface->temp_origin[1] = b;
		} else if (button == GLUT_MIDDLE_BUTTON) {
			interface->map_scene->set_zoom(MAR_Z);
			interface->map_scene->set_position(-1817, -1817, 0);
			interface->map_origin[0] = -1;
			interface->map_origin[1] = 1;
		}
	} else if (state == GLUT_UP) {
		interface->map_scene->mouse_action_end(x, y);
		if (button == GLUT_RIGHT_BUTTON) {
			map_scale(x, y, &a, &b);
			interface->map_origin[0] -= a - interface->temp_origin[0];
			interface->map_origin[1] -= b - interface->temp_origin[1];
		}
	}
	glutShowWindow();
}

void map_on_mouse_move(int x, int y) {
	interface->map_scene->mouse_action(x, y);
	glutPostRedisplay();
}

void map_keyboard(unsigned char c, int x, int y) {
	if (c == '+') {
		interface->map_scene->set_zoom(ZOOM * interface->map_scene->get_zoom());
		interface->map_origin[0] = interface->map_origin[0] / ZOOM;
		interface->map_origin[1] = interface->map_origin[1] / ZOOM;
	}
	if (c == '-') {
		interface->map_scene->set_zoom(interface->map_scene->get_zoom() / ZOOM);
		interface->map_origin[0] = interface->map_origin[0] * ZOOM;
		interface->map_origin[1] = interface->map_origin[1] * ZOOM;
	}
	glutPostRedisplay();
	if (c == 'q') {
		glutSetWindow(interface->map);
		glutHideWindow();
		glutSetWindow(interface->menu);
		interface->map_show = false;
	}
	if (c == 's') {
		char filename[LINE];
		sprintf(filename, "%s/map_%i.ppm", interface->working_dir,
				interface->win_save_nonce);
		interface->map_scene->save(filename);
		interface->win_save_nonce++;
	}
}

void map_display(void) {
	glutSetWindow(interface->map);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	interface->map_scene->render();
	glutSwapBuffers();
}
int select_tree(double x, double y, int *tree_id, int *node_id) {
	int i, sel = -1;
	double max_d = FLT_MAX;
	double d;
	for (i = 0; i < interface->num_points; i++) {
		d = sqrt(pow(x - interface->points[i * 2], 2) + pow(y
				- interface->points[i * 2 + 1], 2));
		if (d < RANGE && d < max_d) {
			max_d = d;
			sel = i;
		}
	}
	if (sel < 0)
		*tree_id = -1;
	else {
		*tree_id = interface->ids[sel];
		*node_id = interface->nodes_ids[sel];
	}
	return sel;
}
