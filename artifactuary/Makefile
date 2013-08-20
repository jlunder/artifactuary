ifeq ($(OS),WIN32)
	CCFLAGS += -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		CCFLAGS += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		CCFLAGS += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OS = LINUX
		CCFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OS = OSX
		CCFLAGS += -D OSX
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		CCFLAGS += -D ARM
	endif
endif


USE_NCURSES=1


GLUS_COMMON_SRC= \
	GLUS/glus_extension.c \
	GLUS/glus_load.c \
	GLUS/glus_log.c \
	GLUS/glus_math.c \
	GLUS/glus_matrix.c \
	GLUS/glus_planar.c \
	GLUS/glus_plane.c \
	GLUS/glus_point.c \
	GLUS/glus_quaternion.c \
	GLUS/glus_random.c \
	GLUS/glus_save.c \
	GLUS/glus_screenshot.c \
	GLUS/glus_vector.c \
	GLUS/glus_version.c \
	GLUS/glus_viewprojection.c

GLUS_GL_SRC= \
	GLUS/glus_glfw.c \
	GLUS/glus_line.c \
	GLUS/glus_perlin.c \
	GLUS/glus_shaderprogram.c \
	GLUS/glus_shape.c \
	GLUS/glus_shape_adjacency.c \
	GLUS/glus_shape_texgen.c \
	GLUS/glus_wavefront.c

GLUS_GLES_SRC= \
	GLUS/glus_egl_es.c \
	GLUS/glus_line.c \
	GLUS/glus_os_wrapper_es.c \
	GLUS/glus_perlin_es.c \
	GLUS/glus_shaderprogram_es.c \
	GLUS/glus_shape.c \
	GLUS/glus_shape_texgen.c \
	GLUS/glus_wavefront.c

GLUS_GLES2_SRC= \
	GLUS/glus_egl_es.c \
	GLUS/glus_line_es2.c \
	GLUS/glus_os_wrapper_es.c \
	GLUS/glus_perlin_es.c \
	GLUS/glus_shaderprogram_es.c \
	GLUS/glus_shape_es2.c \
	GLUS/glus_shape_texgen_es2.c \
	GLUS/glus_wavefront_es2.c


ifeq ($(OS), LINUX)
	GLUS_SRC=$(GLUS_COMMON_SRC) $(GLUS_GLES2_SRC) GLUS/glus_raspberrypi_es.c
	CC=gcc
	CFLAGS=-O2 -W -Wall --std=gnu99 -fgnu89-inline
	CPPFLAGS=-DGLUS_ES2 -I ./GLUS -I /opt/vc/include -I /opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux
	LDFLAGS=-lSDL -lEGL -lGLESv2 -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm -L /opt/vc/lib
endif
ifeq ($(OS), OSX)
	GLUS_SRC=$(GLUS_COMMON_SRC) $(GLUS_GL_SRC)
	CC=gcc
	CFLAGS=-O2 -W -Wall --std=gnu99 -fgnu89-inline
	CPPFLAGS=-DGLUS_GL -I ./GLUS
	LDFLAGS=-lGL -lrt -lm
endif
ifeq ($(OS), WIN32)
	GLUS_SRC=$(GLUS_COMMON_SRC) $(GLUS_GLES2_SRC) GLUS/glus_windows_es.c
	CC=!
	CFLAGS=
	CPPFLAGS=
	LDFLAGS=
endif


ifeq ($(USE_NCURSES), 1)
	CPPFLAGS+= -DUSE_NCURSES
	LDFLAGS+= -lncurses
endif


MAKEDEPEND=@set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.d: %.c Makefile
	@$(MAKEDEPEND)


ARTIFACTUARY_SRC= \
	$(GLUS_SRC) \
	artifactuary.c \
	array.c \
	effect.c \
	main.c \
	effect_joe_fire.c \
	effect_joe_text_scroll.c \
	effect_timer_select.c \
	effect_vlad_fire_0.c \
	effect_vlad_plasma_0.c \
	gles2_harness.c \
	spi_harness.c


-include $(ARTIFACTUARY_SRC:.c=.d)
-include $(GLUS_SRC:.c=.d)


.PHONY: all cleanall clean


%.o: %.c Makefile
	@$(MAKEDEPEND)
	gcc -c -g $(CFLAGS) -o $@ $(CPPFLAGS) $<

artifactuary: $(ARTIFACTUARY_SRC:.c=.o)
	gcc -g -o $@ $(LDFLAGS) $^

all: artifactuary

cleanall:
	for X in artifactuary *.o GLUS/*.o; do \
		if [ -f $$X ]; then rm $$X; fi \
		done

clean:
	for X in artifactuary *.o; do \
		if [ -f $$X ]; then rm $$X; fi \
		done

