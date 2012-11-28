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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 4 "bp_script_parser.y"

#include "bp_ray.h"
#include <bp_sphere.h>
#include <mesh.h>

/* declared in the lexer */
extern int bp_script_lex ();
#define yylex bp_script_lex
extern int bp_script_lineno;
extern int bp_script_lex_file (scene_t *, const char *);
extern char * bp_script_filename ();

static int has_parse_error;
static scene_t *scene;
static GHashTable *obj_table;
static bp_mempool_t *tmppool;
static void parse_error (char *str, ...);
static void parse_error1 (char *str, ...);

static bp_mesh_t mesh;
static unsigned int mesh_index;

int yyerror (char *str);

#define MAX_NUM_ITEMS 25

#define NEW_LIST(list, t) {\
        (list) =(struct list_t *)bp_mempool_alloc(tmppool, sizeof (struct list_t)); \
        (list)->data = (YYSTYPE *) bp_mempool_alloc (scene->material_pool, sizeof (YYSTYPE)); \
        (list)->next = NULL; \
        (list)->tag  = t; \
}

#define MAKE_ERROR(l) \
     do { \
        l = NEW (struct list_t); \
        l->tag = 0; \
        l->next = NULL; \
        l->data = NULL; \
     } while (0);

#define MAKE_VECTOR(i, t, d) NEW_LIST (i, t); COPY(d, i->data->vector); 
#define MAKE_FLOAT(i, t, v)  NEW_LIST (i, t); i->data->fval = v;
#define MAKE_INT(i, t, v)    NEW_LIST (i, t); i->data->val = v;
#define MAKE_OBJECT(i, t, o) NEW_LIST (i, t); i->data->object = o;

void triangle_init (vector_t v0, vector_t v1, vector_t v2,
		    float s0, float t0, float s1, float t1, float s2, float t2)
{
	vector_t edge1, edge2, normal;
	
	SUB(edge1, v1, v0);
	SUB(edge2, v2, v0);
	CROSS (normal, edge1, edge2);
	vector_normalize (normal, normal);


	bp_texture_coord2f (s0, t0); 
	bp_vertex (v0);
	bp_normal (normal);
	bp_texture_coord2f (s1, t1); 
	bp_normal (normal);
	bp_vertex (v1);
	bp_texture_coord2f (s2, t2);
	bp_normal (normal);
	bp_vertex (v2);
}

void smooth_triangle_init (vector_t v0, vector_t v1, vector_t v2, 
			   vector_t n0, vector_t n1, vector_t n2,
			   float s0, float t0, float s1, float t1,
			   float s2, float t2)
{	
	vector_t normal;

	bp_texture_coord2f (s0, t0); 
	vector_normalize (n0, normal);
	bp_normal (normal);
	bp_vertex (v0);
		
	bp_texture_coord2f (s1, t1); 
	vector_normalize (n1, normal);
	bp_normal (normal);
	bp_vertex (v1);

	bp_texture_coord2f (s2, t2);
	vector_normalize (n2, normal);
	bp_normal (normal);
	bp_vertex (v2);
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 196 of yacc.c.  */
#line 340 "bp_script_parser.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 352 "bp_script_parser.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  68
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   386

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  75
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  58
/* YYNRULES -- Number of rules. */
#define YYNRULES  143
/* YYNRULES -- Number of states. */
#define YYNSTATES  349

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   323

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    70,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      69,    72,    71,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    73,     2,    74,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    17,    25,    27,
      32,    42,    54,    56,    58,    60,    63,    65,    67,    69,
      71,    73,    78,    83,    88,    90,    92,    94,    96,    98,
     100,   102,   104,   106,   108,   110,   116,   120,   125,   127,
     130,   133,   136,   139,   142,   144,   149,   153,   155,   158,
     160,   163,   166,   169,   172,   175,   178,   181,   183,   184,
     193,   198,   201,   204,   208,   211,   214,   217,   220,   221,
     226,   231,   233,   236,   238,   241,   244,   247,   250,   253,
     256,   258,   263,   268,   273,   276,   283,   290,   295,   298,
     305,   310,   315,   319,   321,   323,   330,   334,   344,   360,
     364,   380,   402,   406,   407,   408,   418,   419,   426,   429,
     431,   433,   434,   441,   444,   446,   448,   449,   456,   459,
     461,   469,   470,   471,   479,   483,   485,   488,   490,   492,
     494,   495,   497,   507,   523,   527,   543,   565,   569,   571,
     573,   575,   581,   586
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
      82,     0,    -1,    64,    -1,    23,    -1,     3,    -1,     4,
      -1,    69,    77,    70,    77,    71,    -1,    69,    77,    70,
      77,    70,    77,    71,    -1,    77,    -1,    69,    77,     1,
      71,    -1,    69,    77,    70,    77,    70,    77,    70,    77,
      71,    -1,    69,    77,    70,    77,    70,    77,    70,    77,
      70,    77,    71,    -1,    83,    -1,     1,    -1,    84,    -1,
      83,    84,    -1,    85,    -1,    87,    -1,    86,    -1,    90,
      -1,    93,    -1,    19,     5,    72,    88,    -1,    15,    73,
      79,    74,    -1,    15,    73,    97,    74,    -1,    88,    -1,
      89,    -1,   105,    -1,   106,    -1,   107,    -1,   123,    -1,
     108,    -1,    98,    -1,   103,    -1,   102,    -1,    97,    -1,
      36,    73,    79,    97,    74,    -1,    36,     1,    74,    -1,
      28,    73,    91,    74,    -1,    92,    -1,    91,    92,    -1,
       9,    77,    -1,    11,    79,    -1,    40,     4,    -1,    14,
      77,    -1,     1,    -1,    17,    73,    94,    74,    -1,    17,
       1,    74,    -1,    95,    -1,    95,    94,    -1,     1,    -1,
      21,    79,    -1,    65,    79,    -1,    54,    79,    -1,    37,
      79,    -1,    38,    79,    -1,    12,    77,    -1,    58,    79,
      -1,     1,    -1,    -1,    43,    73,    16,    73,   131,     6,
      74,    74,    -1,    43,    73,     1,    74,    -1,    18,    79,
      -1,    53,    79,    -1,    18,    53,    79,    -1,    18,    80,
      -1,    18,    81,    -1,    18,     5,    -1,    18,     1,    -1,
      -1,    24,    73,    99,    74,    -1,    24,    73,     5,    74,
      -1,   100,    -1,   100,    99,    -1,     1,    -1,    10,    79,
      -1,    52,    77,    -1,    20,    77,    -1,    47,    77,    -1,
      48,     4,    -1,    26,    76,    -1,    42,    -1,    49,    73,
      97,    74,    -1,    49,    73,   132,    74,    -1,    49,    73,
       5,    74,    -1,    49,     1,    -1,    60,    73,   101,    98,
      96,    74,    -1,    60,    73,    98,   101,    96,    74,    -1,
      60,    73,     5,    74,    -1,    60,     1,    -1,    39,    73,
     102,    30,    77,    74,    -1,    39,    73,   102,    74,    -1,
      39,    73,     5,    74,    -1,    39,    73,     1,    -1,   103,
      -1,   102,    -1,    59,    73,    79,     4,   104,    74,    -1,
      59,     1,    74,    -1,    62,    73,    79,    70,    79,    70,
      79,   104,    74,    -1,    62,    73,    79,    70,    79,    70,
      79,    66,    78,    70,    78,    70,    78,   104,    74,    -1,
      62,     1,    74,    -1,    56,    73,    79,    70,    79,    70,
      79,    70,    79,    70,    79,    70,    79,   104,    74,    -1,
      56,    73,    79,    70,    79,    70,    79,    70,    79,    70,
      79,    70,    79,    66,    78,    70,    78,    70,    78,   104,
      74,    -1,    56,     1,    74,    -1,    -1,    -1,    32,    73,
     109,   111,   115,   119,   104,   110,    74,    -1,    -1,    68,
      73,     4,   112,   113,    74,    -1,   113,   114,    -1,   114,
      -1,    79,    -1,    -1,    44,    73,     4,   116,   117,    74,
      -1,   117,   118,    -1,   118,    -1,    79,    -1,    -1,    22,
      73,     4,   120,   121,    74,    -1,   121,   122,    -1,   122,
      -1,    69,     4,    70,     4,    70,     4,    71,    -1,    -1,
      -1,    41,    73,   124,   126,   104,   125,    74,    -1,    41,
       1,    74,    -1,   127,    -1,   127,   126,    -1,     1,    -1,
     129,    -1,   130,    -1,    -1,   102,    -1,    62,    73,    79,
      70,    79,    70,    79,   128,    74,    -1,    62,    73,    79,
      70,    79,    70,    79,    66,    78,    70,    78,    70,    78,
     128,    74,    -1,    62,     1,    74,    -1,    56,    73,    79,
      70,    79,    70,    79,    70,    79,    70,    79,    70,    79,
     128,    74,    -1,    56,    73,    79,    70,    79,    70,    79,
      70,    79,    70,    79,    70,    79,    66,    78,    70,    78,
      70,    78,   128,    74,    -1,    56,     1,    74,    -1,    31,
      -1,    50,    -1,    27,    -1,    33,    73,   131,     6,    74,
      -1,    33,    73,     1,    74,    -1,    33,     1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   229,   229,   232,   238,   241,   247,   254,   259,   263,
     269,   280,   292,   293,   300,   301,   305,   306,   307,   308,
     309,   313,   322,   326,   333,   334,   335,   336,   337,   338,
     339,   343,   348,   353,   358,   366,   369,   376,   380,   381,
     385,   389,   393,   397,   401,   408,   443,   450,   451,   456,
     462,   466,   470,   474,   478,   482,   486,   490,   500,   503,
     511,   519,   525,   531,   537,   542,   546,   556,   565,   568,
     601,   614,   615,   619,   626,   629,   632,   635,   638,   641,
     644,   651,   656,   661,   676,   685,   692,   699,   709,   726,
     732,   737,   747,   754,   755,   764,   768,   777,   788,   800,
     808,   819,   831,   840,   844,   839,   857,   856,   865,   866,
     870,   879,   878,   886,   887,   891,   900,   899,   908,   909,
     913,   926,   931,   925,   938,   945,   946,   947,   954,   955,
     960,   963,   972,   977,   983,   990,   995,  1000,  1008,  1011,
    1015,  1022,  1028,  1032
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FNUM", "NUM", "IDENTIFIER", "STRING",
  "WIDTH", "HEIGHT", "ADC_BAILOUT", "AMBIENT", "AMBIENT_LIGHT", "ANGLE",
  "ANTI_ALIASING_DEPTH", "ASSUMED_GAMMA", "BACKGROUND", "BUMP_MAP",
  "CAMERA", "COLOR", "DECLARE", "DIFFUSE", "DIRECTION", "FACE_INDICES",
  "FALSE_LITERAL", "FINISH", "FILTER", "FRESNEL", "GIF", "GLOBAL_SETTINGS",
  "INPUT_FILE_NAME", "IOR", "JPEG", "KMESH", "IMAGE_MAP",
  "INTERIOR_TEXTURE", "LIBRARY_PATH", "LIGHT_SOURCE", "LOCATION",
  "LOOK_AT", "MATERIAL", "MAX_TRACE_LEVEL", "MESH", "METALLIC", "NORMAL",
  "NORMALS", "OUTPUT_FILE_NAME", "OUTPUT_FILE_TYPE", "PHONG", "PHONG_SIZE",
  "PIGMENT", "PNG", "POSITION", "REFLECTION", "BP_RGB", "RIGHT", "ROTATE",
  "SMOOTH_TRIANGLE", "SPECULAR", "SKY", "SPHERE", "TEXTURE", "TRANSLATE",
  "TRIANGLE", "TRANSMIT", "TRUE_LITERAL", "UP", "UV_VECTORS", "UV_MAPPING",
  "VERTICES", "'<'", "','", "'>'", "'='", "'{'", "'}'", "$accept", "bool",
  "fnum", "vector2", "vector", "vector4", "vector5", "script",
  "bpr_statements", "statement", "declare_statement", "background",
  "object_statement", "bpr_item", "light", "global_settings",
  "global_settings_items", "global_settings_item", "camera_decl",
  "camera_items", "camera_item", "normal", "color", "finish",
  "finish_items", "finish_item", "pigment", "texture", "_material",
  "material", "sphere", "triangle", "smooth_triangle", "kmesh", "@1", "@2",
  "kvertices", "@3", "_kvertices", "kvertex", "knormals", "@4",
  "_knormals", "knormal", "face_indices", "@5", "_face_indices",
  "face_index", "mesh", "@6", "@7", "mesh_objects", "mesh_object",
  "mesh_object_material", "mesh_triangle", "mesh_smooth_triangle",
  "image_type", "image_map", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,    60,
      44,    62,    61,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    75,    76,    76,    77,    77,    78,    79,    79,    79,
      80,    81,    82,    82,    83,    83,    84,    84,    84,    84,
      84,    85,    86,    86,    87,    87,    87,    87,    87,    87,
      87,    88,    88,    88,    88,    89,    89,    90,    91,    91,
      92,    92,    92,    92,    92,    93,    93,    94,    94,    94,
      95,    95,    95,    95,    95,    95,    95,    95,    96,    96,
      96,    97,    97,    97,    97,    97,    97,    97,    98,    98,
      98,    99,    99,    99,   100,   100,   100,   100,   100,   100,
     100,   101,   101,   101,   101,   102,   102,   102,   102,   103,
     103,   103,   103,   104,   104,   105,   105,   106,   106,   106,
     107,   107,   107,   109,   110,   108,   112,   111,   113,   113,
     114,   116,   115,   117,   117,   118,   120,   119,   121,   121,
     122,   124,   125,   123,   123,   126,   126,   126,   127,   127,
     128,   128,   129,   129,   129,   130,   130,   130,   131,   131,
     131,   132,   132,   132
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     1,     1,     5,     7,     1,     4,
       9,    11,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     4,     4,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     5,     3,     4,     1,     2,
       2,     2,     2,     2,     1,     4,     3,     1,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     1,     0,     8,
       4,     2,     2,     3,     2,     2,     2,     2,     0,     4,
       4,     1,     2,     1,     2,     2,     2,     2,     2,     2,
       1,     4,     4,     4,     2,     6,     6,     4,     2,     6,
       4,     4,     3,     1,     1,     6,     3,     9,    15,     3,
      15,    21,     3,     0,     0,     9,     0,     6,     2,     1,
       1,     0,     6,     2,     1,     1,     0,     6,     2,     1,
       7,     0,     0,     7,     3,     1,     2,     1,     1,     1,
       0,     1,     9,    15,     3,    15,    21,     3,     1,     1,
       1,     5,     4,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    12,    14,
      16,    18,    17,    24,    25,    19,    20,    34,    31,    33,
      32,    26,    27,    28,    30,    29,     0,     0,     0,    67,
       4,     5,    66,     0,     0,     8,    61,    64,    65,     0,
       0,     0,   103,     0,     0,     0,     0,   121,     0,    62,
       0,     0,     0,     0,    88,     0,     0,     0,     1,    15,
       0,     0,    46,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    68,    73,     0,     0,     0,
       0,    80,     0,     0,     0,     0,     0,    44,     0,     0,
       0,     0,     0,    38,     0,    36,     0,    92,     0,     0,
     124,     0,     0,   102,     0,    96,     0,     0,     0,     0,
      68,    99,     0,    22,    23,    55,    50,    53,    54,    52,
      56,    51,    45,    48,     0,     0,    21,    70,    74,    76,
       3,     2,    79,    77,    78,    75,    69,    72,    40,    41,
      43,    42,    37,    39,     0,     0,     0,    91,     0,    90,
     127,     0,     0,     0,     0,   128,   129,     0,     0,     0,
      87,    84,     0,    58,    58,     0,     9,     0,     0,     0,
       0,    35,     0,     0,     0,     0,     0,    94,    93,   122,
     126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,     0,     0,     0,    89,   137,     0,
     134,     0,     0,     0,     0,    95,    83,   143,     0,    81,
      82,     0,    86,    85,     0,     0,     0,   111,     0,   104,
       0,     0,   123,     0,     0,     0,   140,   138,   139,     0,
       0,     0,     0,     0,     7,   110,     0,   109,     0,   116,
       0,     0,     0,     0,   142,     0,    60,     0,     0,     0,
       0,   107,   108,   115,     0,   114,     0,   105,     0,     0,
       0,   141,     0,     0,     0,    97,     0,    10,   112,   113,
       0,     0,   119,     0,   130,     0,     0,     0,     0,     0,
       0,   117,   118,     0,     0,   131,     0,     0,     0,     0,
       0,    11,     0,     0,     0,   132,     0,    59,     0,     0,
       0,     0,     0,     0,     6,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   100,    98,   120,   130,
     130,     0,     0,     0,     0,     0,     0,   135,   133,     0,
       0,     0,     0,     0,     0,   101,   130,     0,   136
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,   142,    45,   274,   245,    47,    48,    17,    18,    19,
      20,    21,    22,    23,    24,    25,   102,   103,    26,    81,
      82,   199,    27,    28,    95,    96,   120,   187,   188,   189,
      31,    32,    33,    34,   104,   250,   155,   226,   246,   247,
     180,   248,   264,   265,   206,   266,   281,   282,    35,   111,
     212,   163,   164,   296,   165,   166,   239,   197
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -317
static const short int yypact[] =
{
     216,  -317,   -62,     7,   123,    20,   -42,    -6,    -1,     8,
      25,    18,     1,    21,    23,    28,    31,   114,   295,  -317,
    -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,
    -317,  -317,  -317,  -317,  -317,  -317,   118,    58,   137,  -317,
    -317,  -317,  -317,     1,    24,  -317,  -317,  -317,  -317,    72,
     195,   192,  -317,    74,     1,   138,    77,  -317,    24,  -317,
      87,     1,    94,     1,  -317,   130,    96,     1,  -317,  -317,
      98,   115,  -317,   116,    24,     1,     1,     1,     1,     1,
       1,   135,    45,  -317,    53,   200,  -317,   136,     1,    24,
      83,  -317,    24,   155,    24,   140,    16,  -317,    24,     1,
      24,   207,    91,  -317,   151,  -317,   127,  -317,   149,   -14,
    -317,   132,    55,  -317,   157,  -317,   221,   156,    34,   187,
     217,  -317,   175,  -317,  -317,  -317,  -317,  -317,  -317,  -317,
    -317,  -317,  -317,  -317,   178,    24,  -317,  -317,  -317,  -317,
    -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,
    -317,  -317,  -317,  -317,   177,   210,   182,  -317,    24,  -317,
    -317,    36,    39,   -27,   107,  -317,  -317,    24,     1,   -27,
    -317,  -317,   124,   215,   215,     1,  -317,   189,   257,   190,
     243,  -317,   199,   203,     1,   205,     1,  -317,  -317,  -317,
    -317,   204,   211,   206,   208,    43,   212,   213,   219,   222,
     223,   220,    24,  -317,   285,   225,   -27,  -317,  -317,   229,
    -317,   231,   228,    24,     1,  -317,  -317,  -317,   133,  -317,
    -317,    22,  -317,  -317,     1,    70,     1,  -317,   289,  -317,
       1,     1,  -317,   232,   235,   237,  -317,  -317,  -317,   300,
     244,   249,   147,    24,  -317,  -317,    44,  -317,     1,  -317,
     250,   247,   255,     1,  -317,   252,  -317,   154,   259,   258,
      85,  -317,  -317,  -317,    46,  -317,   264,  -317,     1,     1,
     265,  -317,   331,    24,   268,  -317,    24,  -317,  -317,  -317,
     335,   -54,  -317,   270,   -15,     1,   267,   272,   259,   273,
     275,  -317,  -317,     1,   259,  -317,   269,   277,   276,    24,
     279,  -317,   348,   283,   286,  -317,     1,  -317,   287,   259,
     290,     1,   259,   160,  -317,   -27,   355,   291,   292,   259,
     293,   294,   298,     1,   259,   296,  -317,  -317,  -317,    51,
     303,   259,   259,   297,   299,   302,   304,  -317,  -317,   259,
     259,   -27,   305,   306,   259,  -317,   303,   307,  -317
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -317,  -317,    -5,   -24,    -2,  -317,  -317,  -317,  -317,   346,
    -317,  -317,  -317,   280,  -317,  -317,  -317,   274,  -317,   288,
    -317,   209,   -35,   -58,   281,  -317,   260,     0,     3,  -163,
    -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,   139,
    -317,  -317,  -317,   120,  -317,  -317,  -317,    97,  -317,  -317,
    -317,   218,  -317,  -316,  -317,  -317,   129,  -317
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -126
static const short int yytable[] =
{
      29,    71,    46,    30,    40,    41,   193,   119,    37,    53,
      59,    36,    10,   333,   334,   280,   158,    86,    29,    56,
     291,    30,    60,   240,    62,    49,    88,    40,    41,    64,
     347,    50,    66,    15,    70,   171,    89,   183,   241,    84,
     185,    83,    90,   229,   217,    15,    73,    40,    41,    40,
      41,   294,   106,   112,   134,   109,   134,    74,    91,   114,
     159,   116,   174,    92,    93,   122,    75,    51,    94,   125,
      58,   156,    52,   126,   127,   128,   129,   130,   131,   259,
      38,    54,    76,    77,   139,    29,   138,   143,    30,   145,
     -71,    57,    97,   148,    61,   150,    63,   149,    55,    78,
      98,    65,    99,    79,    67,   100,   140,   172,   160,   184,
      80,    15,   186,    58,    68,    58,   218,   332,   261,   -47,
     278,    40,    41,   135,    39,   167,    40,    41,    42,   194,
     177,   101,    72,   160,   235,   117,     4,   196,    73,   107,
     243,   244,     4,   108,    85,     4,  -125,   141,   105,    74,
     320,   110,   321,   182,     6,   276,   277,   195,    75,   144,
     236,   113,   191,   161,   237,   152,   192,  -125,   115,   162,
     121,    12,   123,   201,    76,    77,    43,    12,   343,   118,
      12,   236,   209,   238,   211,   237,    10,    58,   161,   124,
     -49,    78,    44,    97,   162,    79,    86,   225,    15,    10,
      87,    98,    80,    99,   238,    88,   100,    15,   233,   132,
     137,   151,   234,   258,   146,    89,   -68,     1,     4,   154,
      15,    90,   242,   157,     6,   169,   319,   168,   251,   252,
     170,     2,   101,     3,     4,     5,   118,    91,   260,    10,
       6,     6,    92,    93,     7,   175,   263,    94,     8,   176,
     178,   270,     9,    12,   179,    10,   181,    11,   198,   202,
      15,   203,   263,   204,   300,   205,   283,   284,   287,    12,
     304,   289,    13,   207,   213,    14,    15,   208,    16,   210,
     215,   214,   216,   297,   295,   315,   219,   220,   318,   227,
     224,   303,   221,   249,   308,   325,   222,   223,   228,   230,
     330,   231,   232,   244,   313,   253,   255,   335,   336,   317,
       2,   254,     3,     4,     5,   341,   342,   268,   256,     6,
     346,   329,   257,     7,   267,   269,   271,     8,   273,   295,
     295,     9,   275,   280,    10,   285,    11,   286,   288,   290,
     293,   298,   299,   305,   301,   302,   295,   306,    12,   309,
     307,    13,   310,   311,    14,    15,   312,    16,   314,   322,
     316,   323,   324,    15,    69,   136,   331,   326,   327,   328,
     133,   337,   339,   338,   340,   344,   153,   147,   292,   173,
     345,   348,   190,   200,   279,   262,   272
};

static const unsigned short int yycheck[] =
{
       0,    36,     4,     0,     3,     4,   169,    65,     1,     1,
      12,    73,    39,   329,   330,    69,    30,     1,    18,     1,
      74,    18,     1,     1,     1,     5,    10,     3,     4,     1,
     346,    73,     1,    60,    36,     1,    20,     1,    16,    44,
       1,    43,    26,   206,     1,    60,     1,     3,     4,     3,
       4,    66,    54,    58,     1,    55,     1,    12,    42,    61,
      74,    63,   120,    47,    48,    67,    21,    73,    52,    74,
      69,   106,    73,    75,    76,    77,    78,    79,    80,   242,
      73,    73,    37,    38,    89,    85,    88,    92,    85,    94,
      74,    73,     1,    98,    73,   100,    73,    99,    73,    54,
       9,    73,    11,    58,    73,    14,    23,    73,     1,    73,
      65,    60,    73,    69,     0,    69,    73,    66,    74,    74,
      74,     3,     4,    70,     1,    70,     3,     4,     5,     5,
     135,    40,    74,     1,     1,     5,    18,   172,     1,     1,
      70,    71,    18,     5,    72,    18,    39,    64,    74,    12,
     313,    74,   315,   158,    24,    70,    71,    33,    21,     4,
      27,    74,   167,    56,    31,    74,   168,    60,    74,    62,
      74,    53,    74,   175,    37,    38,    53,    53,   341,    49,
      53,    27,   184,    50,   186,    31,    39,    69,    56,    74,
      74,    54,    69,     1,    62,    58,     1,   202,    60,    39,
       5,     9,    65,    11,    50,    10,    14,    60,   213,    74,
      74,     4,   214,    66,    74,    20,     0,     1,    18,    68,
      60,    26,   224,    74,    24,     4,    66,    70,   230,   231,
      74,    15,    40,    17,    18,    19,    49,    42,   243,    39,
      24,    24,    47,    48,    28,    70,   248,    52,    32,    71,
      73,   253,    36,    53,    44,    39,    74,    41,    43,    70,
      60,     4,   264,    73,   288,    22,   268,   269,   273,    53,
     294,   276,    56,    74,    70,    59,    60,    74,    62,    74,
      74,    70,    74,   285,   284,   309,    74,    74,   312,     4,
      70,   293,    73,     4,   299,   319,    74,    74,    73,    70,
     324,    70,    74,    71,   306,    70,     6,   331,   332,   311,
      15,    74,    17,    18,    19,   339,   340,    70,    74,    24,
     344,   323,    73,    28,    74,    70,    74,    32,    69,   329,
     330,    36,    74,    69,    39,    70,    41,     6,    70,     4,
      70,    74,    70,    74,    71,    70,   346,    70,    53,    70,
      74,    56,     4,    70,    59,    60,    70,    62,    71,     4,
      70,    70,    70,    60,    18,    85,    70,    74,    74,    71,
      82,    74,    70,    74,    70,    70,   102,    96,   281,   119,
      74,    74,   164,   174,   264,   246,   257
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     1,    15,    17,    18,    19,    24,    28,    32,    36,
      39,    41,    53,    56,    59,    60,    62,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    93,    97,    98,   102,
     103,   105,   106,   107,   108,   123,    73,     1,    73,     1,
       3,     4,     5,    53,    69,    77,    79,    80,    81,     5,
      73,    73,    73,     1,    73,    73,     1,    73,    69,    79,
       1,    73,     1,    73,     1,    73,     1,    73,     0,    84,
      79,    97,    74,     1,    12,    21,    37,    38,    54,    58,
      65,    94,    95,    79,    77,    72,     1,     5,    10,    20,
      26,    42,    47,    48,    52,    99,   100,     1,     9,    11,
      14,    40,    91,    92,   109,    74,    79,     1,     5,   102,
      74,   124,    77,    74,    79,    74,    79,     5,    49,    98,
     101,    74,    79,    74,    74,    77,    79,    79,    79,    79,
      79,    79,    74,    94,     1,    70,    88,    74,    79,    77,
      23,    64,    76,    77,     4,    77,    74,    99,    77,    79,
      77,     4,    74,    92,    68,   111,    97,    74,    30,    74,
       1,    56,    62,   126,   127,   129,   130,    70,    70,     4,
      74,     1,    73,   101,    98,    70,    71,    77,    73,    44,
     115,    74,    77,     1,    73,     1,    73,   102,   103,   104,
     126,    77,    79,   104,     5,    33,    97,   132,    43,    96,
      96,    79,    70,     4,    73,    22,   119,    74,    74,    79,
      74,    79,   125,    70,    70,    74,    74,     1,    73,    74,
      74,    73,    74,    74,    70,    77,   112,     4,    73,   104,
      70,    70,    74,    77,    79,     1,    27,    31,    50,   131,
       1,    16,    79,    70,    71,    79,   113,   114,   116,     4,
     110,    79,    79,    70,    74,     6,    74,    73,    66,   104,
      77,    74,   114,    79,   117,   118,   120,    74,    70,    70,
      79,    74,   131,    69,    78,    74,    70,    71,    74,   118,
      69,   121,   122,    79,    79,    70,     6,    77,    70,    77,
       4,    74,   122,    70,    66,   102,   128,    79,    74,    70,
      78,    71,    70,    79,    78,    74,    70,    74,    77,    70,
       4,    70,    70,    79,    71,    78,    70,    79,    78,    66,
     104,   104,     4,    70,    70,    78,    74,    74,    71,    79,
      78,    70,    66,   128,   128,    78,    78,    74,    74,    70,
      70,    78,    78,   104,    70,    74,    78,   128,    74
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 229 "bp_script_parser.y"
    {
		(yyval.val) = 1;
	}
    break;

  case 3:
#line 232 "bp_script_parser.y"
    {
		(yyval.val) = 0;
	}
    break;

  case 4:
#line 238 "bp_script_parser.y"
    {
		(yyval.fval) = (yyvsp[0].fval);
	}
    break;

  case 5:
#line 241 "bp_script_parser.y"
    {
		(yyval.fval) = (yyvsp[0].val);
	}
    break;

  case 6:
#line 247 "bp_script_parser.y"
    {
		(yyval.vector) [0] = (yyvsp[-3].fval);
		(yyval.vector) [1] = (yyvsp[-1].fval);
	}
    break;

  case 7:
#line 254 "bp_script_parser.y"
    {
		(yyval.vector) [0] = (yyvsp[-5].fval);
		(yyval.vector) [1] = (yyvsp[-3].fval);
		(yyval.vector) [2] = (yyvsp[-1].fval);
	}
    break;

  case 8:
#line 260 "bp_script_parser.y"
    {
		(yyval.vector) [0] = (yyval.vector) [1] = (yyval.vector) [2] = (yyvsp[0].fval);
        }
    break;

  case 9:
#line 263 "bp_script_parser.y"
    {
		parse_error ("bad vector declaration");
	}
    break;

  case 10:
#line 270 "bp_script_parser.y"
    {
		(yyval.vector4) [0] = (yyvsp[-7].fval);
		(yyval.vector4) [1] = (yyvsp[-5].fval);
		(yyval.vector4) [2] = (yyvsp[-3].fval);
		(yyval.vector4) [3] = (yyvsp[-1].fval);
	}
    break;

  case 11:
#line 281 "bp_script_parser.y"
    {
		(yyval.vector4) [0] = (yyvsp[-9].fval);
		(yyval.vector4) [1] = (yyvsp[-7].fval);
		(yyval.vector4) [2] = (yyvsp[-5].fval);
		(yyval.vector4) [3] = (yyvsp[-3].fval);
		(yyval.vector4) [4] = (yyvsp[-1].fval);
	}
    break;

  case 13:
#line 294 "bp_script_parser.y"
    {
		parse_error ("error parsing script file");
	}
    break;

  case 21:
#line 314 "bp_script_parser.y"
    {
		if (g_hash_table_lookup (obj_table, (yyvsp[-2].string)))
			parse_error ("Object %s already declared\n", (yyvsp[-2].string));
		g_hash_table_insert (obj_table, (yyvsp[-2].string), (yyvsp[0].list));
	}
    break;

  case 22:
#line 323 "bp_script_parser.y"
    {
		ASSIGN (scene->settings.atmosphere.background, (yyvsp[-1].vector));
	}
    break;

  case 23:
#line 327 "bp_script_parser.y"
    {
		ASSIGN (scene->settings.atmosphere.background, (yyvsp[-1].vector5));
	}
    break;

  case 31:
#line 344 "bp_script_parser.y"
    {
		NEW_LIST ((yyval.list), FINISH);
		(yyval.list)->data->finish = (yyvsp[0].finish);
	}
    break;

  case 32:
#line 349 "bp_script_parser.y"
    {
		NEW_LIST ((yyval.list), MATERIAL);
		(yyval.list)->data->material = (yyvsp[0].material);
	}
    break;

  case 33:
#line 354 "bp_script_parser.y"
    {
		NEW_LIST ((yyval.list), TEXTURE);
		(yyval.list)->data->texture = (yyvsp[0].texture);
	}
    break;

  case 34:
#line 359 "bp_script_parser.y"
    {
		NEW_LIST ((yyval.list), COLOR);
		ASSIGN5 ((yyval.list)->data->vector5, (yyvsp[0].vector5));
	}
    break;

  case 35:
#line 366 "bp_script_parser.y"
    {
         	 bp_light_new ((yyvsp[-2].vector), (yyvsp[-1].vector5));
	 }
    break;

  case 36:
#line 369 "bp_script_parser.y"
    {
		 parse_error ("Error creating light\n");
	 }
    break;

  case 40:
#line 386 "bp_script_parser.y"
    {
		scene->settings.adc_bailout = (yyvsp[0].fval);
	}
    break;

  case 41:
#line 390 "bp_script_parser.y"
    {
		ASSIGN (scene->settings.atmosphere.ambient_light, (yyvsp[0].vector));
	}
    break;

  case 42:
#line 394 "bp_script_parser.y"
    {
		scene->settings.max_trace_level = (yyvsp[0].val);
	}
    break;

  case 43:
#line 398 "bp_script_parser.y"
    {
		scene->settings.gamma = (yyvsp[0].fval);
	}
    break;

  case 44:
#line 402 "bp_script_parser.y"
    {
		parse_error ("error in global settings declaration");
	}
    break;

  case 45:
#line 409 "bp_script_parser.y"
    {
		struct list_t *l;
		camera_t *camera = &scene->camera;

		for (l = (yyvsp[-1].list); l; l = l->next) {
			switch (l->tag) {
			case LOCATION:
				COPY (l->data->vector, camera->location);
				break;
			case SKY:
				COPY (l->data->vector, camera->sky);
				break;
 			case UP:
 				COPY (l->data->vector, camera->up);
 				break;
 			case RIGHT:
 				COPY (l->data->vector, camera->right);
 				break; 
			case LOOK_AT:
				bp_camera_set_look_at (camera, l->data->vector);
				break;
			case ANGLE:
				bp_camera_set_angle (camera, l->data->fval);
				break;
 			case DIRECTION:
				bp_camera_set_direction (camera, l->data->vector);
 				break;
			default:
				parse_error ("unsorported camera information");
				break;
			}
		}

	}
    break;

  case 46:
#line 444 "bp_script_parser.y"
    {
		parse_error ("Error in camera declaration\n");
	}
    break;

  case 48:
#line 452 "bp_script_parser.y"
    {
                 (yyvsp[-1].list)->next = (yyvsp[0].list);
		 (yyval.list) = (yyvsp[-1].list);
	 }
    break;

  case 49:
#line 456 "bp_script_parser.y"
    {
		 parse_error ("error here\n");
         }
    break;

  case 50:
#line 463 "bp_script_parser.y"
    { 
 		MAKE_VECTOR ((yyval.list), DIRECTION, (yyvsp[0].vector));
 	}
    break;

  case 51:
#line 467 "bp_script_parser.y"
    { 
		MAKE_VECTOR ((yyval.list), UP, (yyvsp[0].vector)); 
 	}
    break;

  case 52:
#line 471 "bp_script_parser.y"
    {
 		MAKE_VECTOR ((yyval.list), RIGHT, (yyvsp[0].vector));
 	}
    break;

  case 53:
#line 475 "bp_script_parser.y"
    {
		MAKE_VECTOR ((yyval.list), LOCATION, (yyvsp[0].vector));
	}
    break;

  case 54:
#line 479 "bp_script_parser.y"
    {
		MAKE_VECTOR ((yyval.list), LOOK_AT, (yyvsp[0].vector));
	}
    break;

  case 55:
#line 483 "bp_script_parser.y"
    {
 		MAKE_FLOAT ((yyval.list), ANGLE, (yyvsp[0].fval));
	}
    break;

  case 56:
#line 487 "bp_script_parser.y"
    {
		MAKE_VECTOR ((yyval.list), SKY, (yyvsp[0].vector));
	}
    break;

  case 57:
#line 491 "bp_script_parser.y"
    {
		parse_error ("invalid camera item\n");
		MAKE_ERROR ((yyval.list));
	}
    break;

  case 58:
#line 500 "bp_script_parser.y"
    {
 		(yyval.normal) = bp_normal_get_default (); 
	}
    break;

  case 59:
#line 504 "bp_script_parser.y"
    {
		image_t *im = bp_image_new ((yyvsp[-3].val), (yyvsp[-2].string));
		if (!im)
			parse_error ("bump map file %s not found\n", (yyvsp[-2].string));
		else 
			(yyval.normal) = bp_bump_map_new (im);
	}
    break;

  case 60:
#line 512 "bp_script_parser.y"
    {
 		parse_error ("error in bump map declartion");
	}
    break;

  case 61:
#line 520 "bp_script_parser.y"
    {
		ASSIGN ((yyval.vector5), (yyvsp[0].vector));
		(yyval.vector5) [3] = 1;
		(yyval.vector5) [4] = 0;
	}
    break;

  case 62:
#line 526 "bp_script_parser.y"
    {
		ASSIGN ((yyval.vector5), (yyvsp[0].vector));
		(yyval.vector5) [3] = 1;
		(yyval.vector5) [4] = 0;
	}
    break;

  case 63:
#line 532 "bp_script_parser.y"
    {
		ASSIGN ((yyval.vector5), (yyvsp[0].vector));
		(yyval.vector5) [3] = 1;
		(yyval.vector5) [4] = 0;
	}
    break;

  case 64:
#line 538 "bp_script_parser.y"
    {
		ASSIGN4 ((yyval.vector5), (yyvsp[0].vector4));
		(yyval.vector5) [4] = 0;
	}
    break;

  case 65:
#line 543 "bp_script_parser.y"
    { 
		ASSIGN5 ((yyval.vector5), (yyvsp[0].vector5));
	}
    break;

  case 66:
#line 547 "bp_script_parser.y"
    {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, (yyvsp[0].string));
		if (!res)
			parse_error ("Color %s not found", (yyvsp[0].string));
		else if (res->tag != COLOR)
			parse_error ("%s is not a color", (yyvsp[0].string));
		else
			ASSIGN5 ((yyval.vector5), res->data->vector5);
	}
    break;

  case 67:
#line 557 "bp_script_parser.y"
    {
		parse_error ("error in color declaration\n");
	}
    break;

  case 68:
#line 565 "bp_script_parser.y"
    {
		(yyval.finish) = bp_finish_get_default ();
	}
    break;

  case 69:
#line 569 "bp_script_parser.y"
    {
		struct list_t *l;

		(yyval.finish) = NEW_1 (finish_t, scene->material_pool);
		bp_finish_init ((yyval.finish));
		
		for (l = (yyvsp[-1].list); l; l = l->next) {
			switch (l->tag) {
			case AMBIENT:
				ASSIGN ((yyval.finish)->ambient, l->data->vector);
				break;
			case REFLECTION:
				(yyval.finish)->reflection = l->data->fval;
				break;
			case DIFFUSE:
				(yyval.finish)->diffuse    = l->data->fval;
				break;
			case PHONG:
				(yyval.finish)->phong      = l->data->fval;
				break;
			case PHONG_SIZE:
				(yyval.finish)->phong_size = l->data->val;
				break;
			case FRESNEL:
				(yyval.finish)->options   |= OPTION_FRESNEL;
				break;
			case METALLIC:
				(yyval.finish)->options   |= OPTION_METALLIC;
				break;
			}
		}
	}
    break;

  case 70:
#line 602 "bp_script_parser.y"
    {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, (yyvsp[-1].string));
		if (!res)
			parse_error ("Finish %s not found", (yyvsp[-1].string));
		else if (res->tag != FINISH)
			parse_error ("%s is not a Finish", (yyvsp[-1].string));
		else
			(yyval.finish) = res->data->finish;
	}
    break;

  case 72:
#line 615 "bp_script_parser.y"
    {
                 (yyvsp[-1].list)->next = (yyvsp[0].list);
		 (yyval.list) = (yyvsp[-1].list);
	 }
    break;

  case 73:
#line 619 "bp_script_parser.y"
    {
		 parse_error ("error in finish item here");
		 MAKE_ERROR ((yyval.list));
         }
    break;

  case 74:
#line 626 "bp_script_parser.y"
    {
		 MAKE_VECTOR ((yyval.list), AMBIENT, (yyvsp[0].vector));
	 }
    break;

  case 75:
#line 629 "bp_script_parser.y"
    {
		 MAKE_FLOAT ((yyval.list), REFLECTION, (yyvsp[0].fval));
	 }
    break;

  case 76:
#line 632 "bp_script_parser.y"
    {
		 MAKE_FLOAT ((yyval.list), DIFFUSE, (yyvsp[0].fval));
	 }
    break;

  case 77:
#line 635 "bp_script_parser.y"
    {
		 MAKE_FLOAT ((yyval.list), PHONG, (yyvsp[0].fval));
	 }
    break;

  case 78:
#line 638 "bp_script_parser.y"
    {
		 MAKE_INT ((yyval.list), PHONG_SIZE, (yyvsp[0].val));
	 }
    break;

  case 79:
#line 641 "bp_script_parser.y"
    {
		 MAKE_INT ((yyval.list), FRESNEL, (yyvsp[0].val));
	 }
    break;

  case 80:
#line 644 "bp_script_parser.y"
    {
		 MAKE_INT ((yyval.list), METALLIC, 1);
	 }
    break;

  case 81:
#line 652 "bp_script_parser.y"
    {
		(yyval.pigment) = NEW_1 (pigment_t, scene->material_pool);
		bp_pigment_set_solid_color ((yyval.pigment), (yyvsp[-1].vector5));
	}
    break;

  case 82:
#line 657 "bp_script_parser.y"
    {
		(yyval.pigment) = NEW_1 (pigment_t, scene->material_pool);
		bp_pigment_set_image_map ((yyval.pigment), (yyvsp[-1].image));
	}
    break;

  case 83:
#line 662 "bp_script_parser.y"
    {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, (yyvsp[-1].string));
		if (!res)
			parse_error ("Pigment %s not found", (yyvsp[-1].string));
		else if (res->tag == COLOR) {
			(yyval.pigment) = NEW_1 (pigment_t, scene->material_pool);
			bp_pigment_set_solid_color ((yyval.pigment), res->data->vector5);
		}
		else if (res->tag == PIGMENT) {
			(yyval.pigment) = res->data->pigment;
		}
		else
			parse_error ("%s is not a pigment or color", (yyvsp[-1].string));
	}
    break;

  case 84:
#line 677 "bp_script_parser.y"
    {
		parse_error ("pigment syntax error");
	}
    break;

  case 85:
#line 686 "bp_script_parser.y"
    {
 		(yyval.texture) = bp_texture_new ();
 		bp_texture_set_pigment ((yyval.texture), (yyvsp[-3].pigment));
 		bp_texture_set_finish ((yyval.texture), (yyvsp[-2].finish));
 		bp_texture_set_normal ((yyval.texture), (yyvsp[-1].normal));
	}
    break;

  case 86:
#line 693 "bp_script_parser.y"
    {
 		(yyval.texture) = bp_texture_new ();
 		bp_texture_set_pigment ((yyval.texture), (yyvsp[-2].pigment));
 		bp_texture_set_finish ((yyval.texture), (yyvsp[-3].finish));
 		bp_texture_set_normal ((yyval.texture), (yyvsp[-1].normal));
	}
    break;

  case 87:
#line 700 "bp_script_parser.y"
    {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, (yyvsp[-1].string));
		if (!res)
			parse_error ("Texture %s not found", (yyvsp[-1].string));
		else if (res->tag != TEXTURE)
			parse_error ("%s is not a Texture", (yyvsp[-1].string));
		else
			(yyval.texture) = res->data->texture;
	}
    break;

  case 88:
#line 710 "bp_script_parser.y"
    {
		parse_error ("error in texture definition\n");
	}
    break;

  case 89:
#line 727 "bp_script_parser.y"
    {
 		(yyval.material) = bp_material_new ();
 		bp_material_init ((yyval.material), (yyvsp[-3].texture),  (yyvsp[-1].fval));

	}
    break;

  case 90:
#line 733 "bp_script_parser.y"
    {
 		(yyval.material) = bp_material_new ();
 		bp_material_init ((yyval.material), (yyvsp[-1].texture), 0.0);
	}
    break;

  case 91:
#line 738 "bp_script_parser.y"
    {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, (yyvsp[-1].string));
		if (!res)
			parse_error ("Material %s not found", (yyvsp[-1].string));
		else if (res->tag != MATERIAL)
			parse_error ("%s is not a material", (yyvsp[-1].string));
		else
			(yyval.material) = res->data->material;
	}
    break;

  case 92:
#line 748 "bp_script_parser.y"
    {
		parse_error ("Error creating material");
	}
    break;

  case 94:
#line 756 "bp_script_parser.y"
    {
 		(yyval.material) = bp_material_new ();
 		bp_material_init ((yyval.material), (yyvsp[0].texture), 0.0);
	}
    break;

  case 95:
#line 765 "bp_script_parser.y"
    {
		 bp_sphere ((yyvsp[-3].vector), (yyvsp[-2].val), (yyvsp[-1].material));
	 }
    break;

  case 96:
#line 769 "bp_script_parser.y"
    {
		 parse_error ("error creating a sphere");
	 }
    break;

  case 97:
#line 778 "bp_script_parser.y"
    {
		 unsigned int obj_id;
		 obj_id = bp_object_generate_new ();
		 bp_begin1 (obj_id, BP_TRIANGLE);

  		 triangle_init ((yyvsp[-6].vector), (yyvsp[-4].vector), (yyvsp[-2].vector),
 				   0, 0, 1, 0, 0, 1);
		 bp_material ((yyvsp[-1].material));
		 bp_end ();
	 }
    break;

  case 98:
#line 789 "bp_script_parser.y"
    {
		 unsigned int obj_id;
		 obj_id = bp_object_generate_new ();
		 bp_begin1 (obj_id, BP_TRIANGLE);

 		 triangle_init ((yyvsp[-12].vector), (yyvsp[-10].vector), (yyvsp[-8].vector), 
 				   (yyvsp[-6].vector2) [0], (yyvsp[-6].vector2) [1], (yyvsp[-4].vector2) [0], (yyvsp[-4].vector2) [1], (yyvsp[-2].vector2) [0], 
				   (yyvsp[-2].vector2) [1]);
		 bp_material ((yyvsp[-1].material));
		 bp_end ();
	 }
    break;

  case 99:
#line 801 "bp_script_parser.y"
    {
		 parse_error ("error in creating a triangle\n");
	 }
    break;

  case 100:
#line 809 "bp_script_parser.y"
    {
		 unsigned int obj_id;
		 obj_id = bp_object_generate_new ();
		 bp_begin1 (obj_id, BP_TRIANGLE);

 		 smooth_triangle_init ((yyvsp[-12].vector), (yyvsp[-8].vector), (yyvsp[-4].vector),  (yyvsp[-10].vector), (yyvsp[-6].vector), (yyvsp[-2].vector),
		         0, 0, 1, 0, 0, 1);
		 bp_material ((yyvsp[-1].material));
		 bp_end ();
	 }
    break;

  case 101:
#line 820 "bp_script_parser.y"
    {
		 unsigned int obj_id;
		 obj_id = bp_object_generate_new ();
		 bp_begin1 (obj_id, BP_TRIANGLE);


 		 smooth_triangle_init ((yyvsp[-18].vector), (yyvsp[-10].vector), (yyvsp[-14].vector), (yyvsp[-16].vector), (yyvsp[-12].vector), (yyvsp[-8].vector),
			(yyvsp[-6].vector2) [0], (yyvsp[-6].vector2) [1], (yyvsp[-4].vector2) [0], (yyvsp[-4].vector2) [1], (yyvsp[-2].vector2) [0], (yyvsp[-2].vector2) [1]);
		 bp_material ((yyvsp[-1].material));
		 bp_end ();
	 }
    break;

  case 102:
#line 832 "bp_script_parser.y"
    {
		 parse_error ("error in creating a smooth triangle\n");
	 }
    break;

  case 103:
#line 840 "bp_script_parser.y"
    {
		bp_mesh_init (&mesh);
	}
    break;

  case 104:
#line 844 "bp_script_parser.y"
    {
		unsigned int obj_id = bp_object_generate_new ();
		bp_begin1 (obj_id, BP_KMESH);
		bp_mesh (&mesh);
		bp_material ((yyvsp[0].material));
		bp_end ();
		bp_mesh_destroy (&mesh);
	}
    break;

  case 106:
#line 857 "bp_script_parser.y"
    {
	       mesh_index = 0;
	       bp_mesh_set_num_vertices (&mesh, (yyvsp[0].val));
       }
    break;

  case 110:
#line 871 "bp_script_parser.y"
    {
      	       ASSIGN (mesh.vertices [mesh_index], (yyvsp[0].vector));
	       mesh_index ++;
       }
    break;

  case 111:
#line 879 "bp_script_parser.y"
    {
	       mesh_index = 0;
       }
    break;

  case 115:
#line 892 "bp_script_parser.y"
    {
      	       ASSIGN (mesh.normals [mesh_index], (yyvsp[0].vector));
	       mesh_index ++;
       }
    break;

  case 116:
#line 900 "bp_script_parser.y"
    {
	       mesh_index = 0;
	       bp_mesh_set_num_faces (&mesh, (yyvsp[0].val));
       }
    break;

  case 120:
#line 914 "bp_script_parser.y"
    {
      	       mesh.faces [mesh_index][0] = (yyvsp[-5].val);
      	       mesh.faces [mesh_index][1] = (yyvsp[-3].val);
      	       mesh.faces [mesh_index][2] = (yyvsp[-1].val);
	       mesh_index ++;
       }
    break;

  case 121:
#line 926 "bp_script_parser.y"
    {
		int obj_id = bp_object_generate_new ();
		bp_begin1 (obj_id, BP_MESH);
	}
    break;

  case 122:
#line 931 "bp_script_parser.y"
    {
		bp_material ((yyvsp[0].material));
	}
    break;

  case 123:
#line 935 "bp_script_parser.y"
    {
		bp_end ();
	}
    break;

  case 124:
#line 939 "bp_script_parser.y"
    {
		parse_error ("syntax error in mesh object");
	}
    break;

  case 125:
#line 945 "bp_script_parser.y"
    {}
    break;

  case 126:
#line 946 "bp_script_parser.y"
    {}
    break;

  case 127:
#line 947 "bp_script_parser.y"
    {
		 parse_error ("error in mesh item here");
		 MAKE_ERROR ((yyval.list));
         }
    break;

  case 130:
#line 960 "bp_script_parser.y"
    {
		 (yyval.material) = NULL;
	 }
    break;

  case 131:
#line 964 "bp_script_parser.y"
    {
		 (yyval.material) = bp_material_new ();
		 bp_material_init ((yyval.material), (yyvsp[0].texture), 0.0);
		 assert (0);
	 }
    break;

  case 132:
#line 973 "bp_script_parser.y"
    {
 		 triangle_init ((yyvsp[-6].vector), (yyvsp[-4].vector), (yyvsp[-2].vector),  
 				   0, 0, 1, 0, 0, 1);
	 }
    break;

  case 133:
#line 978 "bp_script_parser.y"
    {
 		 triangle_init ((yyvsp[-12].vector), (yyvsp[-10].vector), (yyvsp[-8].vector), 
 				   (yyvsp[-6].vector2) [0], (yyvsp[-6].vector2) [1], (yyvsp[-4].vector2) [0], (yyvsp[-4].vector2) [1], (yyvsp[-2].vector2) [0], 
				   (yyvsp[-2].vector2) [1]);
	 }
    break;

  case 134:
#line 984 "bp_script_parser.y"
    {
		 parse_error ("error in creating a mesh triangle\n");
	 }
    break;

  case 135:
#line 991 "bp_script_parser.y"
    {
 		 smooth_triangle_init ((yyvsp[-12].vector), (yyvsp[-8].vector), (yyvsp[-4].vector),  (yyvsp[-10].vector), (yyvsp[-6].vector), (yyvsp[-2].vector),
		         0, 0, 1, 0, 0, 1);
	 }
    break;

  case 136:
#line 996 "bp_script_parser.y"
    {
 		 smooth_triangle_init ((yyvsp[-18].vector), (yyvsp[-10].vector), (yyvsp[-14].vector), (yyvsp[-16].vector), (yyvsp[-12].vector), (yyvsp[-8].vector),
 			 (yyvsp[-6].vector2) [0], (yyvsp[-6].vector2) [1], (yyvsp[-4].vector2) [0], (yyvsp[-4].vector2) [1], (yyvsp[-2].vector2) [0], (yyvsp[-2].vector2) [1]);
	 }
    break;

  case 137:
#line 1001 "bp_script_parser.y"
    {
		 parse_error ("error in creating a mesh smooth triangle\n");
	 }
    break;

  case 138:
#line 1008 "bp_script_parser.y"
    {
	       (yyval.val) = bp_JPEG;
       }
    break;

  case 139:
#line 1012 "bp_script_parser.y"
    {
	       (yyval.val) = bp_PNG;
       }
    break;

  case 140:
#line 1016 "bp_script_parser.y"
    {
	       (yyval.val) = bp_GIF;
       }
    break;

  case 141:
#line 1023 "bp_script_parser.y"
    {
 	       (yyval.image) = bp_image_new ((yyvsp[-2].val), (yyvsp[-1].string));
 	       if (!(yyval.image))
		       parse_error1 ("image map could not be created from %s\n", (yyvsp[-1].string));
       }
    break;

  case 142:
#line 1029 "bp_script_parser.y"
    {
	       parse_error ("syntax error in image map \n");
       }
    break;

  case 143:
#line 1033 "bp_script_parser.y"
    {
	       parse_error ("syntax error in image map. \n");
       }
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 2614 "bp_script_parser.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1038 "bp_script_parser.y"


int bp_script_parse_file (scene_t *s)
{
	int res;
	char *dir;

	scene     = s;

	/* add the directory of the script file to the Libary Path*/
	dir = g_path_get_dirname (scene->settings.input_filename);
	bp_library_path_add1 (scene, dir);
	free (dir);

	if (!bp_script_lex_file (scene, scene->settings.input_filename)) {
		bp_report (BP_LOG_ERROR, "Script file `%s' not found\n", scene->settings.input_filename);
		return 1;
	}

	obj_table       = g_hash_table_new (g_str_hash, g_str_equal);
	tmppool         = bp_mempool_new ();

	bp_objects_alloc ();
	has_parse_error = 0;
	res             = yyparse ();

	g_hash_table_destroy (obj_table);
	bp_mempool_destroy (tmppool);

	if (res || has_parse_error)
		return 1;
	else
		return 0;
}

static void parse_error (char *str, ...) {
	va_list ap;
	va_start (ap, str);
	bp_report (BP_LOG_ERROR, "%s:%d: parse error:", bp_script_filename (), bp_script_lineno);
	bp_reportv (BP_LOG_ERROR, str, ap);
	va_end (ap);
	has_parse_error = 1;
}

static void parse_error1 (char *str, ...) {
	va_list ap;
	va_start (ap, str);
	bp_report (BP_LOG_ERROR, "%s:%d: parse error:", bp_script_filename (), bp_script_lineno);
	bp_reportv (BP_LOG_ERROR, str, ap);
	bp_report (BP_LOG_ERROR, "exiting ");
	va_end (ap);
	exit (1);
}

int yyerror (char *str) {
	parse_error (str);
	return 0;
}

