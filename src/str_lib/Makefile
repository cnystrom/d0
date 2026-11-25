
TARGETS =	$(TARGET1)

#==============================================================================

FILENAME1 =	str_lib
TARGET1 =	$(FILENAME1).a

HEADERS1 = 	\
		$(NULL)

CCFILES1 =	\
		strclean.c \
		strlcat.c \
		strlcpy.c \
		$(NULL)

OBJS1 =		${CCFILES1:.c=.o}

LIBS1 = 	\
		$(NULL)

#==============================================================================

SCP_LIB = 	scp_lib
SCP_LIB_PATH =	.
SCP_LIB_BIN =	$(SCP_LIB_PATH)/$(SCP_LIB).a

SCP_LIB_H =	\
		uio.h \
		$(NULL)

SCP_LIB_C =	\
		uio.c \
		$(NULL)

SCP_LIB_OBJS =	\
		${SCP_LIB_C:.c=.o} \
		$(SCP_LIB).o \
		$(NULL)

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
		ar rcs $(TARGET1) $(OBJS1)
		ranlib $(TARGET1)

indent:
		indent -kr -i8 *.h *.c 

clean:
		rm -f $(TARGETS) *.o *.a core a.out *~
 
