//transformations.
#pragma once
#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"

namespace anm {
	//identity matrix
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};

	//scale on x,y,z axes
	inline ew::Mat4 Scale(ew::Vec3 s) {
		return ew::Mat4(
			s.x,   0,   0,  0,
			  0, s.y,   0,  0,
			  0,   0, s.z,  0,
			  0,   0,   0,  1
		);
	};

	//rotation around the X axis in radians (PITCH)
	inline ew::Mat4 RotateX(float rad) {
		return ew::Mat4(
			1,        0,         0, 0,
			0, cos(rad), -sin(rad), 0,
			0, sin(rad),  cos(rad), 0,
			0,        0,         0, 1

		);
	};

	//rotation around the Y axis in radians (YAW)
	inline ew::Mat4 RotateY(float rad) {
		return ew::Mat4(
			 cos(rad), 0, sin(rad), 0,
			        0, 1,        0, 0,
			-sin(rad), 0, cos(rad), 0,
			        0, 0,        0, 1
		);
	};

	//rotation around the z axis in radian (ROLL)
	inline ew::Mat4 RotateZ(float rad) {
		return ew::Mat4(
			cos(rad), -sin(rad), 0, 0,
			sin(rad),  cos(rad), 0, 0,
			       0,         0, 1, 0,
			       0,         0, 0, 1
		);
	};

	//translate x,y,z
	inline ew::Mat4 Translate(ew::Vec3 t) {
		return ew::Mat4(
			  1,   0,   0,  t.x,
		   	  0,   1,   0,  t.y,
			  0,   0,   1,  t.z,
			  0,   0,   0,    1
		);
	};

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f);//Euler angles in degrees
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		ew::Mat4 getModelMatrix() const {
			return (anm::Translate(position)*anm::RotateX(rotation.x * ew::DEG2RAD)*anm::RotateY(rotation.y * (3.1415/180))*anm::RotateZ(rotation.z * (3.1415/180))*anm::Scale(scale));
		}
	};

	//creates a righthanded view space
	//eye = eye (camera) position
	//target = position to look at
	//up = up axis, usually (0,1,0)
	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up) {
		ew::Vec3 f = Normalize(target-eye);
		ew::Vec3 r = Normalize(ew::Cross(up, f));
		ew::Vec3 u = Normalize(ew::Cross(f,r));

		//try to separate out the matrices and then multiply

		ew::Mat4 rotation = (
			r.x, r.y, r.z, 0,
			u.x, u.y, u.z, 0,
			f.x, f.y, f.z, 0,
			  0,   0,   0, 1
			);
		ew::Mat4 translation(
			1, 0, 0, -target.x,
			0, 1, 0, -target.y,
			0, 0, 1, -target.z,
			0, 0, 0, 1
		);

		return rotation * translation;
		/*return ew::Mat4(
			r.x, r.y, r.z, -1 * (ew::Dot(r,eye)),
			u.x, u.y, u.z, -1 * (ew::Dot(u, eye)),
			f.x, f.y, f.z, -1 * (ew::Dot(f, eye)),
			  0,   0,   0,  1
		);*/
	};

	//orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) {
		float right = (aspect * height) / 2.0f;
		float top = height / 2.0f;
		float left = -1 * right;
		float bottom = -1 * top;
		return ew::Mat4(
			(2/(right-left)),                  0,      0, (-1 * ((right) + left)/(right - left)),
			               0, (2/(top - bottom)),      0, (-1 * (( top) + bottom) / (top - bottom)),
			               0,     0,   ((-2/(far-near))), (-1 * ((far) + near) / (far - near)),
			               0,     0,                   0, 1
		);
	};

	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) {
		fov *= ew::DEG2RAD;
		return ew::Mat4(
			(1/(tan(fov/2.0f) * aspect)), 0, 0, 0,
			0, (1/tan(fov/2.0f)), 0, 0,
			0, 0, ((near + far)/(near - far)), ((2*far*near)/(near-far)),
			0, 0, -1, 0
		);
	};
}