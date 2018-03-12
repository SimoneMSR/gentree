/*
 * image.h
 *
 *  Created on: 13/mar/2011
 *      Author: simonemsr
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "interface.h"
#include "graphic_const.h"
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/tree/TreeLayout.h>
#include <ogdf/misclayout/BalloonLayout.h>
#define TEMPLATE "oreas:std:ellipse"
#define PARAM_FILENAME "user_param"
#define TREE_FILENAME "tree.gml"
#define GRAPH_TREE_FILENAME "tree.png"
#define GRAPH_COMMAND "gml2pic -msg-level silent -width 470 -o"


using namespace ogdf;

/*
 * A class for tree layout graphic attributes
 */
class Oval_Graph: public GraphAttributes {
public:
	Oval_Graph(const Graph &G, long initAttributes);
	void set_oval_templateNodes();
	void color_node(node *n);
};

/*
 * Find the Image showed in the specified window
 */
Image find_image(int win_id) ;

/*
 * Build a window containing the layout of specified tree (highlighting the specified node)
 */
void image(int,int);

/*
 * Show a window containing a tree layout
 */
void image_display(void) ;

/*
 * Image window keyboard callback
 */
void image_keyboard(unsigned char c, int x, int y) ;

/*
 * Image window kreshape function
 */
void image_reshape(int width, int height);


#endif /* IMAGE_H_ */
