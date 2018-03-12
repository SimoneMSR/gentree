#include "gui.h"

Interface interface;

Interface_c::Interface_c() {
	map_data = NULL;
	cells = 49;
	grid_dim = 2;
	num_zoom = 0;
	num_points = 0;
	curr_epoc = 0;
	grid_origin = new double[2];
	map_origin = new double[2];
	temp_origin = new double[2];
	tool_path_name = new char[LINE];
	working_dir = new char[LINE];
	images = new list<Image> ;
	win_save_nonce = 0;
	grid_cell_num = NULL;
	gtm = NULL;
	set = NULL;
	gtm = NULL;
	map_show = false;
}

Interface_c::~Interface_c() {
	if (map_scene)
		map_scene->~Scene();
	if (histogram_scene)
		histogram_scene->~Scene();
	if (grid_scene)
		grid_scene->~Scene();
	if (text_editor)
		text_editor->close();
	if (message)
		message->close();
	if (browser)
		browser->close();
	if (glui)
		glui->close();
	if (gtm)
		gtm->~P_GTMsd_c();
	if (set)
		set->~Dataset_c();
	free_vector(selection);
	free_vector(grid_origin);
	free_vector(map_origin);
	free_vector(temp_origin);
	free_vector(map_data);
	free_vector(grid_data);
	free_vector(histogram_data);
	free_vector(points);
	free_vector(points_avg);
	free_vector(ids);
	free_vector(indexes);
}

int save_parameters() {
	ofstream file;
	char filename[LINE];
	sprintf(filename, "%s/%s", interface->working_dir, PARAM_FILENAME);
	file.open(filename, ios::out);
	if (!file.is_open())
		return 1;
	file << interface->stationary << " ";
	file << interface->C << " ";
	file << interface->P << " ";
	file << interface->lambda << " ";
	file << interface->epoc << " ";
	file << interface->cells;
	file.close();
	return 0;
}

void load_parameters() {
	ifstream file;
	char filename[LINE];
	sprintf(filename, "%s/%s", interface->working_dir, PARAM_FILENAME);
	file.open(filename, ios::in);
	if (file.is_open()) {
		file >> interface->stationary;
		file >> interface->C;
		file >> interface->P;
		file >> interface->lambda;
		file >> interface->epoc;
		file >> interface->cells;
		file.close();
		interface->glui->sync_live();
	}
}

void load_gtm() {
	if (interface->gtm)
		interface->gtm->~P_GTMsd_c();
	interface->gtm = new P_GTMsd_c(interface->filename);
	if (interface->gtm->C == -1) {
		message_show(4);
		interface->button_load_dataset->disable();
		interface->save_gtm->disable();
		interface->visualization_panel->disable();
	} else {
		interface->C = interface->gtm->C;
		interface->P = interface->gtm->P;
		interface->lambda = 0;
		interface->epoc = 0;
		interface->button_load_dataset->enable();
		interface->save_gtm->enable();
		interface->button_set_trees->disable();
		interface->button_add_trees->disable();
		interface->button_map->disable();
		interface->button_grill->disable();
		interface->button_histogram->disable();
		interface->status_line2->set_name("GTM");
		interface->status_line1->set_name("LOADED");
		interface->glui->sync_live();
	}
}

void load_set() {
	int L, D, retval, i;
	if (interface->set)
		interface->set->~Dataset_c();
	retval = load_dataset(interface->filename, &(interface->set), &L, &D);
	if (retval != 0 || L > interface->gtm->L || D != interface->gtm->D) {
		delete (interface->set);
		message_show(0);
	} else {
		interface->selection = new bool[interface->set->n];
		for (i = 0; i < interface->set->n; i++)
			interface->selection[i] = false;
		interface->button_set_trees->enable();
		interface->button_add_trees->enable();
	}
}

int save_gtm(const char *name) {
	chdir(interface->working_dir);
	return interface->gtm->save(name);
}
void browser_show(int i) {
	interface->brow->fbreaddir(".");
	interface->browser->show();
	interface->browser_switch = i;
}

void browser_fun(int i) {
	switch (i) {
	case 0: {
		//function for double click button
		if (chdir(interface->brow->get_file()) == 0) {
			interface->brow->fbreaddir(".");
			if (interface->browser_switch == 4) {
				//browser is used for tree drawing tool path
				char path[LINE];
				getcwd(path, LINE);
				interface->tool_path->set_text(path);
			}
		} else {
			interface->browser->hide();
			interface->filename = interface->brow->get_file();
			switch (interface->browser_switch) {
			case 0:
				train();
				break;
			case 1:
				load_gtm();
				break;
			case 2:
				load_set();
				break;
			case 3: {
				//function for text_ediror (used for tree list)
				if (interface->text_editor_switch != 2)
					//browser is not used for gtm saving
					interface->text_editor_switch = 1;
				text_editor_fun();
				interface->text_editor->hide();
			}
				break;
			}
		}
	}
		break;
	case 1: {
		//function for BACK button
		chdir("../");
		interface->brow->fbreaddir(".");
	}
		break;
	case 2: {
		//function for HOME button
		chdir("/");
		interface->brow->fbreaddir(".");
	}
		break;
	case 3:
	case 4: {
		//function for ESC and DONE button
		//function for text_ediror (used for gtm saving)
		interface->browser->hide();
	}
		break;
	case 5: {
		//function for working dir button
		chdir(interface->working_dir);
		interface->brow->fbreaddir(".");
	}
		break;
	}
}

void message_show(int m) {
	switch (m) {
	case 0:
		interface->message_error->set_text("Invalid dataset");
		break;
	case 1:
		interface->message_error->set_text("Invalid seletcion");
		break;
	case 2:
		interface->message_error->set_text(
				"Invalid parameters. All values must be positive. Hidden states and P must be perfect square");
		break;
	case 3:
		interface->message_error->set_text(
				"Invalid parameter. Number of cells must be a perfect square");
		break;
	case 4:
		interface->message_error->set_text("Not a gtm file");
		break;
	case 5:
		interface->message_error->set_text("Error for gtm saving");
		break;
	case 6:
		interface->message_error->set_text("Save ok");
		break;
	case 7:
		interface->message_error->set_text("Invalid tool path");
		break;
	case 8:
		interface->message_error->set_text(
				"Training thread is not avaiable. Try later.");
	}
	interface->message->refresh();
	interface->message->show();

}

void message_hide() {
	interface->message->hide();
}

void text_editor_show(int i) {
	interface->text_editor_switch = i;
	interface->text_editor->show();
}

void text_editor_hide() {
	interface->text_editor->hide();
}

void text_editor_fun() {
	int num_trees, *trees;
	switch (interface->text_editor_switch) {
	case 0: {
		//use current string as a list of tree names
		interface->error_load_trees = select_trees_string(interface->set,
				&num_trees, &trees, interface->enter_string->get_text());
	}
		break;
	case 1: {
		//use current string as a filename containing a list of tree names
		interface->error_load_trees = select_trees_file(interface->set,
				&num_trees, &trees, interface->filename);
	}
		break;
	case 2: {
		//use current string as the filename for gtm saving
		if (save_gtm(interface->enter_string->get_text()) != 0)
			message_show(5);
		else
			message_show(6);
	}
		break;
	}
	if (interface->text_editor_switch <= 1) {
		//the gui is used for tree selection
		if (interface->error_load_trees) {
			message_show(1);
			interface->button_map->disable();
			interface->button_grill->disable();
			interface->button_histogram->disable();
			interface->error_load_trees = false;
		} else {
			project_points(num_trees, trees);
			delete (trees);
		}
	}
	interface->text_editor->hide();
}

void select_points(int i) {
	int num_trees, *trees;
	interface->refresh = i;
	switch (interface->group->get_int_val()) {
	case 0: {
		//select all trees
		num_trees = interface->set->n;
		trees = new int[num_trees];
		for (int i = 0; i < interface->set->n; i++)
			trees[i] = i;
		project_points(num_trees, trees);
		delete (trees);
	}
		break;
	case 1: {
		//select or add the trees specified in the textbox
		text_editor_show(0);
	}
		break;
	case 2: {
		//select one tree
		if (interface->tree->get_int_val() >= interface->set->n) {
			message_show(1);
			return;
		}
		num_trees = 1;
		trees = new int[1];
		trees[0] = interface->tree->get_int_val();
		project_points(num_trees, trees);
		delete (trees);
	}
		break;

	}

}

void project_points(int trees, int*selection) {
	if (interface->refresh) {
		free_vector(interface->indexes);
		free_vector(interface->ids);
		free_vector(interface->nodes_ids);
		free_vector(interface->points);
		free_vector(interface->points_avg);
	}
	switch (interface->roots->get_int_val()) {

	case 0: {
		//render all roots of selected trees
		interface->error_load_trees = project_roots(interface->refresh,
				interface->gtm, interface->set, trees, selection,
				&(interface->indexes), &(interface->ids),
				&(interface->nodes_ids), &(interface->num_points),
				&(interface->points), &(interface->points_avg));

	}
		break;
	case 1: {

		interface->error_load_trees = project_all(interface->refresh,
				interface->gtm, interface->set, trees, selection,
				&(interface->indexes), &(interface->ids),
				&(interface->nodes_ids), &(interface->num_points),
				&(interface->points), &(interface->points_avg));

	}
		break;
	case 2: {
		//render all nodes in the specified level of selected trees
		interface->error_load_trees = project_level(interface->refresh,
				interface->gtm, interface->set,
				interface->level->get_int_val(), trees, selection,
				&(interface->indexes), &(interface->ids),
				&(interface->nodes_ids), &(interface->num_points),
				&(interface->points), &(interface->points_avg));
	}
		break;

	}

	if (interface->num_points >= 1 && !interface->error_load_trees) {
		//if no errors
		interface->visualization_panel->enable();
	} else {
		message_show(1);
		interface->error_load_trees = false;
		interface->visualization_panel->disable();
	}

}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glutPostRedisplay();
}

void quit() {
	interface->~Interface_c();
	exit(0);
}

void void_fun() {
	return;
}

void window_rename(int win) {
	char name[NAME_LEN];
	name[0] = '\0';
	if (win == interface->map)
		sprintf(name, "%s", MAP_NAME);
	if (win == interface->histogrid)
		sprintf(name, "%s", GRID_NAME);
	if (win == interface->histogram)
		sprintf(name, "%s", HISTOGRAM_NAME);
	if (interface->refresh == 0) {
		sprintf(name, "%s - %s", name, SELECTION_MIXED);
	} else {
		switch (interface->roots->get_int_val()) {
		case 0:
			sprintf(name, "%s - %s", name, SELECTION_ROOTS);
			break;
		case 1:
			sprintf(name, "%s - %s", name, SELECTION_NODES);
			break;
		case 2:
			sprintf(name, "%s - %s: %i", name, SELECTION_LEVEL,
					interface->level->get_int_val());
		}
	}
	glutSetWindow(win);
	glutSetWindowTitle(name);
}


#ifndef GENTREE_STABLE

void train() {
	int retval;
	pthread_create(&(interface->thread_train), NULL, thread_train, &retval);
	pthread_join(interface->thread_train, NULL);
}
void* thread_train(void*) {


#else

	void train(){
#endif
	interface->curr_epoc = 0;
	if (interface->gtm)
		interface->gtm->~P_GTMsd_c();
	if (interface->set)
		interface->set->~Dataset_c();
	if (interface->C <= 0 || interface->P <= 0 || interface->lambda <= 0
			|| interface->epoc <= 0 || not is_square(interface->C)
			|| not is_square(interface->P)) {
		interface->visualization_panel->disable();
		interface->button_load_dataset->disable();
		interface->save_gtm->disable();
		message_show(2);
	} else {
		if (train_gtm(interface->C, interface->P, interface->lambda,
				interface->checkbox_stationary->get_int_val(), interface->epoc,
				0, &(interface->gtm), &(interface->set), &interface,
				interface->filename) == 0) {
			//no errors during training
			interface->selection = new bool[interface->set->n];
			interface->button_load_dataset->enable();
			interface->selection_panel->enable();
			interface->button_set_trees->enable();
			interface->button_add_trees->enable();
			interface->save_gtm->enable();
			interface->visualization_panel->enable();
			interface->status_line1->set_name("TRAINING");
			interface->status_line2->set_name("COMPLETED");
			interface->status_line3->set_name("");
			save_gtm(GTM_STD_FILENAME);
			save_parameters();
		} else {
			interface->button_load_dataset->disable();
			interface->save_gtm->disable();
			interface->visualization_panel->disable();
			message_show(0);
		}
	}

#ifndef GENTREE_STABLE
	return NULL;
#endif
}


void path() {
	char command[LINE];
	sprintf(command, "%s/gml2pic -h", interface->tool_path->get_text());
	if (system(command)) {
		message_show(7);
	} else {
		interface->load_gtm->enable();
		interface->train_gtm->enable();
		sprintf(interface->tool_path_name, "%s",
				interface->tool_path->get_text());
		interface->path->hide();
	}
}

int gui(int argc, char **argv) {
	interface = new Interface_c();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//CRONTROL PANEL BUILDING

	GLUI *glui = GLUI_Master.create_glui("Menu");
	interface->glui = glui;
	interface->menu = glui->get_glut_window_id();
	glutSetWindow(interface->menu);
	glutSetColor(0, 1, 0.1, 0.1);
	GLUI_Panel *panel = glui->add_panel("", 0);
	GLUI_Master.set_glutIdleFunc(void_fun);
	GLUI_Panel *panel1 = interface->model_panel = glui->add_panel_to_panel(
			panel, "", 0);
	glui->add_statictext_to_panel(panel1, "DATASET");
	interface->button_load_dataset = glui->add_button_to_panel(panel1,
			"load dataset", 2, (GLUI_Update_CB) browser_show);
	glui->add_separator_to_panel(panel1);
	glui->add_statictext_to_panel(panel1, "GTM");

	glui->add_spinner_to_panel(panel1, "hidden states:", GLUI_SPINNER_INT,
			&(interface->C))->set_speed(0.1);
	glui->add_spinner_to_panel(panel1, "P:", GLUI_SPINNER_INT, &(interface->P))->set_speed(
			0.1);
	glui->add_spinner_to_panel(panel1, "lambda:", GLUI_SPINNER_FLOAT,
			&(interface->lambda))->set_speed(0.1);
	glui->add_spinner_to_panel(panel1, "eras:", GLUI_SPINNER_INT,
			&(interface->epoc))->set_speed(0.1);
	interface->checkbox_stationary = glui->add_checkbox_to_panel(panel1,
			"stationary", &(interface->stationary));
	interface->checkbox_stationary->set_alignment(GLUI_ALIGN_RIGHT);
	interface->train_gtm = glui->add_button_to_panel(panel1, "train new GTM",
			0, (GLUI_Update_CB) browser_show);
	interface->status_bar = glui->add_panel_to_panel(panel1, "training status",
			1);
	interface->status_line1 = glui->add_statictext_to_panel(
			interface->status_bar, "");
	interface->status_line2 = glui->add_statictext_to_panel(
			interface->status_bar, "");
	interface->status_line3 = glui->add_statictext_to_panel(
			interface->status_bar, "");
	interface->load_gtm = glui->add_button_to_panel(panel1, "load GTM", 1,
			(GLUI_Update_CB) browser_show);
	interface->save_gtm = glui->add_button_to_panel(panel1, "save GTM", 2,
			(GLUI_Update_CB) text_editor_show);
	glui->add_column_to_panel(panel, true);
	GLUI_Panel *panel2 = interface->selection_panel = glui->add_panel_to_panel(
			panel, "", 0);
	glui->add_statictext_to_panel(panel2, "Projected information");
	interface->group = glui->add_radiogroup_to_panel(panel2);
	glui->add_radiobutton_to_group(interface->group, "all");
	glui->add_radiobutton_to_group(interface->group, "list of trees");
	glui->add_radiobutton_to_group(interface->group, "one tree");
	interface->tree = glui->add_spinner_to_panel(panel2, "tree:",
			GLUI_SPINNER_INT);
	interface->tree ->set_speed(0.1);
	glui->add_separator_to_panel(panel2);
	interface->roots = glui->add_radiogroup_to_panel(panel2);
	glui->add_radiobutton_to_group(interface->roots, "roots");
	glui->add_radiobutton_to_group(interface->roots, "all nodes");
	glui->add_radiobutton_to_group(interface->roots, "level");
	interface->level = glui->add_spinner_to_panel(panel2, "level:",
			GLUI_SPINNER_INT);
	interface->level->set_speed(0.1);
	glui->add_column_to_panel(panel, true);
	GLUI_Panel *panel3 = interface->visualization_panel
			= glui->add_panel_to_panel(panel, "", 0);
	glui->add_statictext_to_panel(panel3, "Visualization modality");
	interface->button_map = glui->add_button_to_panel(panel3, "Project", 0,
			(GLUI_Update_CB) project);
	GLUI_Panel *p = glui->add_panel_to_panel(panel3, "");
	interface->button_grill = glui->add_button_to_panel(p, "Histogrid", 0,
			(GLUI_Update_CB) histogrid);
	glui->add_spinner_to_panel(p, "cells", GLUI_SPINNER_INT,
			&(interface->cells));
	interface->button_histogram = glui->add_button_to_panel(panel3,
			"Histogram", 0, (GLUI_Update_CB) histogram);
	glui->add_separator_to_panel(panel3);
	interface->button_set_trees = glui->add_button_to_panel(panel3, "set map",
			1, (GLUI_Update_CB) select_points);
	interface->button_add_trees = glui->add_button_to_panel(panel3,
			"project new trees", 0, (GLUI_Update_CB) select_points);
	glui->add_checkbox_to_panel(panel3, "classes", &(interface->classes));
	glui->add_button("Quit", 0, (GLUI_Update_CB) quit);
	interface->train_gtm->disable();
	interface->load_gtm->disable();
	interface->save_gtm->disable();
	interface->button_load_dataset->disable();
	interface->button_set_trees->disable();
	interface->button_add_trees->disable();
	interface->button_map->disable();
	interface->button_grill->disable();
	interface->button_histogram->disable();

	//MESSAGE WINDOW BUILDING

	interface->message = GLUI_Master.create_glui("Message", 0, 300, 300);
	interface->message->hide();
	interface->message->add_statictext("");
	interface->message_error = interface->message->add_statictext("");
	interface->message_error->set_h(40);
	interface->message->add_button("OK", 0, (GLUI_Update_CB) message_hide);

	//BROWSER WINDOW BUILDING

	interface->browser = GLUI_Master.create_glui("Browser", 0, 600, 100);
	interface->browser->hide();
	interface->brow = new GLUI_FileBrowser(interface->browser, "", false, 0,
			(GLUI_Update_CB) browser_fun);
	interface->browser->add_button("DONE", 3, (GLUI_Update_CB) browser_fun);
	interface->browser->add_button("ESC", 3, (GLUI_Update_CB) browser_fun);
	interface->browser->add_column(true);
	interface->browser->add_button("back", 1, (GLUI_Update_CB) browser_fun);
	interface->browser->add_button("home", 2, (GLUI_Update_CB) browser_fun);
	interface->browser->add_button("working dir", 5,
			(GLUI_Update_CB) browser_fun);

	//MAP WINDOW BUILDING

	interface->map = glutCreateWindow("Map");
	glutHideWindow();
	glutReshapeFunc(reshape);
	glutDisplayFunc(map_display);
	glutMouseFunc(map_on_mouse_button);
	glutMotionFunc(map_on_mouse_move);
	glutPassiveMotionFunc(map_on_mouse_on);
	glutKeyboardFunc(map_keyboard);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glutReshapeWindow(WIN_SIZ, WIN_SIZ);
	interface->map_widget = CloudPtr(new Cloud());
	interface->map_widget->set_thickness(0.5);
	interface->map_axis = FramePtr(new Frame());
	interface->map_axis->set_bg_color(0, 0, 0, 0);
	interface->map_axis->set_position(0, 0, 0, 0);
	interface->map_scene = ScenePtr(new Scene);
	interface->map_scene->set_ortho_mode(true);
	interface->map_scene->set_size(MAP_SIZ, MAP_SIZ, 0);
	interface->map_scene->set_max_zoom(MAX_ZOOM);
	interface->map_scene->set_bg_color(0, 0, 0, 0);
	interface->map_scene->add(interface->map_widget);
	interface->map_scene->add(interface->map_axis);
	interface->tree_info = TextBoxPtr(new TextBox);
	interface->tree_info->set_bg_color(0, 0, 0, 0);
	interface->tree_info->set_fontsize(1);
	interface->map_scene->add(interface->tree_info);


	//HISTOGRID WINDOW BUILDING

	interface->histogrid = glutCreateWindow("Grid");
	glutHideWindow();
	glutDisplayFunc(histogrid_display);
	glutReshapeWindow(WIN_SIZ_Y, WIN_SIZ);
	glutMouseFunc(histogrid_on_mouse_button);
	glutPassiveMotionFunc(histogrid_on_mouse_on);
	glutKeyboardFunc(histogrid_keyboard);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	interface->grid_scene = new Scene;
	interface->textbox1 = TextBoxPtr(new TextBox);
	interface->textbox2 = TextBoxPtr(new TextBox);
	interface->textbox3 = TextBoxPtr(new TextBox);
	interface->textbox4 = TextBoxPtr(new TextBox);
	interface->textbox5 = TextBoxPtr(new TextBox);
	interface->textbox6 = TextBoxPtr(new TextBox);
	interface->textbox1->set_position(GRI_TEXT_OFF_X, 10, 0);
	interface->textbox2->set_position(GRI_TEXT_OFF_X + GRI_SIZ / 2, 10, 0);
	interface->textbox3->set_position(GRI_TEXT_OFF_X / 2 + GRI_SIZ, 10, 0);
	interface->textbox4->set_position(GRI_TEXT_OFF_X + GRI_SIZ + GRI_MARGIN,
			GRI_TEXT_OFF_Y, 0);
	interface->textbox5->set_position(GRI_TEXT_OFF_X + GRI_SIZ + GRI_MARGIN,
			GRI_TEXT_OFF_Y + GRI_SIZ / 2, 0);
	interface->textbox6->set_position(GRI_TEXT_OFF_X + GRI_SIZ + GRI_MARGIN,
			GRI_TEXT_OFF_Y + GRI_SIZ, 0);
	interface->textbox1->set_bg_color(0, 0, 0, 0);
	interface->textbox2->set_bg_color(0, 0, 0, 0);
	interface->textbox3->set_bg_color(0, 0, 0, 0);
	interface->textbox4->set_bg_color(0, 0, 0, 0);
	interface->textbox5->set_bg_color(0, 0, 0, 0);
	interface->textbox6->set_bg_color(0, 0, 0, 0);

	interface->grid_scene->add(interface->textbox1);
	interface->grid_scene->add(interface->textbox2);
	interface->grid_scene->add(interface->textbox3);
	interface->grid_scene->add(interface->textbox4);
	interface->grid_scene->add(interface->textbox5);
	interface->grid_scene->add(interface->textbox6);

	interface->grid_widget = TexturePtr(new Texture);
	interface->grid_scene->set_bg_color(0, 0, 0, 0);
	interface->grid_scene->set_position(GRI_REL_OFF_X, GRI_REL_OFF_Y, 0);
	interface->grid_scene->set_zoom(3);
	interface->grid_scene->set_orientation(0, 0);
	interface->grid_scene->add(interface->grid_widget);
	interface->cell_info = TextBoxPtr(new TextBox);
	interface->cell_info->set_bg_color(0, 0, 0, 0);
	interface->cell_info->set_fontsize(1);
	interface->grid_scene->add(interface->cell_info);

	interface->text_editor = GLUI_Master.create_glui("");
	interface->text_editor->hide();
	interface->enter_string = new GLUI_TextBox(interface->text_editor, true);
	interface->text_editor->add_button("OK", 0,
			(GLUI_Update_CB) text_editor_fun);
	interface->text_editor->add_button("browse", 3,
			(GLUI_Update_CB) browser_show);
	interface->text_editor->add_button("ESC", 0,
			(GLUI_Update_CB) text_editor_hide);
	interface->enter_string->set_w(200);
	interface->enter_string->set_h(200);
	glutReshapeFunc(histogrid_reshape);

	//HISTOGRAM WINDOW BUILDING

	interface->histogram = glutCreateWindow("Histogram");
	glutHideWindow();
	glutReshapeWindow(WIN_SIZ, WIN_SIZ);
	glutReshapeFunc(reshape);
	glutDisplayFunc(histogram_display);
	glutMouseFunc(histogram_on_mouse_button);
	glutMotionFunc(histogram_on_mouse_move);
	glutKeyboardFunc(histogram_keyboard);
	interface->histogram_scale = new Grid_Axis();
	interface->histogram_widget = HistogramPtr(new Histogram());
	interface->histogram_widget->set_colormap(Colormap::Red());
	interface->histogram_axis = FramePtr(new Frame());
	interface->histogram_axis->set_position(0, 0, 0, 0);
	interface->histogram_scene = ScenePtr(new Scene);
//	interface->histogram_scene->add(interface->histogram_scale);
	interface->histogram_scene->add(interface->histogram_axis);
	interface->histogram_scene->add(interface->histogram_widget);

	interface->histogram_scene->set_size(2000, 2000, 2000);
	interface->histogram_scene->set_zoom(0.7);
	interface->histogram_scene->set_position(-750, -750, 0);
	Vec4f ori = Vec4f();
	ori.x = 90;
	ori.y = 90;
	ori.z = 0;
	interface->histogram_scene->set_orientation(ori);

	getcwd(interface->working_dir, LINE);
	if (chdir(WORKING_DIR)) {
		mkdir(WORKING_DIR, 0777);
		chdir(WORKING_DIR);
	}
	sprintf(interface->working_dir, "%s/%s", interface->working_dir,
			WORKING_DIR);
	if (system("./gml2pic -h")) {
		interface->path = GLUI_Master.create_glui("Insert path", 0, 300, 300);
		interface->path->add_statictext(
				"Please specify tree drawing tool absolute path ")->set_h(20);
		interface->tool_path = interface->path->add_edittext("path",
				GLUI_EDITTEXT_TEXT, NULL);
		interface->tool_path->set_w(400);
		interface->path->add_button("OK", 0, (GLUI_Update_CB) path);
		interface->path->add_button("browse", 4, (GLUI_Update_CB) browser_show);
	} else {
		interface->load_gtm->enable();
		interface->train_gtm->enable();
		getcwd(interface->tool_path_name, LINE);
	}
	load_parameters();
	glutMainLoop();

	return 0;
}

