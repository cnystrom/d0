#the compiler
CC = gcc

#compiler options

# Linux
COPTS = -O3 -g -Wall $(shell sdl-config --cflags)
# Solaris
#COPTS = -O3 -g -Wall -I/usr/local/include/SDL -D_REENTRANT -Wno-deprecated 

#header files
INCLUDES = \
	-I. \
	$(NULL)

#library directories
LIBDIRS = \
	$(NULL)

#libraries to link with
LIBS = \
	-lutil \
	-lSDL_ttf \
	-lSDL_image \
	-lSDL_mixer \
	-lsmpeg \
	$(NULL)

#all compiler flags
CFLAGS = $(COPTS) $(INCLUDES) -DUNIX

#all linker options and libs
LFLAGS = $(LIBDIRS) $(shell sdl-config --libs) -lSDL_mixer

#default compilation rules
%.o:	%.c
	$(CC) -c $< $(CFLAGS) -o $@

#local headers to depend on
HEADERS = \
	sdl_lib.h \
	movie_lib.h \
	$(NULL)

#files to compile
CCFILES = \
	sdl_lib.c \
	movie_lib.c \
	movie_demo.c \
	$(NULL)

#all local .o files
OBJS =  ${CCFILES:.c=.o}

default: movie_demo

movie_demo: $(OBJS) $(HEADERS) $(LIBS)
	$(CC) $(LFLAGS) $(OBJS) $(LIBS) -o $@

new: clean default

distrib:
	./distrib.sh

indent:
	indent -kr -i8 *.h *.c

install:
	./install.sh

splint:
	splint +posixstrictlib -retvalint -retvalother -I/usr/include/SDL *.c

clean:
	rm -f a.out core $(OBJS) *.o movie_demo *~

