#pragma once

#include "lab_m1/DroneGame/Transform3D.h"

#include <vector>
#include <iostream>

using namespace std;

class Tree
{
public:
	static class Trunk {
	public:
		float height;
		float radius;
		Mesh* mesh;
		glm::mat4 modelMatrix;

	public:
		Trunk() = default;
		Trunk(float heightValue, float radiusValue, Mesh* meshValue)
		{
			this->height = heightValue;
			this->radius = radiusValue;
			this->mesh = meshValue;
		}

	};


	static class Cone {
	public:
		float height;
		float radius;
		Mesh* mesh;
		glm::mat4 modelMatrix;

	public:
		Cone() = default;
		Cone(float heightValue, float radiusValue, Mesh* meshValue)
		{
			this->height = heightValue;
			this->radius = radiusValue;
			this->mesh = meshValue;
		}
	};

public:
	glm::vec3 position;
	Trunk trunk;
	vector<Cone> cones;
	float coneHeightOffsetPercentage;


public:
	Tree() = default;

	Tree(glm::vec3 positionValue, Trunk& trunkValue, vector<Cone>& conesValue)
	{
		this->coneHeightOffsetPercentage = 0.2f;

		this->position = positionValue;
		this->trunk = trunkValue;
		this->cones = conesValue;

		this->trunk.modelMatrix =
			Transform3D::Translate(position.x, position.y, position.z)
			* Transform3D::Scale(trunk.radius, trunk.height / 2.f, trunk.radius)
			* Transform3D::RotateOY(glm::radians(0.0f));

		float heightOffset = position.y + trunk.height;

		for (Cone& cone : this->cones) {
			cone.modelMatrix =
				Transform3D::Translate(position.x, heightOffset, position.z)
				* Transform3D::Scale(cone.radius, cone.height / 2.f, cone.radius);

			heightOffset += coneHeightOffsetPercentage * cone.height;
		}
	}
};