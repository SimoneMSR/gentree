/*
 * grid.cpp
 *
 *  Created on: 13/mar/2011
 *      Author: simonemsr
 */

#include "grid.h"

extern Interface interface;

void center(int i, double center[2]) {
	int d = sqrt(interface->cells);
	center[0] = (i % d + 0.5) * GRI_SIZ / sqrt(interface->cells);
	center[1] = ((d - ((i) / d) - 1) + 0.5) * GRI_SIZ / sqrt(interface->cells);
}

int in(double x, double y) {
	y = GRI_SIZ - y;
	int cells = interface->cells;
	if (x == GRI_SIZ) {
		if (y == GRI_SIZ)
			return cells - 1;
		else
			return sqrt(cells) * ((int) (sqrt(cells) * y / GRI_SIZ) + 1) - 1;
	} else {
		if (y == GRI_SIZ)
			return (cells - sqrt(cells)) + (int) (sqrt(cells) * x / GRI_SIZ);
	}
	int xx = sqrt(cells) * x / GRI_SIZ;
	int yy = sqrt(cells) * y / GRI_SIZ;
	return (xx) + sqrt(cells) * yy;
}

int is_in(int num_cell, double xxx, double yyy) {
	double x = xxx + 1, y = yyy + 1;
	if (x == 2) {
		if (y == 2)
			return num_cell - 1;
		else
			return sqrt(num_cell) * ((int) (sqrt(num_cell) * y / 2) + 1) - 1;
	} else {
		if (y == 2)
			return (num_cell - sqrt(num_cell)) + (int) (sqrt(num_cell) * x / 2);
	}
	int xx = sqrt(num_cell) * x / 2;
	int yy = sqrt(num_cell) * y / 2;
	return (xx) + sqrt(num_cell) * yy;
}


void scale(double x, double y, double xy[2]) {
	xy[0] = (x - GRI_SIZ / 2) * interface->grid_dim / GRI_SIZ;
	xy[1] = (GRI_SIZ / 2 - y) * interface->grid_dim / GRI_SIZ;
}


void scale_inverse(double x, double y, double xy[2]) {
	x -= interface->grid_origin[0];
	y -= interface->grid_origin[1];
	xy[0] = GRI_SIZ * x / interface->grid_dim + GRI_SIZ / 2;
	xy[1] = -GRI_SIZ * y / interface->grid_dim + GRI_SIZ / 2;
}



void histogrid_zoom_in(double o_x, double o_y) {
	if (((o_x >= 0 && o_x <= GRI_SIZ) && (o_y >= 0 && o_y <= GRI_SIZ))) {
		interface->num_zoom++;
		double *new_origin = new double[2];
		double *point = new double[2];
		int i, o = in(o_x, o_y);
		char text[5];
		center(o, new_origin);
		scale(new_origin[0], new_origin[1], new_origin);
		interface->grid_origin[0] += new_origin[0];
		interface->grid_origin[1] += new_origin[1];
		interface->grid_dim = interface->grid_dim / sqrt(interface->cells);
		sprintf(text, "%.4g", interface->grid_origin[0] - interface->grid_dim
				/ 2);
		interface->textbox1->set_buffer(text);
		sprintf(text, "%.4g", (interface->grid_origin[0] + interface->grid_dim
				/ 2) / 2);
		interface->textbox2->set_buffer(text);
		sprintf(text, "%.4g", interface->grid_origin[0] + interface->grid_dim
				/ 2);
		interface->textbox3->set_buffer(text);
		sprintf(text, "%.4g", interface->grid_origin[1] + interface->grid_dim
				/ 2);
		interface->textbox4->set_buffer(text);
		sprintf(text, "%.4g", (interface->grid_origin[1] + interface->grid_dim
				/ 2) / 2);
		interface->textbox5->set_buffer(text);
		sprintf(text, "%.4g", interface->grid_origin[1] - interface->grid_dim
				/ 2);
		interface->textbox6->set_buffer(text);

		for (i = 0; i < interface->cells; i++)
			interface->grid_cell_num[i] = 0;
		for (i = 0; i < 2 * interface->num_points; i = i + 2) {
			if (content(interface->points[i], interface->points[i + 1])) {
				scale_inverse(interface->points[i], interface->points[i + 1],
						point);
				interface->grid_cell_num[in(point[0], point[1])]++;
			}
		}

		int width = 64;
		int height = 64;
		GLdouble *values = new GLdouble[width * height];
		GLdouble min = 0;
		GLdouble max = 1;

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				GLdouble x = -3.0f + 6.0f * (i / GLdouble(width));
				GLdouble y = -3.0f + 6.0f * (j / GLdouble(height));

				GLdouble v = interface->grid_cell_num[is_in(interface->cells, x
						/ 3, y / 3)];
				values[j * width + i] = v;
				if (v < min)
					min = v;
				else if (v > max)
					max = v;
			}
		}
		DataPtr data = DataPtr(new Data);
		data->set(values, width, height, 1, GL_DOUBLE, sizeof(GLdouble));
		ColormapPtr cmap = Colormap::LightBlue();
		cmap->scale(min, max);
		interface->grid_widget->set_colormap(cmap);
		interface->grid_widget->set_c_data(data);
		interface->grid_scene->add(interface->grid_widget);
		interface->grid_scene->render();
		map_syncronize();
		delete (new_origin);
	}
}

void histogrid_keyboard(unsigned char c, int x, int y) {
	if (c == 'q') {
		glutSetWindow(interface->histogrid);
		glutHideWindow();
		glutSetWindow(interface->menu);
	}
	if (c == 's') {
		char filename[LINE];
		sprintf(filename, "%s/grid_%i.ppm", interface->working_dir,
				interface->win_save_nonce);
		interface->grid_scene->save(filename);
		interface->win_save_nonce++;
	}
}

void histogrid_zoom_out() {
	double *mouse = new double[2], *origin = new double[2], *centre =
			new double[2], *point = new double[2];
	char text[5];
	if (interface->num_zoom >= 1) {
		origin[0] = interface->grid_origin[0];
		origin[1] = interface->grid_origin[1];
		interface->grid_origin[0] = 0;
		interface->grid_origin[1] = 0;
		interface->grid_dim = 2;
		int i, o;
		for (i = 0; i < interface->num_zoom - 1; i++) {
			scale_inverse(origin[0], origin[1], mouse);
			o = in(mouse[0], mouse[1]);
			center(o, centre);
			scale(centre[0], centre[1], centre);
			interface->grid_origin[0] += centre[0];
			interface->grid_origin[1] += centre[1];
			interface->grid_dim = interface->grid_dim / sqrt(interface->cells);
		}
		sprintf(text, "%.4g", interface->grid_origin[0] - interface->grid_dim
				/ 2);
		interface->textbox1->set_buffer(text);
		sprintf(text, "%.4g", interface->grid_origin[0]);
		interface->textbox2->set_buffer(text);
		sprintf(text, "%.4g", interface->grid_origin[0] + interface->grid_dim
				/ 2);
		interface->textbox3->set_buffer(text);
		sprintf(text, "%.4g", interface->grid_origin[1] + interface->grid_dim
				/ 2);
		interface->textbox4->set_buffer(text);
		sprintf(text, "%.4g", interface->grid_origin[1]);
		interface->textbox5->set_buffer(text);
		sprintf(text, "%.4g", interface->grid_origin[1] - interface->grid_dim
				/ 2);
		interface->textbox6->set_buffer(text);

		for (i = 0; i < interface->cells; i++)
			interface->grid_cell_num[i] = 0;
		for (i = 0; i < 2 * interface->num_points; i = i + 2) {
			if (content(interface->points[i], interface->points[i + 1])) {
				scale_inverse(interface->points[i], interface->points[i + 1],
						point);
				interface->grid_cell_num[in(point[0], point[1])]++;
			}
		}

		int width = 64;
		int height = 64;
		GLdouble *values = new GLdouble[width * height];
		GLdouble min = 0;
		GLdouble max = 1;

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				GLdouble x = -3.0f + 6.0f * (i / GLdouble(width));
				GLdouble y = -3.0f + 6.0f * (j / GLdouble(height));

				GLdouble v = interface->grid_cell_num[is_in(interface->cells, x
						/ 3, y / 3)];
				values[j * width + i] = v;
				if (v < min)
					min = v;
				else if (v > max)
					max = v;
			}
		}
		DataPtr data = DataPtr(new Data);
		data->set(values, width, height, 1, GL_DOUBLE, sizeof(GLdouble));
		ColormapPtr cmap = Colormap::LightBlue();
		cmap->scale(min, max);
		interface->grid_widget->set_colormap(cmap);
		interface->grid_widget->set_c_data(data);
		interface->grid_scene->add(interface->grid_widget);
		interface->grid_scene->render();
		interface->num_zoom--;
		map_syncronize();
	}

}

bool content(double x, double y) {
	return x > (interface->grid_origin[0] - interface->grid_dim / 2) && x
			<= (interface->grid_origin[0] + interface->grid_dim / 2) && y
			> (interface->grid_origin[1] - interface->grid_dim / 2) && y
			<= (interface->grid_origin[1] + interface->grid_dim / 2);
}

void histogrid() {
	if (not is_square(interface->cells)) {
		message_show(3);
		return;
	}
	window_rename(interface->histogrid);
	glutSetWindow(interface->histogrid);
	glutPostRedisplay();
	glutShowWindow();
	interface->grid_dim = 2;
	interface->num_zoom = 0;
	interface->grid_origin[0] = 0;
	interface->grid_origin[1] = 0;
	interface->grid_cell_dim = 2 / sqrt(interface->cells);
	int cell = interface->cells;
	free_vector(interface->grid_cell_num);
	interface->grid_cell_num = new int[cell];
	int i;
	for (i = 0; i < cell; i++)
		interface->grid_cell_num[i] = 0;
	for (i = 0; i < 2 * interface->num_points; i = i + 2) {
		interface->grid_cell_num[is_in(cell, interface->points[i],
				interface->points[i + 1])]++;
	}

	interface->textbox1->set_buffer("-1");
	interface->textbox2->set_buffer("0");
	interface->textbox3->set_buffer("1");
	interface->textbox4->set_buffer("1");
	interface->textbox5->set_buffer("0");
	interface->textbox6->set_buffer("-1");

	int width = 64;
	int height = 64;
	GLdouble *values = new GLdouble[width * height];
	free_vector(interface->grid_data);
	interface->grid_data = values;
	GLdouble min = 0;
	GLdouble max = 1;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			GLdouble x = -3.0f + 6.0f * (i / GLdouble(width));
			GLdouble y = -3.0f + 6.0f * (j / GLdouble(height));

			GLdouble v = interface->grid_cell_num[is_in(cell, x / 3, y / 3)];
			values[j * width + i] = v;
			if (v < min)
				min = v;
			else if (v > max)
				max = v;
		}
	}
	DataPtr data = DataPtr(new Data);
	data->set(values, width, height, 1, GL_DOUBLE, sizeof(GLdouble));

	ColormapPtr cmap = Colormap::LightBlue();
	cmap->scale(min, max);
	interface->grid_widget->set_colormap(cmap);
	interface->grid_widget->set_c_data(data);

}

void histogrid_on_mouse_button(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_RIGHT_BUTTON) {
			histogrid_zoom_out();
		} else if (button == GLUT_LEFT_BUTTON) {
			histogrid_zoom_in(x - GRI_OFF_X, y - GRI_OFF_Y);
		}
	}
	glutPostRedisplay();
}

void histogrid_on_mouse_on(int x, int y) {
	x -= GRI_OFF_X;
	y -= GRI_OFF_Y;
	if (((x >= 0 && x <= GRI_SIZ) && (y >= 0 && y <= GRI_SIZ))) {

		double mouse[2];
		int cell = in(x, y);
		scale(x, y, mouse);
		char line[LINE];
		if (interface->grid_cell_num[cell] > 0) {
			sprintf(line, "%i hits", interface->grid_cell_num[cell]);
			interface->cell_info->set_buffer(line);
			interface->cell_info->set_position(x, y);
			interface->cell_info->show();
			glutPostRedisplay();
		}
	} else
		interface->cell_info->hide();
}

void histogrid_reshape(int x, int y) {
	glutSetWindow(interface->histogrid);
	glViewport(0, 0, x, y);
	glutReshapeWindow(WIN_SIZ_Y, WIN_SIZ);
	glutSwapBuffers();
	glutPostRedisplay();
}

void histogrid_display(void) {
	glutSetWindow(interface->histogrid);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	interface->grid_scene->render();
	glutSwapBuffers();
}
