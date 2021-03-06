LIBS := -lpthread -lOsi -lOsiClp -lCoinUtils -lClp -lOGDF -lpng -lgsl -lgslcblas -lGL -lGLEW  -lglut -lglui 

#Modify variable LIBS_DIRS with installed libs paths
LIBS_DIRS := # example: /usr/lib/coin-Osi/lib

#Modify variable OGDF_LIB_DIR with installed OGDF lib path
OGDF_LIB_DIR :=  # example: /usr/lib/OGDF

SUBDIRS := \
. \
scigl \

#Source files
CPP_SRCS += \
./BLAS.cpp \
./Dataset.cpp \
./GTMsd.cpp \
./I_O.cpp \
./Tree.cpp \
./WorkingTree.cpp \
./em.cpp \
./error.cpp \
./grid.cpp \
./gui.cpp \
./histogram.cpp \
./image.cpp \
./inference.cpp \
./main.cpp \
./map.cpp \
./train.cpp \
./svm.cpp \
./tree_svm.cpp

#Object files
OBJS += \
./obj/BLAS.o \
./obj/Dataset.o \
./obj/GTMsd.o \
./obj/I_O.o \
./obj/Tree.o \
./obj/WorkingTree.o \
./obj/em.o \
./obj/error.o \
./obj/grid.o \
./obj/gui.o \
./obj/histogram.o \
./obj/image.o \
./obj/inference.o \
./obj/main.o \
./obj/map.o \
./obj/train.o \
./obj/svm.o \
./obj/tree_svm.o

#SCIGL Source files
CPP_SRCS += \
./scigl/grid_axis.cpp 

CC_SRCS += \
./scigl/axis.cc \
./scigl/colorbar.cc \
./scigl/colormap.cc \
./scigl/cube.cc \
./scigl/data.cc \
./scigl/font.cc \
./scigl/gram.cc \
./scigl/light.cc \
./scigl/line.cc \
./scigl/my_cloud.cc \
./scigl/my_frame.cc \
./scigl/object.cc \
./scigl/scene.cc \
./scigl/shapes.cc \
./scigl/surface.cc \
./scigl/terminal.cc \
./scigl/textbox.cc \
./scigl/texture.cc \
./scigl/trackball.cc \
./scigl/widget.cc 

#SCIGL Object files
OBJS += \
./obj/scigl/axis.o \
./obj/scigl/colorbar.o \
./obj/scigl/colormap.o \
./obj/scigl/cube.o \
./obj/scigl/data.o \
./obj/scigl/font.o \
./obj/scigl/gram.o \
./obj/scigl/grid_axis.o \
./obj/scigl/light.o \
./obj/scigl/line.o \
./obj/scigl/my_cloud.o \
./obj/scigl/my_frame.o \
./obj/scigl/object.o \
./obj/scigl/scene.o \
./obj/scigl/shapes.o \
./obj/scigl/surface.o \
./obj/scigl/terminal.o \
./obj/scigl/textbox.o \
./obj/scigl/texture.o \
./obj/scigl/trackball.o \
./obj/scigl/widget.o 

USER_DEFINE :=

obj/scigl/%.o: ./scigl/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUSE_COIN -DCOIN_OSI_CLP -I $(OGDF_LIB_DIR) -I ./scigl -O0 -g3 -Wall -c -fmessage-length=0   -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

obj/scigl/%.o: ./scigl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUSE_COIN -DCOIN_OSI_CLP -I $(OGDF_LIB_DIR) -I ./scigl -O0 -g3 -Wall -c -fmessage-length=0   -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

obj/%.o: ./%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(USER_DEFINE) -DUSE_COIN -DCOIN_OSI_CLP -I $(OGDF_LIB_DIR) -I ./scigl -O0 -g3 -Wall -c -fmessage-length=0  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

GenTree: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L $(OGDF_LIB_DIR)/_release -L $(LIBS_DIRS) -o "../GenTree" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

stable: USER_DEFINE = -D GENTREE_STABLE
stable:	GenTree

format_control: USER_DEFINE = - D CONTROL
format_control: GenTree

stable+control:  USER_DEFINE = -D GENTREE_STABLE -D CONTROL
stable+control: GenTree
