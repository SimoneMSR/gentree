/*
 * gui.h
 *
 *  Created on: 27/feb/2011
 *      Author: simonemsr
 */
#include <sys/stat.h>
#include <sys/types.h>
#include "interface.h"
#include "train.h"
#include "map.h"
#include "image.h"
#include "grid.h"
#include "histogram.h"
#include "graphic_const.h"
#define GTM_STD_FILENAME "gtmtmp"
#define MAP_NAME "Map"
#define GRID_NAME "Grid"
#define HISTOGRAM_NAME "Histogram"
#define SELECTION_ROOTS "roots"
#define SELECTION_NODES "all nodes"
#define SELECTION_LEVEL "level"
#define SELECTION_MIXED "mixed"
#define NAME_LEN 50
#define WORKING_DIR "TMP_t_w_d"

/*
 * Load_gtm button callback. Load a GTMsd for projection from a file
 */
void load_gtm();

/*
 * Save_gtm button callback. Save the trained GTMsd in a file
 */
int save_gtm(const char*);

/*
 * Train_gtm button callbacks. Train a new GTMsd
 */
void *thread_train(void *a);
void train();

/*
 * Load_dataset button callback. Load a dataset from file
 */
void load_set();

/*
 * Empty callback.
 */
void myGlutIdle(void) ;
void void_fun();

/*
 *Select_trees button callback. Define the projection subset of nodes in the dataset
 */
void select_points(int);

/*
 * Project the selected subset of nodes
 */
void project_points(int,int*);

/*
 * Quit button callback. Close the application
 */
void quit() ;

/*
 * Text editor callback.
 */
void text_editor_fun() ;

/*
 * Show the text editor window
 */
void text_editor_show(int);

/*
 * Update the training_panel during the training of a GTM
 */
void status() ;

/*
 * Show/hide the message window
 */
void message_show(int m);
void message_hide();

/*
 * Reshape a window
 */
void reshape(int width, int height) ;

/*
 * Rename a window
 */
void window_rename(int i);

/*
 * Build and launch the control panel graphic interface
 */
int gui(int argc, char **argv) ;
