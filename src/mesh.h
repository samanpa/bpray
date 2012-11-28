#ifndef BP_MESH_H
#define BP_MESH_H

#include <stdint.h>
#include <bp_types.h>

typedef uint32_t face_t [3];
typedef struct {
	uint32_t num_vertices;
	uint32_t num_faces;
	vector_t *vertices;
	vector_t *normals;
	face_t *faces ;
} bp_mesh_t;

void
bp_mesh_init (bp_mesh_t *mesh);

void
bp_mesh_set_num_vertices (bp_mesh_t *mesh, unsigned int num_vertics);

void
bp_mesh_set_num_faces (bp_mesh_t *mesh,  unsigned int num_vertices);

void
bp_mesh_destroy (bp_mesh_t *mesh);
#endif
