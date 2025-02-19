#pragma once

#include "lab_m1/DroneGame/Transform3D.h"


using namespace std;




class CheckPoint
{
	public:
		glm::vec3 position;
		
		float thickness;
		float pillarHeight;
		float gateLength;
		float gateHeight;
		float totalHeight;
		
		Mesh* leftPillarMesh;
		Mesh* rightPillarMesh;
		Mesh* lowerLineMesh;
		Mesh* upperLineMesh;
		
		glm::mat4 leftPillarModelMatrix;
		glm::mat4 rightPillarModelMatrix;
		glm::mat4 lowerLineModelMatrix;
		glm::mat4 upperLineModelMatrix;
		
		glm::vec3 leftPillarColor;
		glm::vec3 rightPillarColor;
		glm::vec3 lowerLineColor;
		glm::vec3 upperLineColor;



	public:
		CheckPoint() = default;
		CheckPoint(
			glm::vec3 positionValue,
			float pillaHeightValue, float gateLengthValue, float gateHeightValue,
			Mesh* leftPillarMeshValue, Mesh* rightPillarMeshValue,
			Mesh* lowerLineMeshValue, Mesh* upperLineMeshValue)
		{
			this->thickness = 0.1f;
			this->pillarHeight = pillaHeightValue;
			this->gateLength = gateLengthValue;
			this->gateHeight = gateHeightValue;
			this->totalHeight = pillarHeight + gateHeight;

			this->position = positionValue;
			this->leftPillarMesh = leftPillarMeshValue;
			this->rightPillarMesh = rightPillarMeshValue;
			this->lowerLineMesh = lowerLineMeshValue;
			this->upperLineMesh = upperLineMeshValue;

			this->leftPillarModelMatrix =
				glm::mat4(1.0f)
				* Transform3D::Translate(position.x - gateLength / 2.f, position.y + totalHeight / 2.f, position.z)
				* Transform3D::Scale(thickness, totalHeight, thickness);

			this->rightPillarModelMatrix =
				glm::mat4(1.0f)
				* Transform3D::Translate(position.x + gateLength / 2.f, position.y + totalHeight / 2.f, position.z)
				* Transform3D::Scale(thickness, totalHeight, thickness);

			this->lowerLineModelMatrix =
				glm::mat4(1.0f)
				* Transform3D::Translate(position.x, position.y + pillarHeight, position.z)
				* Transform3D::Scale(gateLength, thickness, thickness);

			this->upperLineModelMatrix =
				glm::mat4(1.0f)
				* Transform3D::Translate(position.x, position.y + totalHeight - thickness / 2.f, position.z)
				* Transform3D::Scale(gateLength, thickness, thickness);
		}
};