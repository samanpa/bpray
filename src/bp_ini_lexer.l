%{
#include <bp_ray.h>
#include <bp_ini_parser.h>
%}

DIGIT                 [0-9]
DIGITS               {DIGIT}+
IDENT_START_CHAR     [a-zA-Z]
IDENT_CHAR           [0-9a-zA-Z_]
COMMENT              "/"\*([^*]|[\n]|(\*+([^*/]|[\n])))*\*+"/"
%option yylineno
%option noyywrap
%option prefix="bp_ini_" outfile="lex.yy.c"

%%

__dummy__rule_to_get_rid_of_warnings__ {
	REJECT;

	yy_flex_realloc (NULL, 1);
	unput (4);
}

-?{DIGITS} {
	bp_ini_lval.val = atoi (yytext);
	return NUM;
}

[-+]?{DIGITS}\.?{DIGITS}?([eE][-+]{DIGITS})? {
	bp_ini_lval.fval = atof (yytext);
	return FNUM;
}

adc_bailout {
        return ADC_BAILOUT;
}

assumed_gamma {
        return ASSUMED_GAMMA;
}

Antialias_Depth {
	return ANTI_ALIASING_DEPTH;
}

false {
        return FALSE_LITERAL;
}

gif {
        return GIF;
}

height|Height {
	return HEIGHT;
}

Input_File_Name {
        return INPUT_FILE_NAME;
}

jpg {
        return JPEG;
}

jpeg {
        return JPEG;
}

BANNA_LIBRARY_PATH {
	return LIBRARY_PATH;
}

max_trace_level {
        return MAX_TRACE_LEVEL;
}

png {
        return PNG;
}

true {
        return TRUE_LITERAL;
}

width|Width {
	return WIDTH;
}


" "|"\n"|"\r\n"|"\t"  {
        return yylex ();
}

"//".*"\n" {
         return yylex ();
}

{COMMENT} {
         return yylex ();
}

\".*\" {
        bp_ini_lval.string = strdup (yytext + 1);
        bp_ini_lval.string [strlen (bp_ini_lval.string) - 1] = '\0';
        return STRING;
}

{IDENT_START_CHAR}{IDENT_CHAR}* {
        bp_ini_lval.string = strdup (yytext);
        return IDENTIFIER;
}

. {
	return yytext [0];
}

%%
int bp_ini_lex_file (scene_t *scene, const char *filename)
{
        FILE *fp;

	fp = fopen (filename, "r");
	if (!fp)
		return 0;
	else {
		yyin = fp;
		return 1;
	}
}
