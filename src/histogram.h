/*
 * histrogram.h
 *
 *  Created on: 13/mar/2011
 *      Author: simonemsr
 */

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "interface.h"
#include "graphic_const.h"

/*
 * Rename a window
 */
void window_rename(int);

/*
 * Histogram button callback. Build and draw the histogram window as new, using current selected datasubset
 */
void histogram() ;

/*
 * Display histogram window
 */
void histogram_display(void) ;

/*
 * Mouse button pressed callback for histogram window
 */
void histogram_on_mouse_button(int button, int state, int x, int y) ;

/*
 * Mouse move callback for histogram window
 */
void histogram_on_mouse_move(int x, int y) ;

/*
 * Mouse pointing callback for histogram window
 */
void histogram_on_mouse_on(int x, int y);

/*
 * Keyboard callback for histogram window
 */
void histogram_keyboard(unsigned char c, int x, int y);



#endif /* HISTOGRAM_H_ */
