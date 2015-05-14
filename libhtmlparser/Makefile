MAJOR=1
MINOR=0
VERSION=$(MAJOR).$(MINOR)

#TARGETAR=libhtmlparser.a
TARGETSO=libhtmlparser.so

CC=gcc
CXX=g++ -g -pipe
LD=ld
AR=ar

CFLAGS= -DNDEBUG -D_UNIX -D_LINUX -D_REENTRANT -Wall -fPIC  -I. -I/usr/local/include 
CXXFLAGS= -DNDEBUG -D_UNIX -D_LINUX -D_REENTRANT -Wall -fPIC  -I. -I/usr/local/include  
ARFLAGS=-rcs
LDFLAGS=$(ARCH) -shared -Wl,-soname,$(TARGETSO).$(MAJOR) -lpthread  -L/usr/local/lib -o
SYSLIBS=-lpthread 
LIBS= 

ARCH=
RANLIB=ranlib
LINK=ln -sf

#OBJ_DIR=.obj/

SRCS= $(wildcard *.c) $(wildcard *.cc)
OBJS=$(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(SRCS)))
TARGET_OBJS=$(patsubst %.c,$(OBJ_DIR)%.o,$(patsubst %.cc,$(OBJ_DIR)%.o,$(SRCS)))

.SUFFIXES: .c .cc

.c.o:
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)$@ $<

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $(OBJ_DIR)$@ $<

.PHONY : all clean

all: $(TARGETAR) $(TARGETSO)

#$(TARGETAR): $(OBJS)
#	$(AR) $(ARFLAGS) $@ $(OBJS)
#	$(RANLIB) $(TARGETAR)

$(TARGETSO): $(OBJS)
	$(CXX) $(LDFLAGS) $(TARGETSO).$(VERSION) $(TARGET_OBJS)
	$(LINK) $(TARGETSO).$(VERSION) $(TARGETSO).$(MAJOR)
	$(LINK) $(TARGETSO).$(MAJOR) $(TARGETSO)


clean:
	rm -f $(TARGETAR) $(TARGETSO) $(TARGETSO).$(VERSION) $(TARGETSO).$(MAJOR) $(TARGET_OBJS) core

