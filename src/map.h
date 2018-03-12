/*
 * map.h
 *
 *  Created on: 13/mar/2011
 *      Author: simonemsr
 */

#ifndef __MAP_H__
#define __MAP_H__

#include "interface.h"
#include "image.h"
#include "graphic_const.h"
#define MOUSE_POINT_TOLLERANCE RANGE

/*
 * Rename a window
 */
void window_rename(int);

/*
 * Project button callback. Build and draw the map projection window as new, using current selected datasubset
 */
void project() ;

/*
 * Show the map window
 */
void map_display(void) ;



/*
 * Mouse button pressed callback for map window
 */
void map_on_mouse_button(int button, int state, int x, int y) ;

/*
 * Mouse move callback for map window
 */
void map_on_mouse_move(int x, int y) ;

/*
 * Mouse pointing callback for map window
 */
void map_on_mouse_on(int x, int y);

/*
 * Keyboard callback for map window
 */
void map_keyboard(unsigned char c, int x, int y);

/*
 * Identify the node projected in (x,y)
 */
int select_tree(double x, double y,int *id,int *node_id);

/*
 * Syncronize histogrid and map window projection zone
 */
void map_syncronize();




#endif
