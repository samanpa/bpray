%defines
%verbose

%{
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

%}

%union {
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
}

%token <fval> FNUM
%token <val> NUM
%token <string> IDENTIFIER
%token <string> STRING

%token WIDTH
%token HEIGHT

%token ADC_BAILOUT
%token AMBIENT
%token AMBIENT_LIGHT
%token ANGLE
%token ANTI_ALIASING_DEPTH
%token ASSUMED_GAMMA
%token BACKGROUND
%token BUMP_MAP
%token CAMERA
%token COLOR
%token DECLARE
%token DIFFUSE
%token DIRECTION
%token FACE_INDICES
%token FALSE_LITERAL
%token FINISH
%token FILTER
%token FRESNEL
%token GIF
%token GLOBAL_SETTINGS
%token INPUT_FILE_NAME
%token IOR
%token JPEG
%token KMESH
%token IMAGE_MAP
%token INTERIOR_TEXTURE
%token LIBRARY_PATH
%token LIGHT_SOURCE
%token LOCATION
%token LOOK_AT
%token MATERIAL
%token MAX_TRACE_LEVEL
%token MESH
%token METALLIC
%token NORMAL
%token NORMALS
%token OUTPUT_FILE_NAME
%token OUTPUT_FILE_TYPE
%token SPECULAR
%token SHININESS
%token PIGMENT
%token PNG
%token POSITION
%token REFLECTION
%token BP_RGB
%token RIGHT
%token ROTATE
%token SMOOTH_TRIANGLE
%token SPECULAR
%token SKY
%token SPHERE
%token TEXTURE
%token TRANSLATE
%token TRIANGLE
%token TRANSMIT
%token TRUE_LITERAL
%token UP
%token UV_VECTORS
%token UV_MAPPING
%token VERTICES

%type <val>       bool
%type <list>      bpr_item

%type <list>      camera_item
%type <list>      camera_items
%type <vector5>   color

%type <fval>      fnum
%type <finish>    finish
%type <list>      finish_item
%type <list>      finish_items

%type <image>     image_map
%type <val>       image_type
%type <material>  material
%type <material>  _material
%type <material>  mesh_object_material
%type <list>      mesh_object
%type <list>      mesh_objects
%type <list>      mesh_triangle
%type <list>      mesh_smooth_triangle
%type <normal>    normal
%type <pigment>   pigment
%type <texture>   texture
/*
%type <texture>   interior_texture
*/
%type <vector>    vector
%type <vector2>   vector2
%type <vector4>   vector4
%type <vector5>   vector5

%start script

%%

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

vector2
        :  '<' fnum ',' fnum '>'        {
		$<vector>$ [0] = $2;
		$<vector>$ [1] = $4;
	}
;

vector
        :  '<' fnum ',' fnum ',' fnum '>'        {
		$<vector>$ [0] = $2;
		$<vector>$ [1] = $4;
		$<vector>$ [2] = $6;
	}
        | fnum
        {
		$<vector>$ [0] = $<vector>$ [1] = $<vector>$ [2] = $1;
        }
        | '<' fnum error '>'        {
		parse_error ("bad vector declaration");
	}
;

vector4
        : '<' fnum ',' fnum ',' fnum ',' fnum '>'
        {
		$<vector4>$ [0] = $2;
		$<vector4>$ [1] = $4;
		$<vector4>$ [2] = $6;
		$<vector4>$ [3] = $8;
	}
;		


vector5
        : '<' fnum ',' fnum ',' fnum ',' fnum ',' fnum '>'
        {
		$<vector4>$ [0] = $2;
		$<vector4>$ [1] = $4;
		$<vector4>$ [2] = $6;
		$<vector4>$ [3] = $8;
		$<vector4>$ [4] = $10;
	}
;


script
        : bpr_statements
        | error
        {
		parse_error ("error parsing script file");
	}
;

bpr_statements
        : statement
        | bpr_statements statement
;

statement
        : declare_statement
        | object_statement
        | background
        | global_settings
        | camera_decl
;

declare_statement
        : DECLARE IDENTIFIER '=' bpr_item
        {
		if (g_hash_table_lookup (obj_table, $2))
			parse_error ("Object %s already declared\n", $2);
		g_hash_table_insert (obj_table, $2, $4);
	}
;

background
        : BACKGROUND '{' vector '}'
        {
		ASSIGN (scene->settings.atmosphere.background, $3);
	}
        | BACKGROUND '{' color '}'
        {
		ASSIGN (scene->settings.atmosphere.background, $3);
	}
;

object_statement
        : bpr_item
        | light
        | sphere
        | triangle
        | smooth_triangle
        | mesh
        | kmesh
;

bpr_item
        : finish
        {
		NEW_LIST ($<list>$, FINISH);
		$<list>$->data->finish = $1;
	}
        | _material
        {
		NEW_LIST ($<list>$, MATERIAL);
		$<list>$->data->material = $1;
	}
        | texture
        {
		NEW_LIST ($<list>$, TEXTURE);
		$<list>$->data->texture = $1;
	}
        | color
        {
		NEW_LIST ($<list>$, COLOR);
		ASSIGN5 ($<list>$->data->vector5, $1);
	}
;

light
         : LIGHT_SOURCE '{' vector color '}' {
         	 bp_light_new ($3, $4);
	 }
         | LIGHT_SOURCE error '}' {
		 parse_error ("Error creating light\n");
	 }
;


global_settings
        : GLOBAL_SETTINGS '{' global_settings_items '}'
;

global_settings_items
        : global_settings_item
        | global_settings_items global_settings_item
;

global_settings_item
        : ADC_BAILOUT fnum
        {
		scene->settings.adc_bailout = $2;
	}
        | AMBIENT_LIGHT vector
        {
		ASSIGN (scene->settings.atmosphere.ambient_light, $2);
	}
        | MAX_TRACE_LEVEL NUM
        {
		scene->settings.max_trace_level = $2;
	}
        | ASSUMED_GAMMA fnum
        {
		scene->settings.gamma = $2;
	}
        | error
        {
		parse_error ("error in global settings declaration");
	}
;

camera_decl
        : CAMERA '{' camera_items '}'
        {
		struct list_t *l;
		camera_t *camera = &scene->camera;

		for (l = $3; l; l = l->next) {
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
        | CAMERA error '}'
        {
		parse_error ("Error in camera declaration\n");
	}
;

camera_items
         : camera_item 
         | camera_item camera_items
         {
                 $1->next = $2;
		 $<list>$ = $1;
	 }
         | error {
		 parse_error ("error here\n");
         }
;

camera_item
        :  DIRECTION vector
        { 
 		MAKE_VECTOR ($<list>$, DIRECTION, $2);
 	} 
        |  UP vector
        { 
		MAKE_VECTOR ($<list>$, UP, $2); 
 	} 
        | RIGHT vector
        {
 		MAKE_VECTOR ($<list>$, RIGHT, $2);
 	} 
        |  LOCATION vector
        {
		MAKE_VECTOR ($<list>$, LOCATION, $2);
	}
        | LOOK_AT vector
        {
		MAKE_VECTOR ($<list>$, LOOK_AT, $2);
	}
        | ANGLE fnum
        {
 		MAKE_FLOAT ($<list>$, ANGLE, $2);
	}
        | SKY vector
        {
		MAKE_VECTOR ($<list>$, SKY, $2);
	}
        | error
        {
		parse_error ("invalid camera item\n");
		MAKE_ERROR ($<list>$);
	}
;

/* normal definition */
normal
        :
        {
 		$<normal>$ = bp_normal_get_default (); 
	}
        | NORMAL '{' BUMP_MAP '{' image_type STRING '}' '}'
        {
		image_t *im = bp_image_new ($5, $6);
		if (!im)
			parse_error ("bump map file %s not found\n", $6);
		else 
			$<normal>$ = bp_bump_map_new (im);
	}
        | NORMAL '{' error '}'
        {
 		parse_error ("error in bump map declartion");
	}
;

/* color definition */
color
        : COLOR vector
        {
		ASSIGN ($<vector5>$, $2);
		$<vector5>$ [3] = 1;
		$<vector5>$ [4] = 0;
	}
        | BP_RGB vector
        {
		ASSIGN ($<vector5>$, $2);
		$<vector5>$ [3] = 1;
		$<vector5>$ [4] = 0;
	}
        | COLOR BP_RGB vector
        {
		ASSIGN ($<vector5>$, $3);
		$<vector5>$ [3] = 1;
		$<vector5>$ [4] = 0;
	}
        | COLOR vector4
        {
		ASSIGN4 ($<vector5>$, $2);
		$<vector5>$ [4] = 0;
	}
        | COLOR vector5
        { 
		ASSIGN5 ($<vector5>$, $2);
	}
        | COLOR IDENTIFIER
        {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, $2);
		if (!res)
			parse_error ("Color %s not found", $2);
		else if (res->tag != COLOR)
			parse_error ("%s is not a color", $2);
		else
			ASSIGN5 ($<vector5>$, res->data->vector5);
	}
        | COLOR error
        {
		parse_error ("error in color declaration\n");
	}		
;

/* FINISH Definition */
finish
        : 
        {
		$<finish>$ = bp_finish_get_default ();
	}
        | FINISH '{' finish_items '}'
        {
		struct list_t *l;

		$<finish>$ = NEW_1 (finish_t, scene->material_pool);
		bp_finish_init ($<finish>$);
		
		for (l = $3; l; l = l->next) {
			switch (l->tag) {
			case AMBIENT:
				ASSIGN ($<finish>$->ambient, l->data->vector);
				break;
			case REFLECTION:
				$<finish>$->reflection = l->data->fval;
				break;
			case DIFFUSE:
				$<finish>$->diffuse    = l->data->fval;
				break;
			case SPECULAR:
				$<finish>$->specular   = l->data->fval;
				break;
			case SHININESS:
				$<finish>$->shininess = l->data->val;
				break;
			case FRESNEL:
				$<finish>$->options   |= OPTION_FRESNEL;
				break;
			case METALLIC:
				$<finish>$->options   |= OPTION_METALLIC;
				break;
			}
		}
	}
        | FINISH '{' IDENTIFIER '}'
        {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, $3);
		if (!res)
			parse_error ("Finish %s not found", $3);
		else if (res->tag != FINISH)
			parse_error ("%s is not a Finish", $3);
		else
			$<finish>$ = res->data->finish;
	}
;

finish_items
         : finish_item
         | finish_item finish_items {
                 $1->next = $2;
		 $<list>$ = $1;
	 }
         | error {
		 parse_error ("error in finish item here");
		 MAKE_ERROR ($$);
         }
;

finish_item
        : AMBIENT vector {
		 MAKE_VECTOR ($<list>$, AMBIENT, $2);
	 }
         | REFLECTION fnum {
		 MAKE_FLOAT ($<list>$, REFLECTION, $2);
	 }
         | DIFFUSE fnum {
		 MAKE_FLOAT ($<list>$, DIFFUSE, $2);
	 }
         | SPECULAR fnum {
		 MAKE_FLOAT ($<list>$, SPECULAR, $2);
	 }
         | SHININESS NUM {
		 MAKE_INT ($<list>$, SHININESS, $2);
	 }
         | FRESNEL bool {
		 MAKE_INT ($<list>$, FRESNEL, $2);
	 }
         | METALLIC {
		 MAKE_INT ($<list>$, METALLIC, 1);
	 }
;

/* pigment definition */
pigment 
        : PIGMENT '{' color '}'
        {
		$<pigment>$ = NEW_1 (pigment_t, scene->material_pool);
		bp_pigment_set_solid_color ($<pigment>$, $3);
	}
        | PIGMENT '{' image_map '}' 
        {
		$<pigment>$ = NEW_1 (pigment_t, scene->material_pool);
		bp_pigment_set_image_map ($<pigment>$, $3);
	}
        | PIGMENT '{' IDENTIFIER '}' 
        {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, $3);
		if (!res)
			parse_error ("Pigment %s not found", $3);
		else if (res->tag == COLOR) {
			$<pigment>$ = NEW_1 (pigment_t, scene->material_pool);
			bp_pigment_set_solid_color ($<pigment>$, res->data->vector5);
		}
		else if (res->tag == PIGMENT) {
			$<pigment>$ = res->data->pigment;
		}
		else
			parse_error ("%s is not a pigment or color", $3);
	}
        | PIGMENT error
        {
		parse_error ("pigment syntax error");
	}
;


/* texture definition */
texture
        : TEXTURE '{' pigment finish normal '}'
        {
 		$<texture>$ = bp_texture_new ();
 		bp_texture_set_pigment ($<texture>$, $3);
 		bp_texture_set_finish ($<texture>$, $4);
 		bp_texture_set_normal ($<texture>$, $5);
	}
        | TEXTURE '{' finish pigment normal '}'
        {
 		$<texture>$ = bp_texture_new ();
 		bp_texture_set_pigment ($<texture>$, $4);
 		bp_texture_set_finish ($<texture>$, $3);
 		bp_texture_set_normal ($<texture>$, $5);
	}
        | TEXTURE '{' IDENTIFIER '}'
        {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, $3);
		if (!res)
			parse_error ("Texture %s not found", $3);
		else if (res->tag != TEXTURE)
			parse_error ("%s is not a Texture", $3);
		else
			$<texture>$ = res->data->texture;
	}
        | TEXTURE error 
        {
		parse_error ("error in texture definition\n");
	}
;


/* MATERIAL definition */
/* This is split into 2 to avoid reduce-reduce errors. The error occurs if things
   are declared like:

       material -> texture
       
       declare  -> #declare identifier = material
       declare  -> #declare identifier = texture
*/
_material
        :  MATERIAL '{' texture IOR fnum '}'
        {
 		$<material>$ = bp_material_new ();
 		bp_material_init ($<material>$, $3,  $5);

	}
        | MATERIAL '{' texture '}'
        {
 		$<material>$ = bp_material_new ();
 		bp_material_init ($<material>$, $3, 0.0);
	}
        | MATERIAL '{' IDENTIFIER '}'
        {
		struct list_t *res = (struct list_t *)g_hash_table_lookup (obj_table, $3);
		if (!res)
			parse_error ("Material %s not found", $3);
		else if (res->tag != MATERIAL)
			parse_error ("%s is not a material", $3);
		else
			$<material>$ = res->data->material;
	}
        | MATERIAL '{' error
        {
		parse_error ("Error creating material");
	}
;

material
        : _material
        | texture 
        {
 		$<material>$ = bp_material_new ();
 		bp_material_init ($<material>$, $1, 0.0);
	}
;

/* sphere definition */
sphere
         : SPHERE '{' vector NUM material '}'
         {
		 bp_sphere ($3, $4, $5);
	 }
         | SPHERE error '}'
         {
		 parse_error ("error creating a sphere");
	 }

;

/* triangle definition */
triangle
         : TRIANGLE '{' vector ',' vector ',' vector material '}'
         {
		 unsigned int obj_id;
		 obj_id = bp_object_generate_new ();
		 bp_begin1 (obj_id, BP_TRIANGLE);

  		 triangle_init ($3, $5, $7,
 				   0, 0, 1, 0, 0, 1);
		 bp_material ($8);
		 bp_end ();
	 }
         | TRIANGLE '{' vector ',' vector ',' vector UV_VECTORS vector2 ',' vector2 ',' vector2 material '}'
         {
		 unsigned int obj_id;
		 obj_id = bp_object_generate_new ();
		 bp_begin1 (obj_id, BP_TRIANGLE);

 		 triangle_init ($3, $5, $7, 
 				   $9 [0], $9 [1], $11 [0], $11 [1], $13 [0], 
				   $13 [1]);
		 bp_material ($14);
		 bp_end ();
	 }
         | TRIANGLE error '}'
         {
		 parse_error ("error in creating a triangle\n");
	 }
;

/* smooth triangle definition */
smooth_triangle
         : SMOOTH_TRIANGLE '{' vector ',' vector ',' vector ',' vector ',' vector ',' vector material '}'
         {
		 unsigned int obj_id;
		 obj_id = bp_object_generate_new ();
		 bp_begin1 (obj_id, BP_TRIANGLE);

 		 smooth_triangle_init ($3, $7, $11,  $5, $9, $13,
		         0, 0, 1, 0, 0, 1);
		 bp_material ($14);
		 bp_end ();
	 }
         | SMOOTH_TRIANGLE '{' vector ',' vector ',' vector ',' vector ',' vector ',' vector UV_VECTORS vector2 ',' vector2 ',' vector2 material '}'
         {
		 unsigned int obj_id;
		 obj_id = bp_object_generate_new ();
		 bp_begin1 (obj_id, BP_TRIANGLE);


 		 smooth_triangle_init ($3, $11, $7, $5, $9, $13,
			$15 [0], $15 [1], $17 [0], $17 [1], $19 [0], $19 [1]);
		 bp_material ($20);
		 bp_end ();
	 }
         | SMOOTH_TRIANGLE error '}'
         {
		 parse_error ("error in creating a smooth triangle\n");
	 }
;

/* kmesh definiation */
kmesh
        : KMESH '{'
        {
		bp_mesh_init (&mesh);
	} 
           kvertices knormals face_indices material
        {
		unsigned int obj_id = bp_object_generate_new ();
		bp_begin1 (obj_id, BP_KMESH);
		bp_mesh (&mesh);
		bp_material ($7);
		bp_end ();
		bp_mesh_destroy (&mesh);
	}
          '}'
;

kvertices
       : VERTICES '{' NUM
       {
	       mesh_index = 0;
	       bp_mesh_set_num_vertices (&mesh, $3);
       }
          _kvertices '}'
;

_kvertices
       : _kvertices kvertex
       | kvertex
;

kvertex
       : vector 
       {
      	       ASSIGN (mesh.vertices [mesh_index], $1);
	       mesh_index ++;
       }
;

knormals
       :  NORMALS '{' NUM
       {
	       mesh_index = 0;
       }
          _knormals '}'
;

_knormals
       : _knormals knormal
       | knormal
;

knormal
       : vector 
       {
      	       ASSIGN (mesh.normals [mesh_index], $1);
	       mesh_index ++;
       }
;

face_indices
       :  FACE_INDICES '{' NUM
       {
	       mesh_index = 0;
	       bp_mesh_set_num_faces (&mesh, $3);
       }
          _face_indices '}'
;

_face_indices
       : _face_indices face_index
       | face_index
;

face_index
       : '<' NUM ',' NUM ',' NUM '>'
       {
      	       mesh.faces [mesh_index][0] = $2;
      	       mesh.faces [mesh_index][1] = $4;
      	       mesh.faces [mesh_index][2] = $6;
	       mesh_index ++;
       }
;


/* mesh definition */
mesh
        : MESH '{'
        {
		int obj_id = bp_object_generate_new ();
		bp_begin1 (obj_id, BP_MESH);
	}
               mesh_objects material
        {
		bp_material ($5);
	}
          '}'
        {
		bp_end ();
	}
        | MESH error '}'
        {
		parse_error ("syntax error in mesh object");
	}
;

mesh_objects
         : mesh_object {}
         | mesh_object mesh_objects {}
         | error {
		 parse_error ("error in mesh item here");
		 MAKE_ERROR ($$);
         }
;

mesh_object
         : mesh_triangle
         | mesh_smooth_triangle
; 

mesh_object_material
         :
         {
		 $<material>$ = NULL;
	 }
         | texture
         {
		 $<material>$ = bp_material_new ();
		 bp_material_init ($<material>$, $1, 0.0);
		 assert (0);
	 }
;

mesh_triangle
         : TRIANGLE '{' vector ',' vector ',' vector mesh_object_material '}'
         {
 		 triangle_init ($3, $5, $7,  
 				   0, 0, 1, 0, 0, 1);
	 }
         | TRIANGLE '{' vector ',' vector ',' vector UV_VECTORS vector2 ',' vector2 ',' vector2 mesh_object_material '}'
         {
 		 triangle_init ($3, $5, $7, 
 				   $9 [0], $9 [1], $11 [0], $11 [1], $13 [0], 
				   $13 [1]);
	 }
         | TRIANGLE error '}'
         {
		 parse_error ("error in creating a mesh triangle\n");
	 }
;

mesh_smooth_triangle
         : SMOOTH_TRIANGLE '{' vector ',' vector ',' vector ',' vector ',' vector ',' vector mesh_object_material '}'
         {
 		 smooth_triangle_init ($3, $7, $11,  $5, $9, $13,
		         0, 0, 1, 0, 0, 1);
	 }
         | SMOOTH_TRIANGLE '{' vector ',' vector ',' vector ',' vector ',' vector ',' vector UV_VECTORS vector2 ',' vector2 ',' vector2 mesh_object_material '}'
         {
 		 smooth_triangle_init ($3, $11, $7, $5, $9, $13,
 			 $15 [0], $15 [1], $17 [0], $17 [1], $19 [0], $19 [1]);
	 }
         | SMOOTH_TRIANGLE error '}'
         {
		 parse_error ("error in creating a mesh smooth triangle\n");
	 }
;


image_type
       : JPEG {
	       $$ = bp_JPEG;
       }
       | PNG
       {
	       $$ = bp_PNG;
       }
       | GIF
       {
	       $$ = bp_GIF;
       }
;

image_map
       : IMAGE_MAP '{' image_type STRING '}'
       {
 	       $<image>$ = bp_image_new ($3, $4);
 	       if (!$<image>$)
		       parse_error1 ("image map could not be created from %s\n", $4);
       }
       | IMAGE_MAP '{' error '}'
       {
	       parse_error ("syntax error in image map \n");
       }
       | IMAGE_MAP error 
       {
	       parse_error ("syntax error in image map. \n");
       }
;

%%

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
