EXTRA_DIST 		= ply2mesh.py

bin_PROGRAMS 		= banna
lib_LTLIBRARIES 	= libbp_ray.la

#force lex and yacc to generate files
BUILT_SOURCES = bp_script_parser.h bp_script_lexer.c bp_script_parser.c \
	bp_ini_lexer.c bp_ini_parser.c
CLEANFILES = $(BUILT_SOURCES) bp_script_parser.c $(srcdir)/*~

#change optimiztion flag
common_CFLAGS 		= -g -Wall -Wimplicit -pedantic -O3 -pipe  

core_FILES		= bp_scene.c bp_scene.h bp_ray.c bp_ray.h bp_config.h \
	config.h bp_camera.c bp_camera.h bp_types.h 

util_FILES		= bp_io.h bp_io.c bp_memory.h bp_memory.c bp_utils.h \
	bp_utils.c bp_math.h bp_math_generic.h bp_math.c math_sse.h bp_debug.h \
	bp_debug.c bp_kd_tree.c bp_kd_tree.h bp_kd_tree_wald.c

parse_FILES		= bp_script.h bp_script_parser.y bp_script_lexer.l \
	bp_ini_lexer.l bp_ini_parser.y

material_FILES		= bp_pigment.h bp_pigment.c bp_texture.h bp_texture.c \
	bp_material.c bp_material.h bp_image.h bp_image.c bp_normal.h bp_normal.c \
	bp_perlin_noise.c bp_noise.h bp_shade.c bp_shade.h

object_FILES		= bp_light.c bp_light.h bp_object.h bp_object.c \
	bp_triangle.c bp_sphere.h bp_sphere.c bp_box.h bp_box.c mesh.h mesh.c \
	bp_primitive.h $(material_FILES)

#create bp_script_parser.h
AM_YFLAGS = -d

libbp_ray_la_SOURCES	= $(core_FILES) $(parse_FILES) $(object_FILES) $(util_FILES)


libbp_ray_la_LDFLAGS 	= -no-undefined -version-info 0:1:0
libbp_ray_la_CFLAGS 	= $(common_CFLAGS)


banna_CFLAGS  		= $(common_CFLAGS)
banna_SOURCES 		= banna.c ui.h ui.c
banna_LDADD 		= libbp_ray.la
banna_LDFLAGS 		= -lgd -lGL -lglut
