#pragma once

#include "Globals.h"
#include "Module.h"
#include "p2Point.h"

#include "Application.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include "glmath.h"
#include "glut/glut.h"

struct OBJModel {
	const char* path;
	vec3 outVertices;
	vec3 out_uvs;
	vec3 out_normals;
};


class OBJLoader : public Module{
public:

	bool LoadOBJ(const char* pathvec3, vec3 &outVertices, vec2 &out_uvs, vec3 &out_normals);

	bool Update();

private:

};

