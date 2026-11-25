
TARGETS =	$(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)
#TARGETS =	$(SOCK_LIB_BIN)

#==============================================================================

FILENAME1 =	tcp_server
TARGET1 =	$(FILENAME1)

HEADERS1 = 	\
		sock_lib.h \
		$(NULL)

CCFILES1 =	\
		$(FILENAME1).c \
		$(NULL)

OBJS1 =		${CCFILES1:.c=.o}

LIBS1 = 	\
		-lnsl \
		$(SOCK_LIB_BIN) \
		$(NULL)

#==============================================================================

FILENAME2 =	tcp_client
TARGET2 =	$(FILENAME2)

HEADERS2 = 	\
		sock_lib.h \
		$(NULL)

CCFILES2 =	\
		$(FILENAME2).c \
		$(NULL)

OBJS2 =		${CCFILES2:.c=.o}

LIBS2 = 	\
		-lnsl \
		$(SOCK_LIB_BIN) \
		$(NULL)

#==============================================================================

FILENAME3 =	udp_server
TARGET3 =	$(FILENAME3)

HEADERS3 = 	\
		sock_lib.h \
		$(NULL)

CCFILES3 =	\
		$(FILENAME3).c \
		$(NULL)

OBJS3 =		${CCFILES3:.c=.o}

LIBS3 = 	\
		-lnsl \
		$(SOCK_LIB_BIN) \
		$(NULL)

#==============================================================================

FILENAME4 =	udp_client
TARGET4 =	$(FILENAME4)

HEADERS4 = 	\
		sock_lib.h \
		$(NULL)

CCFILES4 =	\
		$(FILENAME4).c \
		$(NULL)

OBJS4 =		${CCFILES4:.c=.o}

LIBS4 = 	\
		-lnsl \
		$(SOCK_LIB_BIN) \
		$(NULL)

#==============================================================================

FILENAME5 =	udp_client2
TARGET5 =	$(FILENAME5)

HEADERS5 = 	\
		sock_lib.h \
		$(NULL)

CCFILES5 =	\
		$(FILENAME5).c \
		$(NULL)

OBJS5 =		${CCFILES5:.c=.o}

LIBS5 = 	\
		-lnsl \
		$(SOCK_LIB_BIN) \
		$(NULL)

#==============================================================================

SOCK_LIB_PATH =	.
SOCK_LIB_BIN =	sock_lib.a

SOCK_LIB_H =     \
                sock_lib.h \
                $(NULL)

SOCK_LIB_C =     \
                sock_lib.c \
                $(NULL)

SOCK_LIB_OBJS =  \
                ${SOCK_LIB_C:.c=.o} \
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

$(TARGET2):	$(OBJS2) $(HEADERS2) $(LIBS2)
		$(CC) $(LFLAGS) $(OBJS2) $(LIBS2) -o $@

$(TARGET3):	$(OBJS3) $(HEADERS3) $(LIBS3)
		$(CC) $(LFLAGS) $(OBJS3) $(LIBS3) -o $@

$(TARGET4):	$(OBJS4) $(HEADERS4) $(LIBS4)
		$(CC) $(LFLAGS) $(OBJS4) $(LIBS4) -o $@

$(TARGET5):	$(OBJS5) $(HEADERS5) $(LIBS5)
		$(CC) $(LFLAGS) $(OBJS5) $(LIBS5) -o $@

$(SOCK_LIB_BIN): $(SOCK_LIB_OBJS) $(SOCK_LIB_H) $(SOCK_LIB_C)
		ar rcs $(SOCK_LIB_BIN) $(SOCK_LIB_OBJS)
		ranlib $(SOCK_LIB_BIN)

indent:
		indent -kr -i8 *.h *.c 

clean:
		rm -f $(TARGETS) *.o *.a core a.out *~ .*.swp
 
