#pragma once
#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../anm/transformations.h"
#include <GLFW/glfw3.h>


namespace anm{
	struct Camera {
		ew::Vec3 position; //camera body position
		ew::Vec3 target; //position to look at
		float fov; //Vertical fov in degrees
		float aspectRatio; //screen width / screen height
		float nearPlane; //near plane distance +z
		float farPlane; //far plane distance +z
		bool orthographic; // toggle between ortho and perspective
		float orthoSize; //height of orthographic frustum
		ew::Mat4 ViewMatrix() {
			return anm::LookAt(position, target, ew::Vec3(0.0f, 1.0f, 0.0f));
		};//World->view
		ew::Mat4 ProjectionMatrix() {
			if (orthographic) { return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane); }
			return Perspective(fov, aspectRatio, nearPlane, farPlane);
		};//view->clip

		void setValues(int sWidth, int sHeight)
		{
			position = ew::Vec3(0.0f, 0.0f, 5.0f); //We will be looking down the -Z axis!
			target = ew::Vec3(0.0f, 0.0f, 0.0f);
			fov = 60.0f;
			orthoSize = 6.0f;
			nearPlane = 0.1f;
			farPlane = 100.0f;
			orthographic = true;
			aspectRatio = sWidth / static_cast<float>(sHeight);
		}
	};//end of camera

	struct CameraControls {
		double prevMouseX, prevMouseY;//mouse position from previous frame
		float yaw = 0, pitch = 0; // degrees
		float mouseSensitivity = 0.1f; //How fast to turn with mouse
		bool firstMouse = true;//flag to store initial mouse position
		float moveSpeed = 5.0f; // how fast to move with arrow keys

		void moveCamera(GLFWwindow* window, Camera* camera, CameraControls* controls, float deltaTime) {
			//if right mouse is not held, release cursor and return early
			if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
				//release cursor
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				controls->firstMouse = true;
				return;
			}

			//GLFW_CURSOR_DISABLED hides the cursor but still lets the position be changed
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			//get screen mouse position this frame
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			//if we just started righ clicking, set prevMouse values to current position
			//this prevents a bug where the camera moves as soon as we click
			if (controls->firstMouse) {
				controls->firstMouse = false;
				controls->prevMouseX = mouseX;
				controls->prevMouseY = mouseY;
			}

				//find pitch and yaw, and clamp pitch
				yaw += (mouseX - prevMouseX) * mouseSensitivity;
				pitch -= (mouseY - prevMouseY) * mouseSensitivity;
				if (pitch < -89.0f) { pitch = -89.0f; }
				if (pitch > 89.0f) { pitch = 89.0f; }

				//remeber previous mouse position
				controls->prevMouseX = mouseX;
				controls->prevMouseY = mouseY;

				//construct forward vector using yaw and pitch and convert to radians
				ew::Vec3 forward = ew::Normalize(ew::Vec3(
					(sin(yaw*ew::DEG2RAD) * cos(pitch*ew::DEG2RAD)), sin(pitch*ew::DEG2RAD), ((-1*cos(yaw * ew::DEG2RAD)) * cos(pitch*ew::DEG2RAD))));//.z

				ew::Vec3 right = ew::Normalize(ew::Cross(ew::Vec3(0.0f, 1.0f, 0.0f),forward));
				//ew::Vec3 right = ew::Normalize(ew::Cross(forward,ew::Vec3(0.0f, 1.0f, 0.0f)));
				ew::Vec3 up = ew::Normalize(ew::Cross(right, forward));

				
				//keyboard move controls
				if (glfwGetKey(window, GLFW_KEY_W)) {
					camera->position += forward * controls->moveSpeed * deltaTime;
				}
				if (glfwGetKey(window, GLFW_KEY_A)) {
					camera->position += right * controls->moveSpeed * deltaTime;
				}
				if (glfwGetKey(window, GLFW_KEY_D)) {
					camera->position -= right * controls->moveSpeed * deltaTime;
				}
				if (glfwGetKey(window, GLFW_KEY_S)) {
					camera->position -= forward * controls->moveSpeed * deltaTime;
				}
				if (glfwGetKey(window, GLFW_KEY_Q)) {
					camera->position -= up * controls->moveSpeed * deltaTime;
				}
				if (glfwGetKey(window, GLFW_KEY_E)) {
					camera->position += up * controls->moveSpeed * deltaTime;
				}



				//by setting target to a point in front of the camera along its forward direction
				//our lookAt will be updated accordinglt when rendering
				camera->target = camera->position + forward;

		}//end of move camera
	};//end of camera controls
}
