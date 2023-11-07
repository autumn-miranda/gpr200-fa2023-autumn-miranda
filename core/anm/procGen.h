#pragma once
#include "../ew/mesh.h"

namespace anm {
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subdivisions);


	ew::Mesh reshapePlane(float width, float height, int subdivisions) {
		ew::MeshData planeMeshData = anm::createPlane(width, height, subdivisions);
		return ew::Mesh(planeMeshData);
	};

	ew::Mesh reshapeCyl(float height, float radius, int segments) {
		ew::MeshData cylMeshData = anm::createCylinder(height, radius, segments);
		return ew::Mesh(cylMeshData);
	};

	ew::Mesh reshapeSphere(float radius, int segments) {
		ew::MeshData sphereMeshData = anm::createSphere(radius, segments);
		return ew::Mesh(sphereMeshData);
	};

}