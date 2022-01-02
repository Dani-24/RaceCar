#include "OBJLoader.h"

bool OBJLoader::LoadOBJ(const char* path, vec3 &out_Vertices, vec2 &out_uvs, vec3 &out_normals) {
	 
	// Variables
	p2List <uint> vertexIndices, uvIndices, normalIndices;
	p2List <vec3> temp_vertices;
	p2List <vec2> temp_uvs;
	p2List <vec3> temp_normals;

	// Load the file
	FILE* file = fopen(path, "r");

	if (file == NULL) {
		LOG("OBJLoader ERROR: Impossible to open the file !");
		return false;
	}

	// Read the File
	while (1) {
		char lineHeader[128];

		int res = fscanf_s(file, "%s", lineHeader);
		if (res = EOF) {
			break;
		}

		// Vertices
		if (strcmp(lineHeader, "v") == 0) {
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.add(vertex);
		}
		// Uvs
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.add(uv);
		}
		// Normals
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.add(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			const char* vertex1,* vertex2,* vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.add(vertexIndex[0]);
			vertexIndices.add(vertexIndex[1]);
			vertexIndices.add(vertexIndex[2]);
			uvIndices.add(uvIndex[0]);
			uvIndices.add(uvIndex[1]);
			uvIndices.add(uvIndex[2]);
			normalIndices.add(normalIndex[0]);
			normalIndices.add(normalIndex[1]);
			normalIndices.add(normalIndex[2]);
		}
	}

	// Process the data

	p2List_item <uint>* vertexIndex = vertexIndices.getFirst();
	p2List_item <vec3>* vertex = temp_vertices.getFirst()->prev;

	p2List <vec3> outVertices;

	for (unsigned int i = 0; i < vertexIndices.count(); i++) {
		
		outVertices.add(vertex->data);

		vertexIndex = vertexIndex->next;
		vertex = vertex->next;
	}

	return true;
}

bool OBJLoader::Update() {

	vec3 vertices;
	vec2 uvs;
	vec3 normals;

	LoadOBJ("Assets/models/prueba.obj", vertices, uvs, normals);

	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	return true;
}