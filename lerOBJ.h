#ifndef LER_OBJ_H_
#define LER_OBJ_H_

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;


/* Vectors */
typedef float vec3_t[3];
typedef float vec4_t[4];

/* Vertex */
struct obj_vertex_t
{
	vec4_t xyzw;
};

/* Texture coordinates */
struct obj_texCoord_t
{
	vec3_t uvw;
};

/* Normal vector */
struct obj_normal_t
{
	vec3_t ijk;
};

/* Polygon */
struct obj_face_t
{
	GLenum type;        /* primitive type */
	int num_elems;      /* number of vertices */

	int *vert_indices;  /* vertex indices */
	int *uvw_indices;   /* texture coordinate indices */
	int *norm_indices;  /* normal vector indices */
};

/* OBJ model structure */
struct obj_model_t
{
	int num_verts;                     /* number of vertices */
	int num_texCoords;                 /* number of texture coords. */
	int num_normals;                   /* number of normal vectors */
	int num_faces;                     /* number of polygons */

	int has_texCoords;                 /* has texture coordinates? */
	int has_normals;                   /* has normal vectors? */

	struct obj_vertex_t *vertices;     /* vertex list */
	struct obj_texCoord_t *texCoords;  /* tex. coord. list */
	struct obj_normal_t *normals;      /* normal vector list */
	struct obj_face_t *faces;          /* model's polygons */
};




class LerOBJ
{
	public:
	
		LerOBJ();
		void FreeModel (struct obj_model_t *mdl);
		int MallocModel (struct obj_model_t *mdl);
		int FirstPass (FILE *fp, struct obj_model_t *mdl);
		int SecondPass (FILE *fp, struct obj_model_t *mdl);
		int ReadOBJModel (const char *filename, struct obj_model_t *mdl);
		void RenderOBJModel (struct obj_model_t *mdl);
		
		void cleanup(struct obj_model_t &objfile);

	
};

#endif /* LER_OBJ_H_ */