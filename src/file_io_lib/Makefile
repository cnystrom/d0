
TARGETS =	$(TARGET1)

#==============================================================================

FILENAME1 =	file_io_lib
TARGET1 =	$(FILENAME1).a

HEADERS1 = 	\
		$(FILENAME1).h \
		$(NULL)

CCFILES1 =	\
		misc_io.c \
		path.c \
		$(NULL)

OBJS1 =		${CCFILES1:.c=.o}

LIBS1 = 	\
		$(STR_LIB_BIN) \
		$(NULL)

STR_LIB_PATH =	../str_lib
STR_LIB_BIN =	$(STR_LIB_PATH)/str_lib.a

#==============================================================================

CC =		gcc

COPTS =		-O3 -g -Wall 

INCLUDES = 	\
		$(NULL)

LIBDIRS = 	\
		$(NULL)
 
#CFLAGS =	$(COPTS) $(INCLUDES) -DUNIX
CFLAGS =	$(COPTS) $(INCLUDES) -DDEBUG -DUNIX

LFLAGS =	$(LIBDIRS)

#default compilation rules
%.o:		%.c
		$(CC) -c $< $(CFLAGS) -o $@

all: 		$(TARGETS)

default:	$(TARGETS)

new:		clean default

#==============================================================================

$(TARGET1):	$(OBJS1) $(HEADERS1) $(LIBS1)
		ar rcs $(TARGET1) $(LIBS1) $(OBJS1)
		ranlib $(TARGET1)

$(STR_LIB_BIN):
		cd $(STR_LIB_PATH); make

indent:
		indent -kr -i8 *.h *.c 

clean:
		rm -f $(TARGETS) *.o *.a core a.out *~
 
