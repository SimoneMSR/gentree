/*
 * grid.h
 *
 *  Created on: 13/mar/2011
 *      Author: simonemsr
 */

#ifndef GRID_H_
#define GRID_H_
#include "interface.h"
#include "graphic_const.h"
#include "data.h"

/*
 * Scale x and y in [-1,1]
 */
void scale(double x, double y, double xy[2]);

/*
 * Scale x and y in [0,GRID_SIZ]
 */
void scale_inverse(double x, double y, double xy[2]);

/*
 * Store in center the center of i-th region of histogrid
 */
void center(int i, double center[2]);

/*
 *Return the id of region of histogrid containing the point (x,y). x and y are in [-1;1]
 */
int is_in(int num_cell, double x, double y) ;

/*
 *Return the id of region of histogrid containing the point (x,y). x and y are in [0;GRID_SIZ]
 */
int in(double x, double y);

/*
 * Return true if and only if (x,y) is contained in the histogrid
 */
bool content(double x, double y) ;

/*
 * Histogrid button callback. Build and draw the histogrid window as new, using current selected datasubset
 */
void histogrid();

/*
 * Display histogrid window
 */
void histogrid_display(void);

/*
 * Mouse button pressed callback for histogrid window
 */
void histogrid_on_mouse_button(int button, int state, int x, int y) ;

/*
 * Mouse pointing callback for histogrid window
 */
void histogrid_on_mouse_on(int x, int y);

/*
 * Keyboard callback for histogrid window
 */
void histogrid_keyboard(unsigned char c, int x, int y);

/*
 * Reshape histogrid window
 */
void histogrid_reshape(int x, int y);



/*
 * Redraw histogrid projection zone associated to the point (o_x,o_y)
 */
void histogrid_zoom_in(double o_x, double o_y) ;

/*
 * Redraw hisogrid projection zone with a zoom-out
 */
void histogrid_zoom_out();

/*
 * Show the message window
 */
void message_show(int);

/*
 * Rename a window
 */
void window_rename(int);

/*
 * Syncronize histogrid and map window projection zone
 */
void map_syncronize();


#endif /* GRID_H_ */
