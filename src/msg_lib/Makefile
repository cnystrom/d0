
TARGETS =	$(TARGET1) $(TARGET2) $(TARGET3)
#TARGETS =	$(MSG_LIB_BIN)

#==============================================================================

FILENAME1 =	server
TARGET1 =	$(FILENAME1)

HEADERS1 = 	\
		msg_lib.h \
		$(NULL)

CCFILES1 =	\
		$(FILENAME1).c \
		$(NULL)

OBJS1 =		${CCFILES1:.c=.o}

LIBS1 = 	\
		$(SOCK_LIB_BIN) \
		$(MSG_LIB_BIN) \
		$(NULL)

#==============================================================================

FILENAME2 =	client
TARGET2 =	$(FILENAME2)

HEADERS2 = 	\
		msg_lib.h \
		$(NULL)

CCFILES2 =	\
		$(FILENAME2).c \
		$(NULL)

OBJS2 =		${CCFILES2:.c=.o}

LIBS2 = 	\
		$(MSG_LIB_BIN) \
		$(NULL)

#==============================================================================

FILENAME3 =	server2
TARGET3 =	$(FILENAME3)

HEADERS3 = 	\
		msg_lib.h \
		$(NULL)

CCFILES3 =	\
		$(FILENAME3).c \
		$(NULL)

OBJS3 =		${CCFILES3:.c=.o}

LIBS3 = 	\
		$(MSG_LIB_BIN) \
		$(NULL)

#==============================================================================

SOCK_LIB =	sock_lib
SOCK_LIB_PATH =	../$(SOCK_LIB)
SOCK_LIB_BIN =	$(SOCK_LIB_PATH)/$(SOCK_LIB).a
SOCK_LIB_OBJ =	$(SOCK_LIB_PATH)/$(SOCK_LIB).o

#==============================================================================

MSG_LIB_PATH =	$(NULL)
MSG_LIB_BIN =	msg_lib.a

MSG_LIB_H =     \
                ../sock_lib/types.h \
                buffer.h \
                msg_lib.h \
                $(NULL)

MSG_LIB_C =     \
                buffer.c \
                msg_lib.c \
                $(NULL)

MSG_LIB_OBJS =  \
                ${MSG_LIB_C:.c=.o} \
                $(NULL)

#==============================================================================

CC =		gcc

COPTS =		-O3 -g -Wall $(shell sdl-config --cflags)

INCLUDES = 	\
		$(NULL)

LIBDIRS = 	\
		$(NULL)
 
#CFLAGS =	$(COPTS) $(INCLUDES) -DUNIX
CFLAGS =	$(COPTS) $(INCLUDES) -DDEBUG -DUNIX

LFLAGS =	$(LIBDIRS) $(shell sdl-config --libs)

#default compilation rules
%.o:		%.c
		$(CC) -c $< $(CFLAGS) -o $@

all: 		$(TARGETS)

default:	$(TARGETS)

new:		clean default

#==============================================================================

$(TARGET1):	$(OBJS1) $(HEADERS1) $(LIBS1)
		$(CC) $(LFLAGS) $(OBJS1) $(LIBS1) -o $@

$(TARGET2):	$(OBJS2) $(HEADERS2) $(LIBS2)
		$(CC) $(LFLAGS) $(OBJS2) $(LIBS2) -o $@

$(TARGET3):	$(OBJS3) $(HEADERS3) $(LIBS3)
		$(CC) $(LFLAGS) $(OBJS3) $(LIBS3) -o $@

$(MSG_LIB_BIN): $(SOCK_LIB_OBJ) $(MSG_LIB_OBJS) $(MSG_LIB_H) $(MSG_LIB_C)
		ar rcs $(MSG_LIB_BIN) $(SOCK_LIB_OBJ) $(MSG_LIB_OBJS)
		ranlib $(MSG_LIB_BIN)

$(SOCK_LIB_BIN):
		cd $(SOCK_LIB_PATH); make

indent:
		indent -kr -i8 *.h *.c 

clean:
		rm -f $(TARGETS) *.o *.a core a.out *~
 
