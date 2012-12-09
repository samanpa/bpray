%{
#include <bp_ray.h>
#include <bp_script_parser.h>

int bp_script_lex_file (scene_t *scene, const char *filename);
%}

DIGIT                 [0-9]
DIGITS               {DIGIT}+
IDENT_START_CHAR     [a-zA-Z]
IDENT_CHAR           [0-9a-zA-Z_]
COMMENT              "/"\*([^*]|[\n]|(\*+([^*/]|[\n])))*\*+"/"
%option yylineno
%option prefix="bp_script_" outfile="lex.yy.c"
%%

__dummy__rule_to_get_rid_of_warnings__ {
	REJECT;

	yy_flex_realloc (NULL, 1);
	unput (4);
}

-?{DIGITS} {
	yylval.val = atoi (yytext);
	return NUM;
}

[-+]?{DIGITS}\.?{DIGITS}?([eE][-+]{DIGITS})? {
	yylval.fval = atof (yytext);
	return FNUM;
}

adc_bailout {
        return ADC_BAILOUT;
}

ambient {
        return AMBIENT;
}

ambient_light {
        return AMBIENT_LIGHT;
}

angle {
        return ANGLE;
}

assumed_gamma {
        return ASSUMED_GAMMA;
}

Antialias_Depth {
	return ANTI_ALIASING_DEPTH;
}

background {
        return BACKGROUND;
}

bump_map {
        return BUMP_MAP;
}

camera {
	return CAMERA;
}

color {
	return COLOR;
}

#declare {
        return DECLARE;
}

diffuse {
        return DIFFUSE;
}

direction {
        return DIRECTION;
}

face_indices {
	return FACE_INDICES;
}

false {
        return FALSE_LITERAL;
}

filter {
        return FILTER;
}

finish {
        return FINISH;
}

fresnel {
        return FRESNEL;
}

global_settings {
        return GLOBAL_SETTINGS;
}

gif {
        return GIF;
}

height|Height {
	return HEIGHT;
}

image_map {
        return IMAGE_MAP;
}

#include {
	int next_token = yylex ();
	if (next_token == STRING) {
 		if (!bp_script_lex_file (curr_scene, yylval.string)) {
 			bp_report (BP_LOG_ERROR, "Script file `%s' not found", yylval.string);
 		}
 	} 
	else {
		bp_report (BP_LOG_ERROR, "filename expected after include");
	}
        return yylex ();

}

Input_File_Name {
        return INPUT_FILE_NAME;
}

interior_texture {
        return INTERIOR_TEXTURE;
}

ior {
        return IOR;
}

jpg {
        return JPEG;
}

jpeg {
        return JPEG;
}

kmesh {
	return KMESH;
}

Library_Path {
	return LIBRARY_PATH;
}

light_source {
        return LIGHT_SOURCE;
}

location {
        return LOCATION;
}

look_at {
        return LOOK_AT;
}

material {
        return MATERIAL;
}

max_trace_level {
        return MAX_TRACE_LEVEL;
}

mesh {
        return MESH;
}

metallic {
        return METALLIC;
}

normal {
        return NORMAL;
}

normals {
        return NORMALS;
}

Output_File_Name {
	return OUTPUT_FILE_NAME;
}

Output_File_Type {
	return OUTPUT_FILE_TYPE;
}

specular {
        return SPECULAR;
}

shininess {
        return SHININESS;
}

pigment {
        return PIGMENT;
}

png {
        return PNG;
}

position {
        return POSITION;
}

reflection {
        return REFLECTION;
}

rgb {
	return BP_RGB;
}

right {
        return RIGHT;
}

rotate {
        return ROTATE;
}

sky {
        return SKY;
}

smooth_triangle {
        return SMOOTH_TRIANGLE;
}

specular {
        return SPECULAR;
}

sphere {
	return SPHERE;
}

texture {
        return TEXTURE;
}

transmit {
        return TRANSMIT;
}

translate {
        return TRANSLATE;
}

triangle {
        return TRIANGLE;
}

true {
        return TRUE_LITERAL;
}

up {
        return UP;
}

uv_vectors {
         return UV_VECTORS;
}

uv_mapping {
          return UV_MAPPING;
}

vertices {
	return VERTICES;
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
        yylval.string = strdup (yytext + 1);
        yylval.string [strlen (yylval.string) - 1] = '\0';
        return STRING;
}

{IDENT_START_CHAR}{IDENT_CHAR}* {
        yylval.string = strdup (yytext);
        return IDENTIFIER;
}

. {
	return yytext [0];
}

%%

struct input_t {
	struct yy_buffer_state *state;
	int lineno;
	char *filename;
};

struct input_t inputs[20];
int num_inputs;

int yywrap () {
	--num_inputs;
 	fclose (inputs [num_inputs].state->yy_input_file);
	yy_delete_buffer (inputs [num_inputs].state);
	free (inputs [num_inputs].filename);

	if (num_inputs) {
		yy_switch_to_buffer (inputs [num_inputs - 1].state);
	        yylineno = inputs [num_inputs].lineno;
		return 0;
	} else {
		return 1;
        }
}

char * bp_script_filename ()
{
        return inputs [num_inputs - 1].filename;
}

int bp_script_lex_file (scene_t *scene, const char *filename)
{
        FILE *fp;

	fp = bp_file_open (scene, filename, "r");
	if (!fp)
		return 0;
 	else {
		yyin = fp;

		inputs [num_inputs].lineno   = yylineno;
		inputs [num_inputs].state    = yy_create_buffer (yyin, YY_BUF_SIZE);
		inputs [num_inputs].filename = strdup (filename);
		
		yy_switch_to_buffer (inputs [num_inputs].state);

		yylineno = 1;
		num_inputs ++;
		return 1;
	}
}
