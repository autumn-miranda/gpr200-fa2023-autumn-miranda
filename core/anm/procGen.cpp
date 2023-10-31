#include "procGen.h"
#include "../ew/mesh.h"

ew::MeshData anm::createPlane(float width, float height, int subdivisions)
{
	ew::MeshData plane;

	//Plane Vertices
	for (int row = 0; row <= subdivisions; row++) {
		for (int col = 0; col <= subdivisions; col++) {
			//normal and pos y both equal (1,0)
			ew::Vec3 pos;
			ew::Vertex vertex;
			pos.x = width * (col / subdivisions);
			pos.z = -height * (row / subdivisions);
			vertex.pos = pos;
			plane.vertices.push_back(vertex);
		}

	}

	return plane;
};
