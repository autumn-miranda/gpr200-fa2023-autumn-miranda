#include "procGen.h"
#include "../ew/mesh.h"
#include "../ew/ewMath/ewMath.h"

ew::MeshData anm::createPlane(float width, float height, int subdivisions)
{
	ew::MeshData plane;
	if (subdivisions <= 0) { subdivisions = 1; }

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



ew::MeshData createCylinder(float height, float radius, int numSegments) {
	ew::MeshData cyl;
		//cylinder vertices
	//top center
	float topY = (height / 2);
	ew::Vec3 pos;
	ew::Vertex vertex;
	pos = ew::Vec3(0.0f, topY, 0.0f);
	vertex.pos = pos;
	vertex.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
	vertex.uv = ew::Vec2(0.0f, 0.0f);
	cyl.vertices.push_back(vertex);
	//top ring
	float thetaStep = (2*ew::PI) / numSegments;
	for (int i = 0; i <= numSegments; i++) {
		float theta = i * thetaStep;
		pos.x = cos(theta) * radius;
		pos.y = topY;
		pos.z = sin(theta) * radius;
		vertex.pos = pos;
		vertex.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
		vertex.uv = ew::Vec2(1.0f,1.0f);//temp UV value of 1,1
		cyl.vertices.push_back(vertex);
	}//end of top ring
	//side top ring
	for (int i = 0; i <= numSegments; i++) {
		float theta = i * thetaStep;
		pos.x = cos(theta) * radius;
		pos.y = topY;
		pos.z = sin(theta) * radius;
		vertex.pos = pos;
		vertex.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
		vertex.uv = ew::Vec2(1.0f, 1.0f);//temp UV value of 1,1
		cyl.vertices.push_back(vertex);
	}//end of top ring


		//cylinder indices

	return cyl;
};

/*
Cyliner layout:
	Add top Center
	Add top ring vertices (top face)
	add top ring vertices (side face)
	add bottom ring vertices (side face)
	add bottom ring vertices(bottom face)
	add bottom center
*/