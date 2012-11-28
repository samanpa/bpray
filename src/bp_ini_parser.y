%defines
%verbose
%name-prefix="bp_ini_"

%{
#include "bp_ray.h"

/* declared in the lexer */
extern int bp_ini_lex ();
#define yylex bp_ini_lex
extern int bp_ini_lineno;
extern int bp_ini_lex_file (scene_t *, const char *);


static int has_parse_error;
static scene_t *scene;
static void parse_error (char *str, ...);
static int yyerror (char *str);

static char *ini_filename;
%}

%union {
	float  fval;
	int    val;
        char  *string;
}

%token <fval> FNUM
%token <val> NUM
%token <string> IDENTIFIER
%token <string> STRING

%token WIDTH
%token HEIGHT

%token ADC_BAILOUT
%token ANTI_ALIASING_DEPTH
%token ASSUMED_GAMMA
%token FALSE_LITERAL
%token GIF
%token INPUT_FILE_NAME
%token JPEG
%token LIBRARY_PATH
%token MAX_TRACE_LEVEL
%token PNG
%token TRUE_LITERAL
/*
%type <val>       bool

%type <fval>      fnum
*/
%start script

%%
/*
bool
        : TRUE_LITERAL        {
		$<val>$ = 1;
	}
        | FALSE_LITERAL        {
		$<val>$ = 0;
	}
;

fnum
        : FNUM        {
		$<fval>$ = $1;
	}
        | NUM        {
		$<fval>$ = $1;
	}
;
*/
script
        : ini_script
        | error
        {
		parse_error ("error parsing ini file %s", ini_filename);
	}
;

ini_script
        : ini_script_item
        | ini_script ini_script_item 
;

ini_script_item
        : WIDTH '=' NUM
        {
		bp_scene_get_horizontal_resolution (scene) = $3;
	}
        | HEIGHT '=' NUM
	{
		bp_scene_get_vertical_resolution (scene) = $3;
	}
	| LIBRARY_PATH     '=' STRING
        {
		bp_library_path_add (scene, $3);
	}
        | INPUT_FILE_NAME '=' STRING
        {
		scene->settings.input_filename = $3;
	}
	| ANTI_ALIASING_DEPTH '=' NUM
        {
		scene->settings.num_samples = $3;
	}
;

%%

int bp_ini_parse_file (const scene_t *s, const char *filename)
{
	scene     = (scene_t *)s;
	ini_filename = (char *)filename;
	if (!bp_ini_lex_file (scene, filename)) {
		bp_report (BP_LOG_ERROR, "INI file `%s' not found\n", filename);
		return 1;
	}

	if (yyparse () || has_parse_error) 
		return 1;
	else
		return 0;
}

static void parse_error (char *str, ...) {
	va_list ap;
	va_start (ap, str);
	bp_report (BP_LOG_ERROR, "%s:%d: parse error:", ini_filename, bp_ini_lineno);
	bp_reportv (BP_LOG_ERROR, str, ap);
	va_end (ap);
	has_parse_error = 1;
}

static int yyerror (char *str) {
	parse_error (str);
	return 0;
}
