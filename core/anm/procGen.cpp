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



ew::MeshData anm::createCylinder(float height, float radius, int numSegments) {
	if (numSegments <= 2) { numSegments = 3; }
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
		vertex.normal = ew::Normalize(ew::Vec3(pos.x, 0.0f, pos.z));
		vertex.uv = ew::Vec2(1.0f, 1.0f);//temp UV value of 1,1
		cyl.vertices.push_back(vertex);
	}//end of top ring

	//Bottom side ring
	for (int i = 0; i <= numSegments; i++) {
		float theta = i * thetaStep;
		pos.x = cos(theta) * radius;
		pos.y = -1 * topY;
		pos.z = sin(theta) * radius;
		vertex.pos = pos;
		vertex.normal = ew::Normalize(ew::Vec3(pos.x, 0.0f, pos.z));
		vertex.uv = ew::Vec2(1.0f, 1.0f);//temp UV value of 1,1
		cyl.vertices.push_back(vertex);
	}//end of bottom side ring
	//bottom ring
	for (int i = 0; i <= numSegments; i++) {
		float theta = i * thetaStep;
		pos.x = cos(theta) * radius;
		pos.y = -1 * topY;
		pos.z = sin(theta) * radius;
		vertex.pos = pos;
		vertex.normal = ew::Vec3(0.0f, -1.0f, 0.0f);
		vertex.uv = ew::Vec2(1.0f, 1.0f);//temp UV value of 1,1
		cyl.vertices.push_back(vertex);
	}//end of bottom ring

	//bottom center
	pos = ew::Vec3(0.0f, (-1 * topY), 0.0f);
	vertex.pos = pos;
	vertex.normal = ew::Vec3(0.0f, -1.0f, 0.0f);
	vertex.uv = ew::Vec2(0.0f, 0.0f);
	cyl.vertices.push_back(vertex);


		//cylinder indices
	int center = 0;
	int start = 1;
	for (int i = 0; i <= numSegments; i++) {
		cyl.indices.push_back(start+i);
		cyl.indices.push_back(center);
		cyl.indices.push_back(start + i + 1);
	}//top ring indices

	int sideStart = start + numSegments + 1;
	int col = numSegments + 1;
	for (int i = 0; i < col; i++) {
		start = sideStart + i;
		//top triangle
		cyl.indices.push_back(start);
		cyl.indices.push_back(start + 1);
		cyl.indices.push_back(start + col);
		//bottom triangle
		cyl.indices.push_back(start + 1);
		cyl.indices.push_back(start + col + 1);
		cyl.indices.push_back(start + col);
	}//side indices

	/*center = ; //center is the index of the last vertex
	for (int i = 0; i <= numSegments; i++) {
		cyl.indices.push_back(start + i);
		cyl.indices.push_back(center);
		cyl.indices.push_back(start + i + 1);
	}//bottom ring indices*/

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