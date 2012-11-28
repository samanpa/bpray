/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FNUM = 258,
     NUM = 259,
     IDENTIFIER = 260,
     STRING = 261,
     WIDTH = 262,
     HEIGHT = 263,
     ADC_BAILOUT = 264,
     AMBIENT = 265,
     AMBIENT_LIGHT = 266,
     ANGLE = 267,
     ANTI_ALIASING_DEPTH = 268,
     ASSUMED_GAMMA = 269,
     BACKGROUND = 270,
     BUMP_MAP = 271,
     CAMERA = 272,
     COLOR = 273,
     DECLARE = 274,
     DIFFUSE = 275,
     DIRECTION = 276,
     FACE_INDICES = 277,
     FALSE_LITERAL = 278,
     FINISH = 279,
     FILTER = 280,
     FRESNEL = 281,
     GIF = 282,
     GLOBAL_SETTINGS = 283,
     INPUT_FILE_NAME = 284,
     IOR = 285,
     JPEG = 286,
     KMESH = 287,
     IMAGE_MAP = 288,
     INTERIOR_TEXTURE = 289,
     LIBRARY_PATH = 290,
     LIGHT_SOURCE = 291,
     LOCATION = 292,
     LOOK_AT = 293,
     MATERIAL = 294,
     MAX_TRACE_LEVEL = 295,
     MESH = 296,
     METALLIC = 297,
     NORMAL = 298,
     NORMALS = 299,
     OUTPUT_FILE_NAME = 300,
     OUTPUT_FILE_TYPE = 301,
     PHONG = 302,
     PHONG_SIZE = 303,
     PIGMENT = 304,
     PNG = 305,
     POSITION = 306,
     REFLECTION = 307,
     BP_RGB = 308,
     RIGHT = 309,
     ROTATE = 310,
     SMOOTH_TRIANGLE = 311,
     SPECULAR = 312,
     SKY = 313,
     SPHERE = 314,
     TEXTURE = 315,
     TRANSLATE = 316,
     TRIANGLE = 317,
     TRANSMIT = 318,
     TRUE_LITERAL = 319,
     UP = 320,
     UV_VECTORS = 321,
     UV_MAPPING = 322,
     VERTICES = 323
   };
#endif
/* Tokens.  */
#define FNUM 258
#define NUM 259
#define IDENTIFIER 260
#define STRING 261
#define WIDTH 262
#define HEIGHT 263
#define ADC_BAILOUT 264
#define AMBIENT 265
#define AMBIENT_LIGHT 266
#define ANGLE 267
#define ANTI_ALIASING_DEPTH 268
#define ASSUMED_GAMMA 269
#define BACKGROUND 270
#define BUMP_MAP 271
#define CAMERA 272
#define COLOR 273
#define DECLARE 274
#define DIFFUSE 275
#define DIRECTION 276
#define FACE_INDICES 277
#define FALSE_LITERAL 278
#define FINISH 279
#define FILTER 280
#define FRESNEL 281
#define GIF 282
#define GLOBAL_SETTINGS 283
#define INPUT_FILE_NAME 284
#define IOR 285
#define JPEG 286
#define KMESH 287
#define IMAGE_MAP 288
#define INTERIOR_TEXTURE 289
#define LIBRARY_PATH 290
#define LIGHT_SOURCE 291
#define LOCATION 292
#define LOOK_AT 293
#define MATERIAL 294
#define MAX_TRACE_LEVEL 295
#define MESH 296
#define METALLIC 297
#define NORMAL 298
#define NORMALS 299
#define OUTPUT_FILE_NAME 300
#define OUTPUT_FILE_TYPE 301
#define PHONG 302
#define PHONG_SIZE 303
#define PIGMENT 304
#define PNG 305
#define POSITION 306
#define REFLECTION 307
#define BP_RGB 308
#define RIGHT 309
#define ROTATE 310
#define SMOOTH_TRIANGLE 311
#define SPECULAR 312
#define SKY 313
#define SPHERE 314
#define TEXTURE 315
#define TRANSLATE 316
#define TRIANGLE 317
#define TRANSMIT 318
#define TRUE_LITERAL 319
#define UP 320
#define UV_VECTORS 321
#define UV_MAPPING 322
#define VERTICES 323




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 97 "bp_script_parser.y"
typedef union YYSTYPE {
	vector_t    vector;
	float       vector2 [5];
/* 	vector3_t   vector3; */
	vector4_t   vector4;
	float       vector5 [5];

	float       fval;
	int         val;
        char       *string;

	finish_t   *finish;
	texture_t  *texture;
	material_t *material;
	pigment_t  *pigment;
	normal_t   *normal;

	image_t    *image;

	struct  list_t {
		int tag;
		union YYSTYPE *data;
		struct list_t *next;
	} *list;
} YYSTYPE;
/* Line 1447 of yacc.c.  */
#line 200 "bp_script_parser.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



