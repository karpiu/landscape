error_flags = -Wall -Werror
std_flag = -std=c++0x
compiler = g++-4.4
libs = -lglut -lGLU -lGL

modules = utils.o lod_map.o fps.o

out = prog.e

all: $(out)

utils.o: utils.cpp utils.hpp
	$(compiler) $(error_flags) $(std_flag) -c utils.cpp $(libs) -o utils.o

fps.o: fps.cpp fps.hpp
	$(compiler) $(error_flags) $(std_flag) -c fps.cpp $(libs) -o fps.o

lod_map.o: lod_map.cpp lod_map.hpp
	$(compiler) $(error_flags) $(std_flag) -c lod_map.cpp $(libs) -o lod_map.o

prog.e: main.cpp $(modules)
	$(compiler) $(error_flags) $(std_flag) main.cpp $(modules) $(libs) -o prog.e

clear:
	rm $(out) $(modules)
