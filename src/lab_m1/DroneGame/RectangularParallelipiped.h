#pragma once

#include "components/simple_scene.h"

#include <string>

using namespace std;


class RectangularParallelipiped
{
public:
	Mesh* mesh;
	glm::mat4 modelMatrix;
	float length;
	float width;
	float height;

public:
	RectangularParallelipiped() = default;
	RectangularParallelipiped(float lengthValue, float widthValue, float heightValue)
	{
		this->modelMatrix = glm::mat4(1);
		this->length = lengthValue;
		this->width = widthValue;
		this->height = heightValue;
	}
};
