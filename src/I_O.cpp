/*
 * I_O.cpp
 *
 *  Created on: 08/nov/2010
 *      Author: simonemsr
 */

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "GTMsd.h"
#include "Dataset.h"
#include "I_O.h"
#include "errors.h"

int get_format(ifstream *file) {
	char line[FORMAT_L + 2];
	*file >> line;
	if (strcmp(line, FORMAT))
		return 0;
	return 1;
}

int get_indegree(ifstream *file) {
	char line[F_IN_L + 2];
	*file >> line;
	if (strcmp(line, F_IN))
		return 0;
	*file >> line;
	return 1;
}

int get_outdegree(ifstream *file) {
	char line[F_OUT_L + 2];
	int ret;
	*file >> line;
	if (strcmp(line, F_OUT))
		return 0;
	*file >> ret;
	return ret;
}

int get_dimlabel(ifstream *file) {
	char line[F_DIM_L + 2];
	int ret;
	*file >> line;
	if (strcmp(line, F_DIM))
		return 0;
	*file >> ret;
	return ret;
}

int isBlank(char *s) {
	return s[0] == '\0' || s[0] == '\n' || s[0] == ' ';
}

int isEqual(char *s, const char *r) {
	return strcmp(s, r) == 0;
}

void read_header(ifstream *file) {
	char line[LINE];
	get_format(file);
	get_indegree(file);
	get_outdegree(file);
	get_dimlabel(file);
	*file >> line;
	*file >> line;
}

int count_children(ifstream *file) {
	int count = 0;
	char line[LINE];
	file->getline(line, LINE);

	while (isBlank(line))
		file->getline(line, LINE);
	while (!isBlank(line)) {
		count++;
		file->getline(line, LINE);
	}
	return count;
}

int count_trees(const char *filename, int **nums) {
	ifstream file;
	file.open(filename, ios::in);
	char line[LINE];
	int count = 0;
	if (!get_format(&file))
		//error
		return ERR_FORMAT;
	if (!get_indegree(&file))
		//error
		return ERR_FORMAT;
	if (!get_outdegree(&file))
		//error
		return ERR_FORMAT;
	if (!get_dimlabel(&file))
		//error
		return ERR_FORMAT;

	//number of tree header
	file >> line;

	if (!isEqual(line, F_NUM))
		//error
		return ERR_FORMAT;

	//number of tree
	file >> line;
	//read eventual blank lines
	file >> line;
	while (isBlank(line))
		file >> line;

	/*
	 *
	 * END OF FILE HEADER
	 *
	 */

	// start counting trees
	while (!file.eof()) {
		if (line[0] == 'g') //line is a tree header
			count++;
		file.getline(line, LINE);
		while (!file.eof() && (isBlank(line) || line[0] != 'g')) //read lines until a new tree header
			file.getline(line, LINE);
	}
	file.close();
	file.open(filename, ios::in);
	read_header(&file);
	int i = 0;
	*nums = new int[count];
	while (i < count && !file.eof()) {
		file.getline(line, LINE);
		while (isBlank(line))
			file.getline(line, LINE);
		(*nums)[i] = count_children(&file);
#ifdef CONTROL
		sscanf(line,"%*s %*s %i",&children_control);
		if((*nums)[i]!=children_control) {
			delete(*nums);
			return ERR_FILE;
		}
#endif
		i++;
	}
	if (i != count)
		return ERR_FILE;
	return count;
}

int read_child(ifstream *file) {
	char line[4];
	*file >> line;

	if (line[0] == '-')
		return -1;
	else
		return atoi(line);
}

int load_dataset(const char *filename, Dataset *dat, int *retval_l,
		int *retval_d) {
	Tree current;
	int i, n, N, l, d, D, L, node, child, *nums, num_nodes;
	double label;
	char line[1024];
	ifstream file;
	n = count_trees(filename, &nums);
	if (n < 0)
		return n;
	else {
		file.open(filename, ios::in);
		get_format(&file);
		get_indegree(&file);
		L = get_outdegree(&file);
		D = get_dimlabel(&file);
		file >> line;
		file >> N;
#ifdef CONTROL
		if (N != n)
		//error, inconsistent data
		return ERR_FILE;
#endif

		Dataset data = new Dataset_c(n);
		for (i = 0; i < n; i++) {
			//read declaration of tree header
			file >> line;
			while (isBlank(line))
				file >> line;
			//read tree name and number of children
			file >> line >> node;
			data->trees[i] = new Tree_c(nums[i], L, D);
			current = data->trees[i];
			current->name = strdup(line);
			for (num_nodes = 0; num_nodes < current->nodes; num_nodes++) {
				//read node id
				file >> node;
				for (d = 0; d < D; d++) {
					file >> label;
					current->set_label_field(node, d, label);
				}

				for (l = 0; l < L; l++) {
					child = read_child(&file);
					current->set_child(node, l, child);
				}
				file.getline(line, LINE);

				if (num_nodes == current->nodes - 1)
					current->label = atof(line);

			}
			current->topologic_sort(L, D);
		}
		if (dat)
			*dat = data;
		if (retval_l)
			*retval_l = L;
		if (retval_d)
			*retval_d = D;
		return 0;
	}
}

void save_dataset(int D,int L, Dataset data, char* filename) {
	ofstream file;
	Tree current;
	int i, node, l, d, child;
	file.open(filename, ios::out);
	file << FORMAT << "\n";
	file << F_IN << " 0\n";
	file << F_OUT << " " << L << " " << F_DIM << " " << D << "\n";
	file << F_NUM << " " << data->n << "\n\n";

	for (i = 0; i < data->n; i++) {
		current = data->trees[i];
		file << GRAP << " " << current->name << " " << current->nodes << "\n";
		for (node = current->nodes - 1; node >= 0; node--) {
			file << node << " ";
			for (d = 0; d < D; d++)
				file << current->get_label_field(node, d) << " ";
			for (l = 0; l < L; l++) {
				child = current->get_child(node, l);
				if (child == -1)
					file << "- ";
				else
					file << child << " ";
			}
			if (node == 0)
				file << current->label << "\n";
			else
				file << "*\n";
			file << "\n";
		}
	}
	file.close();
}


int select_trees_file(Dataset d, int*num, int **sel, const char *filename) {
	int tree, trees = 0, extisting_trees = 0, count = 0;
	char c, name[LINE];
	ifstream file;
	file.open(filename, ios::in);
	if (not file.is_open())
		return 1;
	file >> c;
	while (!file.eof()) {
		if (c == ';')
			trees++;
		file >> c;
	}
	if (trees == 0)
		return 1;
	extisting_trees = trees;
	file.close();
	file.open(filename, ios::in);
	for (int i = 0; i < trees; i++) {
		file >> name;
		tree = d->search_tree(name);
		if (tree < 0)
			extisting_trees--;
		file >> name;
	}
	*num = extisting_trees;
	if (*num == 0)
		return 1;
	*sel = new int[extisting_trees];
	file.close();
	file.open(filename, ios::in);
	for (int i = 0; i < trees; i++) {
		file >> name;
		tree = d->search_tree(name);
		if (tree >= 0)
			(*sel)[count++] = tree;
		file >> name;
	}
	return 0;
}

int select_trees_string(Dataset d, int*num, int **sel, const char *tree_list) {
	int tree, trees = 0, extisting_trees = 0, count = 0;
	char c, name[LINE];
	istringstream file(tree_list);
	if (not file)
		return 1;
	file >> c;
	while (!file.eof()) {
		if (c == ';')
			trees++;
		file >> c;
	}
	if (trees == 0)
		return 1;
	extisting_trees = trees;
	istringstream file2(tree_list);
	for (int i = 0; i < trees; i++) {
		file2 >> name;
		tree = d->search_tree(name);
		if (tree < 0)
			extisting_trees--;
		file2 >> name;
		if (strcmp(name, ";") != 0)
			return 1;
	}
	*num = extisting_trees;
	if (*num == 0)
		return 1;
	*sel = new int[extisting_trees];
	istringstream file3(tree_list);
	for (int i = 0; i < trees; i++) {
		file3 >> name;
		tree = d->search_tree(name);
		if (tree >= 0)
			(*sel)[count++] = tree;
		file3 >> name;
	}
	return 0;
}

double *read_png(std::string filename, int *width, int *height, int *depth) {
	*width = 0;
	*height = 0;
	*depth = 0;

	/* Open image file */
	FILE *fp = fopen(filename.c_str(), "rb");
	if (!fp)
		return 0;

	/* Read magic number */
	png_byte magic[8];
	int res = fread(magic, 1, sizeof(magic), fp);
	if (not res) {
	}
	/* Check for valid magic number */
	if (!png_check_sig(magic, sizeof(magic))) {
		fclose(fp);
		return 0;
	}

	/* Create a png read struct */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
			NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		return 0;
	}

	/* Create a png info struct */
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	/* Initialize the setjmp for returning properly after a libpng
	 error occured */
	if (setjmp (png_jmpbuf (png_ptr))) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 0;
	}

	/* Setup libpng for using standard C fread() function
	 with our FILE pointer */
	png_init_io(png_ptr, fp);

	/* Tell libpng that we have already read the magic number */
	png_set_sig_bytes(png_ptr, sizeof(magic));

	/* Read png info */
	png_read_info(png_ptr, info_ptr);

	int bit_depth, color_type;

	/* Get some usefull information from header */
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	/* Convert index color images to RGB images */
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	/* Convert 1-2-4 bits grayscale images to 8 bits
	 grayscale. */
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_gray_1_2_4_to_8(png_ptr);

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	if (bit_depth == 16)
		png_set_strip_16(png_ptr);
	else if (bit_depth < 8)
		png_set_packing(png_ptr);

	/* Update info structure to apply transformations */
	png_read_update_info(png_ptr, info_ptr);

	/* Retrieve updated information */
	png_uint_32 w, h;
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL,
			NULL, NULL);
	*width = w;
	*height = h;

	switch (color_type) {
	case PNG_COLOR_TYPE_GRAY:
		*depth = 1;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		*depth = 2;
		break;
	case PNG_COLOR_TYPE_RGB:
		*depth = 3;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		*depth = 4;
		break;
	default:
		/* Badness */
		break;
	}

	/* We can now allocate memory for storing pixel data */
	unsigned char *cdata = new unsigned char[w * h * (*depth)];

	/* Setup a pointer array.  Each one points at the begening of a row. */
	png_bytep *row_pointers = new png_bytep[h];
	for (unsigned int i = 0; i < h; i++)
		row_pointers[i] = (png_bytep) (cdata + ((h - (i + 1)) * w * (*depth)));

	/* Read pixel data using row pointers */
	png_read_image(png_ptr, row_pointers);

	/* We don't need row pointers anymore */
	delete[] row_pointers;

	/* Finish decompression and release memory */
	png_read_end(png_ptr, NULL);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	fclose(fp);

	double *data = new double[w * h * (*depth)];
	for (unsigned int i = 0; i < (h * w * (*depth)); i++)
		data[i] = cdata[i] / 256.0;
	delete cdata;
	return data;
}

int read_train_parameters(char *filename, int *C, int*P, double*lambda,
		int*eras, int*stationary, int*verbose_level) {
	ifstream file;
	file.open(filename, ios::in);
	if (!file.is_open())
		return TP_FN;
	file >> *C;
	file >> *P;
	file >> *lambda;
	file >> *stationary;
	file >> *eras;
	file >> *verbose_level;
	if (file.bad())
		return TP_FRMT;
	file.close();
	return 0;
}
