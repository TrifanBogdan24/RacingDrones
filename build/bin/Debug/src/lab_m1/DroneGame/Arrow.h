#pragma once

#include "components/simple_scene.h"
#include "Transform3D.h"

#include <string>

using namespace std;


class Arrow
{
public:
	class Cylinder {
	public:
		Mesh* mesh;
		glm::mat4 modelMatrix;
		float radius;
		float length;
	};

	class Cone {
	public:
		Mesh* mesh;
		glm::mat4 modelMatrix;
		float radius;
		float length;
	};

public:
	Arrow::Cylinder cylinder;
	Arrow::Cone cone;

public:
	Arrow() = default;
	Arrow(float radiusValue, float cylinderLength, float coneLength)
	{
		this->cylinder.radius = radiusValue;
		this->cylinder.length = cylinderLength;

		this->cone.radius = radiusValue;
		this->cone.length = coneLength;

	}
};
