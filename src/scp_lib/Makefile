
#TARGETS =	$(TARGET1)
TARGETS =	$(SCP_LIB_BIN)

#==============================================================================

FILENAME1 =	test
TARGET1 =	$(FILENAME1)

HEADERS1 = 	\
		tty_echo.h \
		$(NULL)

CCFILES1 =	\
		$(FILENAME1).c \
		tty_echo.c \
		$(NULL)

OBJS1 =		${CCFILES1:.c=.o}

LIBS1 = 	\
		-lutil \
		$(SCP_LIB_BIN) \
		$(NULL)

#==============================================================================

SCP_LIB = 	scp_lib
SCP_LIB_PATH =	.
SCP_LIB_BIN =	$(SCP_LIB).a

SCP_LIB_H =	\
		uio.h \
		$(NULL)

SCP_LIB_C =	\
		$(SCP_LIB).c \
		uio.c \
		$(NULL)

SCP_LIB_OBJS =	\
		${SCP_LIB_C:.c=.o} \
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
		$(CC) $(LFLAGS) $(OBJS1) $(LIBS1) -o $@

$(SCP_LIB_BIN):	$(SCP_LIB_OBJS) $(SCP_LIB_H) $(SCP_LIB_C)
		ar rcs $(SCP_LIB_BIN) $(SCP_LIB_OBJS)
		ranlib $(SCP_LIB_BIN)

indent:
		indent -kr -i8 *.h *.c 

clean:
		rm -f $(TARGETS) *.o *.a core a.out *~
 
