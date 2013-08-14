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
	GLUS/glus_viewprojection.c \

GLUS_GL_SRC= \
	GLUS/glus_glfw.c \
	GLUS/glus_line.c \
	GLUS/glus_perlin.c \
	GLUS/glus_shaderprogram.c \
	GLUS/glus_shape.c \
	GLUS/glus_shape_adjacency.c \
	GLUS/glus_shape_texgen.c \
	GLUS/glus_wavefront.c \

GLUS_GLES_SRC= \
	GLUS/glus_egl_es.c \
	GLUS/glus_line.c \
	GLUS/glus_os_wrapper_es.c \
	GLUS/glus_perlin_es.c \
	GLUS/glus_shaderprogram_es.c \
	GLUS/glus_shape.c \
	GLUS/glus_shape_texgen.c \
	GLUS/glus_wavefront.c \

GLUS_GLES2_SRC= \
	GLUS/glus_egl_es.c \
	GLUS/glus_line_es2.c \
	GLUS/glus_os_wrapper_es.c \
	GLUS/glus_perlin_es.c \
	GLUS/glus_shaderprogram_es.c \
	GLUS/glus_shape_es2.c \
	GLUS/glus_shape_texgen_es2.c \
	GLUS/glus_wavefront_es2.c \

GLUS_SRC=$(GLUS_COMMON_SRC) $(GLUS_GLES2_SRC) GLUS/glus_raspberrypi_es.c
#	glus_windows_es.c


ARTIFACTUARY_SRC=$(GLUS_SRC) \
	artifactuary.c \
	array.c \
	main.c \
	gles2_harness.c \
	spi_harness.c \
	fire.c \
	scroll.c \
	

.PHONY: all clean

%.o: %.c *.h
	gcc -g -W -Wall --std=gnu99 -fgnu89-inline -c -o $@ -DGLUS_ES2 -I ./GLUS -I /opt/vc/include -I /opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux $<

artifactuary: $(patsubst %.c,%.o,$(ARTIFACTUARY_SRC) $(GLUS_SRC))
	gcc -g -o artifactuary -lSDL -lEGL -lGLESv2 -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm -L /opt/vc/lib $^

all: artifactuary

clean:
	for X in viz1 *.o GLUS/*.o; do \
	  if [ -f $$X ]; then rm $$X; fi \
	done

