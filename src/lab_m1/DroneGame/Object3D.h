#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"



namespace Object3D
{
	Mesh* CreateCylinder(const std::string& name)
	{
        const int numSegments = 36;
        const float radius = 0.5f;
        const float height = 2.0f;
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        // Generate the top and bottom circle vertices
        for (int i = 0; i < numSegments; ++i) {
            float angle = i * 2.0f * glm::pi<float>() / numSegments;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            vertices.push_back(VertexFormat(glm::vec3(x, 0.0f, z), glm::vec3(1, 1, 1)));
            vertices.push_back(VertexFormat(glm::vec3(x, height, z), glm::vec3(1, 1, 1)));
        }

        // Generate indices for the side faces
        for (int i = 0; i < numSegments; ++i) {
            int next = (i + 1) % numSegments;

            // Two triangles for each quad
            indices.push_back(2 * i);         // Bottom vertex
            indices.push_back(2 * i + 1);     // Top vertex
            indices.push_back(2 * next);      // Next bottom vertex

            indices.push_back(2 * next);      // Next bottom vertex
            indices.push_back(2 * i + 1);     // Top vertex
            indices.push_back(2 * next + 1);  // Next top vertex
        }

        // Generate indices for the top and bottom circles
        for (int i = 1; i < numSegments - 1; ++i) {
            // Bottom circle
            indices.push_back(0);
            indices.push_back(2 * i);
            indices.push_back(2 * (i + 1));

            // Top circle
            indices.push_back(1);
            indices.push_back(2 * i + 1);
            indices.push_back(2 * (i + 1) + 1);
        }

        Mesh* cylinder = new Mesh(name);
        cylinder->InitFromData(vertices, indices);
        return cylinder;
	}


    Mesh* CreateCone(const std::string& name)
    {
        const int numSegments = 36;
        const float radius = 0.5f;
        const float height = 2.0f;
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        // Add the apex of the cone (top point)
        vertices.push_back(VertexFormat(glm::vec3(0.0f, height, 0.0f), glm::vec3(1, 1, 1)));  // Apex vertex

        // Generate the bottom circle vertices (base of the cone)
        for (int i = 0; i < numSegments; ++i) {
            float angle = i * 2.0f * glm::pi<float>() / numSegments;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            vertices.push_back(VertexFormat(glm::vec3(x, 0.0f, z), glm::vec3(1, 1, 1)));  // Base vertices
        }

        // Generate indices for the side faces (cone's surface)
        for (int i = 1; i < numSegments; ++i) {
            int next = i + 1;
            if (i == numSegments - 1) {
                next = 1;  // Wrap around for the last segment
            }

            // Side triangle (for each segment)
            indices.push_back(0);          // Apex vertex
            indices.push_back(i);          // Current base vertex
            indices.push_back(next);       // Next base vertex
        }

        // Create a mesh for the cone
        Mesh* cone = new Mesh(name);
        cone->InitFromData(vertices, indices);
        return cone;
    }

    Mesh* CreateBox(const std::string& name)
    {
        std::vector<VertexFormat> vertices = {
            // Front face
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1, 1, 1)),

            // Back face
            VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1, 1, 1)),

            // Left face
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1, 1, 1)),

            // Right face
            VertexFormat(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1, 1, 1)),

            // Top face
            VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1, 1, 1)),

            // Bottom face
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 1))
        };

        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0,     // Front face
            4, 5, 6, 6, 7, 4,     // Back face
            8, 9, 10, 10, 11, 8,   // Left face
            12, 13, 14, 14, 15, 12, // Right face
            16, 17, 18, 18, 19, 16, // Top face
            20, 21, 22, 22, 23, 20  // Bottom face
        };

        Mesh* box = new Mesh(name);
        box->InitFromData(vertices, indices);
        return box;
    }

    Mesh* CreateSphere(const std::string& name)
    {
        const int numSlices = 36;
        const int numStacks = 18;
        const float radius = 0.5f;
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        // Generate vertices
        for (int i = 0; i <= numStacks; ++i) {
            float lat = glm::pi<float>() * float(i) / float(numStacks);
            float sinLat = sin(lat);
            float cosLat = cos(lat);

            for (int j = 0; j <= numSlices; ++j) {
                float lon = 2.0f * glm::pi<float>() * float(j) / float(numSlices);
                float sinLon = sin(lon);
                float cosLon = cos(lon);

                float x = radius * sinLat * cosLon;
                float y = radius * cosLat;
                float z = radius * sinLat * sinLon;

                vertices.push_back(VertexFormat(glm::vec3(x, y, z), glm::vec3(1, 1, 1)));
            }
        }

        // Generate indices for sphere's triangles
        for (int i = 0; i < numStacks; ++i) {
            for (int j = 0; j < numSlices; ++j) {
                int p0 = i * (numSlices + 1) + j;
                int p1 = p0 + 1;
                int p2 = p0 + (numSlices + 1);
                int p3 = p2 + 1;

                if (i != 0) {
                    indices.push_back(p0);
                    indices.push_back(p2);
                    indices.push_back(p1);
                }

                if (i != (numStacks - 1)) {
                    indices.push_back(p1);
                    indices.push_back(p2);
                    indices.push_back(p3);
                }
            }
        }

        Mesh* sphere = new Mesh(name);
        sphere->InitFromData(vertices, indices);
        return sphere;
    }

    Mesh* CreateTerrain(const std::string& name)
    {
        const int numRows = 300;
        const int numCols = 300;
        const float cellSize = 1.0f;

        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        // Generate the vertices for the grid
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                float x = j * cellSize;
                float z = i * cellSize;
                float y = 0.0f;

                glm::vec3 normal(0.0f, 1.0f, 0.0f);

                vertices.push_back(VertexFormat(glm::vec3(x, y, z), normal));
            }
        }

        // Generate indices for the grid (two triangles per cell)
        for (int i = 0; i < numRows - 1; ++i) {
            for (int j = 0; j < numCols - 1; ++j) {
                int topLeft = i * numCols + j;
                int topRight = topLeft + 1;
                int bottomLeft = (i + 1) * numCols + j;
                int bottomRight = bottomLeft + 1;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        Mesh* terrain = new Mesh(name);
        terrain->InitFromData(vertices, indices);

        return terrain;
    }


}