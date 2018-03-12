/*
 * image.cpp
 *
 *  Created on: 13/mar/2011
 *      Author: simonemsr
 */

#include "image.h"

extern Interface interface;

Oval_Graph::Oval_Graph(const Graph &G, long initAttributes) :
	GraphAttributes(G, initAttributes) {
}

void Oval_Graph::set_oval_templateNodes() {
	m_nodeTemplate.fill(TEMPLATE);
	m_width.fill(20);
	m_height.fill(20);

}

void Oval_Graph::color_node(node *n) {
	m_nodeColor[*n] = "red";

}

void image(int tree, int node_id) {
	char command[LINE];
	char window_name[LINE];
	Tree t = interface->set->trees[tree];
	Graph G;
	node* nodes = new node[t->nodes];
	for (int i = 0; i < t->nodes; i++) {
		nodes[i] = G.newNode(i);
	}
	for (int i = 0; i < t->nodes; i++) {
		int child;
		for (int l = 0; l < t->arity; l++) {
			child = t->get_child(i, l);
			if (child > 0)
				G.newEdge(nodes[i], nodes[child]);

		}
	}
	Oval_Graph GA(G, GraphAttributes::nodeGraphics
			| GraphAttributes::edgeGraphics | GraphAttributes::nodeLabel
			| GraphAttributes::nodeTemplate | GraphAttributes::nodeColor);
	GA.set_oval_templateNodes();
	GA.color_node(&(nodes[node_id]));
	TreeLayout *ohl = new TreeLayout;
	ohl->orientation(bottomToTop);
	ohl->call(GA);
	sprintf(command, "%s/%s", interface->working_dir, TREE_FILENAME);
	GA.writeGML(command);

	sprintf(command, "%s/%s %s/%s %s/%s", interface->tool_path_name,
			GRAPH_COMMAND, interface->working_dir, GRAPH_TREE_FILENAME,
			interface->working_dir, TREE_FILENAME);
	system(command);

	sprintf(command, "%s/%s", interface->working_dir, GRAPH_TREE_FILENAME);
	// Read image
	int width, height, depth;
	double *image_png = read_png(command, &width, &height, &depth);
	if (not image_png) {
		printf("Unable to read tree image\n");
		return;
	}
	// Setup texture
	Data* data = new Data;
	Scene *image_scene = new Scene;
	Texture* image_widget = new Texture;

	data->set(image_png, width, height, depth, GL_DOUBLE, depth
			* sizeof(GLdouble));
	image_widget->set_size(((double) width) / (width + height),
			((double) height) / (width + height), 0);
	image_widget->set_c_data(data);
	image_scene->add(image_widget);
	image_scene->set_zoom(3.25);
	image_scene->set_orientation(0, 0);
	sprintf(window_name,"%s - node %i",interface->set->trees[tree]->get_name(),node_id);
	int image = glutCreateWindow(window_name);
	glutDisplayFunc(image_display);
	glutReshapeFunc(image_reshape);
	glutReshapeWindow(WIN_SIZ, WIN_SIZ);
	glutKeyboardFunc(image_keyboard);
	Image imag = new Image_c;
	imag->win_id = image;
	imag->image = image_scene;
	interface->images->push_front(imag);
	return;
}

Image find_image(int win_id) {
	list<Image>::iterator it = interface->images->begin();
	while (it != interface->images->end() && (*it)->win_id != win_id)
		it++;
	if (it != interface->images->end())
		return *it;
	return NULL;
}

void image_keyboard(unsigned char c, int x, int y) {
	int image = glutGetWindow();
	Image imag = find_image(image);
	if (imag) {
		if (c == 'q') {
			glutDestroyWindow(imag->win_id);
			interface->images->remove(imag);
			imag->~Image_c();
		}
		if (c == 's') {
			char filename[LINE];
			sprintf(filename,"%s/tree_%i.ppm",interface->working_dir,interface->win_save_nonce);
			imag->image->save(filename);
			interface->win_save_nonce++;
		}
	}
}

void image_display(void) {
	int win = glutGetWindow();
	Image image = find_image(win);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	image->image->render();
	glutSwapBuffers();

}

void image_reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glutPostRedisplay();

}
