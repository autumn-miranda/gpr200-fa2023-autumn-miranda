#include "procGen.h"
#include "../ew/mesh.h"

ew::MeshData anm::createPlane(float width, float height, int subdivisions)
{
	ew::MeshData plane;

	//Plane Vertices
	for (int row = 0; row <= subdivisions; row++) {
		for (int col = 0; col <= subdivisions; col++) {
			ew::Vec3 pos;
			ew::Vertex vertex;
			pos.x = width * (col / static_cast<float>(subdivisions));
			pos.y = 0.0f;
			pos.z = -height * (row / static_cast<float>(subdivisions));
			vertex.pos = pos;
			vertex.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
			vertex.uv = ew::Vec2(row, col);
			plane.vertices.push_back(vertex);
		}
	}//end of vertices

	//Plane indices
	int columns = subdivisions + 1;
	for (int row = 0; row < subdivisions; row++) {
		for (int col = 0; col < subdivisions; col++) {
			int start = row * columns + col;
			//bottom triangle
			plane.indices.push_back(start);
			plane.indices.push_back(start+1);
			plane.indices.push_back(start+columns+1);

			//top triangle
			plane.indices.push_back(start);
			plane.indices.push_back(start+columns+1);
			plane.indices.push_back(start+columns);

		}
	}//end of indices


	return plane;
};
