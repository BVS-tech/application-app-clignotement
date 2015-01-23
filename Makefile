EXEC=app-clignotement

CC=gcc
LD=g++
CCFLAGS=-c -Isrc -I$(BVSDIR)/include/bvs_toolbox -I$(BVSDIR)/include/bvs_usbdev -I$(BVSDIR)/include/bvs_protocol -I$(BVSDIR)/include/bvs_toolbox $(BVS_FLAGS) `sdl-config --cflags` -O3
LIBS=$(BVSDIR)/lib/bvs_protocol.a $(BVSDIR)/lib/bvs_usbdev.a $(BVSDIR)/lib/bvs_toolbox.a
LDFLAGS=-L$(BVSDIR)/lib $(LIBS) -lusb-1.0 $(BVS_LIBS) `sdl-config --libs` -lSDL_net -lGL

SRC= ${wildcard src/*.cpp}
OBJ= ${patsubst %.cpp, %.o, ${SRC}}

all: ${EXEC}

${EXEC}: ${OBJ} $(LIBS)
	${LD} ${OBJ} ${LDFLAGS} -o $@

%.o: %.cpp
	${CC} ${CCFLAGS} $< -o $@

open:
	gedit Makefile src/*.cpp &

clean:
	rm -f *~ include/*~ src/*~ $(OBJ) $(EXEC)

mrproper: clean
	rm -f ${EXEC}

