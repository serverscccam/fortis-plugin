# Makefile

TARGET_DIR = /opt/STM/STLinux-2.3/devkit/sh4/target
ROOT_DIR = /opt/STM/STLinux-2.3/devkit/sh4/bin/sh4-linux-
INCDIR+= -I$(TARGET_DIR)/include
INCDIR+= -I$(TARGET_DIR)/usr/include
INCDIR+= -I$(TARGET_DIR)/usr/include/directfb
INCDIR+= -I$(TARGET_DIR)/usr/include/freetype2
INCDIR+= -I$(TARGET_DIR)/usr/local/include/freetype2
INCDIR += -Iinclude -I../include -I../../include

###########################
## Common compil options ##
###########################

OPTLEVEL = 2

CC       = $(ROOT_DIR)gcc
CFLAGS   = -w -c -O$(OPTLEVEL) -DST_7201  -DST_OSLINUX  -DARCHITECTURE_ST40
LD       = $(ROOT_DIR)/sh4-linux-ld
LEX      = flex
OBJ     := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
COBJ     := $(patsubst %.c,%.o,$(wildcard *.c))

INCLUDE  = -I.  $(INCDIR)

MAKEXE   = make
BINNAME  = maaking


DEFS     =

all: unix

clean:
	rm -f *.o $(BINNAME) core

unix: $(COBJ) $(OBJ)
	$(ROOT_DIR)g++ -lfreetype -lcrypt -lpthread -lssl -lrt  -o  $(BINNAME)   $(COBJ) $(OBJ) ../plugapi/plugapi.a
	$(ROOT_DIR)strip --strip-debug --strip-unneeded   $(BINNAME)

$(patsubst %.cpp,%.o,$(wildcard *.cpp)) : %.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INCLUDE) $(DEFS)

$(patsubst %.c,%.o,$(wildcard *.c)) : %.o : %.c
	$(CC) $(CFLAGS) -c $< $(INCLUDE) $(DEFS)

