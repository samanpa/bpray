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
     ANTI_ALIASING_DEPTH = 265,
     ASSUMED_GAMMA = 266,
     FALSE_LITERAL = 267,
     GIF = 268,
     INPUT_FILE_NAME = 269,
     JPEG = 270,
     LIBRARY_PATH = 271,
     MAX_TRACE_LEVEL = 272,
     PNG = 273,
     TRUE_LITERAL = 274
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
#define ANTI_ALIASING_DEPTH 265
#define ASSUMED_GAMMA 266
#define FALSE_LITERAL 267
#define GIF 268
#define INPUT_FILE_NAME 269
#define JPEG 270
#define LIBRARY_PATH 271
#define MAX_TRACE_LEVEL 272
#define PNG 273
#define TRUE_LITERAL 274




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 23 "bp_ini_parser.y"
typedef union YYSTYPE {
	float  fval;
	int    val;
        char  *string;
} YYSTYPE;
/* Line 1447 of yacc.c.  */
#line 82 "bp_ini_parser.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE bp_ini_lval;



