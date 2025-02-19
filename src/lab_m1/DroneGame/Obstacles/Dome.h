#pragma once

#include "lab_m1/DroneGame/Transform3D.h"


using namespace std;


// Like the Santorini Dome
// https://www.google.com/search?sca_esv=d12e58ed6977e94a&sxsrf=ADLYWIKHnF5lrKG6C3Z2TZiHEt43n0hR9g:1736086283725&q=greece+blue+domes&udm=2&fbs=AEQNm0AuaLfhdrtx2b9ODfK0pnmis1zS4enB7jefi_fubH5nzy2TnzGs8VHD6VG98pWooD85MABcToI9uDS1eydCwpifH6gFUSOGyrfY_1nJwZlp9ue4OWlmGvVZItjxD_Z0XIXvCQS2m8d9zoWiiEDF3Y2D96G_6oq5JWbdPfrHOwKY1Aa7-ugRm_fGKXhxHHpnQjbYR24z7td9QJRU1m1A5QDW2997mlrkBNb0580aylIu7eQi1Wc&sa=X&ved=2ahUKEwiqn_ax4d6KAxWiExAIHWErJb0QtKgLegQIHhAB&biw=1920&bih=912#vhid=S7lJ2nN3Sbg5rM&vssid=mosaic


class Dome
{
public:
	glm::vec3 position;
	float length;
	float angleOY;
	Mesh* cubeMesh;
	Mesh* semiSphereMesh;
	glm::mat4 cubeModelMatrix;
	glm::mat4 semiSphereModelMatrix;
	glm::vec3 cubeColor;
	glm::vec3 semiSphereColor;


public:
	Dome() = default;
	Dome(
		glm::vec3 positionValue, float lengthValue, float angleOYValue,
		Mesh* cubeMeshValue, Mesh* semiSphereMeshValue,
		glm::vec3 cubeColorValue, glm::vec3 semiSphereColorValue)
	{
		this->position = positionValue;
		this->length = lengthValue;
		this->angleOY = angleOYValue;
		this->cubeMesh = cubeMeshValue;
		this->semiSphereMesh = semiSphereMeshValue;
		this->cubeColor = cubeColorValue;
		this->semiSphereColor = semiSphereColorValue;

		cubeModelMatrix =
			glm::mat4(1.0f)
			* Transform3D::Translate(position.x, position.y + length / 2.f, position.z)
			* Transform3D::Scale(length, length, length)
			* Transform3D::RotateOY(glm::radians(angleOY));

		semiSphereModelMatrix =
			glm::mat4(1.0f)
			* Transform3D::Translate(position.x, position.y + length, position.z)
			* Transform3D::Scale(length, length, length);
	}
};
