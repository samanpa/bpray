#include <mesh.h>
#include <bp_utils.h>

void
bp_mesh_init (bp_mesh_t *mesh)
{
	mesh->num_vertices = 0;
	mesh->num_faces = 0;
	mesh->vertices = NULL;
	mesh->normals = NULL;
	mesh->faces = NULL;
}

void
bp_mesh_set_num_vertices (bp_mesh_t *mesh, unsigned int num_vertices)
{
	/* Are there already vertices in this mesh */
	if (mesh->num_vertices)
		bp_report (BP_LOG_WARNING, "adding vertices to an existing mesh");

	mesh->num_vertices = num_vertices;
	NEW_2 (vector_t, mesh->vertices, num_vertices);
	NEW_2 (vector_t, mesh->normals, num_vertices);
}

void
bp_mesh_set_num_faces (bp_mesh_t *mesh,  unsigned int num_faces)
{
	/* Are there already faces in this mesh */
	if (mesh->num_faces)
		bp_report (BP_LOG_WARNING, "adding vertices to an existing mesh");
	
	mesh->num_faces = num_faces;
	NEW_2 (face_t, mesh->faces, num_faces);
}


void
bp_mesh_destroy (bp_mesh_t *mesh)
{
	if (mesh->num_vertices) {
		mesh->num_vertices = 0;
		free (mesh->vertices);
		free (mesh->normals);
	}

	if (mesh->num_faces) {
		mesh->num_faces = 0;
		free (mesh->faces);
	}
}
