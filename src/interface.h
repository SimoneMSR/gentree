/*
 * Interface.h
 *
 *  Created on: 27/feb/2011
 *      Author: simonemsr
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "object.h"
#if defined(__APPLE__)
#include <Glut/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GL/glui.h>
#include <cstdlib>
#include "scene.h"
#include "my_cloud.h"
#include "texture.h"
#include "textbox.h"
#include "gram.h"
#include "my_frame.h"
#include "grid_axis.h"
#include "font.h"
#include "Dataset.h"
#include "GTMsd.h"
#include "I_O.h"
#include "em.h"
#include "inference.h"
#include "pthread.h"

class Image_c {
public:
	int win_id;
	Scene *image;
	~Image_c(){image->~Scene();};
};

typedef class Image_c *Image;


class Interface_c {
public:
	const char *filename;
	char *tool_path_name,*working_dir;
	int menu, map, histogrid, histogram,legend; //menu,map,grid,histogram and legend window ids
	bool map_show;
	list<Image> *images; //image window ids and structures
	int cells,curr_epoc,stationary, C, P, epoc, selection_num, num_zoom, num_points, *indexes,
			*ids,*nodes_ids;
int *grid_cell_num;
	int browser_switch,text_editor_switch;
	int win_save_nonce,classes;
	pthread_t thread_train;
	bool *selection,refresh;
	double *grid_origin, *map_origin, *temp_origin; //window origin coordinates
	double *map_data, *grid_data, *histogram_data, *points, *points_avg; //point to be rendered
	double grid_cell_dim, grid_dim,curr_likelyhood;
	float lambda;
	bool error_load_trees;
	P_GTMsd gtm; //current gtm
	Dataset set; //current dataset
	GLUI *path; //paths window
	GLUI *glui; //menu window
	GLUI *browser; //browser window
	GLUI *message; //error message window
	GLUI *text_editor; //window for typing string
	GLUI_Checkbox *checkbox_stationary;
	GLUI_Panel *status_bar,*model_panel,*selection_panel,*visualization_panel;
	GLUI_StaticText *status_line1, *message_error,*status_line2,*status_line3;
	GLUI_FileBrowser *brow;
	GLUI_Button *load_gtm,*save_gtm,*train_gtm;
	GLUI_Button *button_map, *button_grill, *button_histogram,
			*button_set_trees,*button_add_trees, *button_load_dataset;
	GLUI_RadioGroup *group, *roots;
	GLUI_Spinner *tree, *level;
	GLUI_TextBox *enter_string;
	GLUI_EditText *tool_path;
	Scene *map_scene, *histogram_scene, *grid_scene; //scene of the map, histogram and grid window
	Cloud *map_widget,legend_widget;
	Frame *map_axis, *histogram_axis;
	Texture *grid_widget;
	Histogram *histogram_widget;
	Grid_Axis *histogram_scale;
	TextBox*textbox1, *textbox2, *textbox3, *textbox4, *textbox5, *textbox6,*tree_info,*cell_info;

	Interface_c();
	~Interface_c();
};

typedef class Interface_c *Interface;

#endif /* INTERFACE_H_ */
