#ifndef GENESIS_MESH_PRIMITIVES_H
#define GENESIS_MESH_PRIMITIVES_H
#include <core/Core.h>
#include <math/Vec3.h>
#include <math/Vec2.h>

GENESIS_NAMESPACE_BEGIN

namespace meshPrimitives
{
	struct Vertex
	{
		Vec3 position;
		Vec2 uv;
	};

    struct MeshPrimitive
    {
        Vector<Vertex> vertices;
        Vector<uint32> indices;
    };

	MeshPrimitive createSphere(uint32 slices = 32, uint32 stacks = 32, float radius = 1.0f)
	{
        MeshPrimitive sphere;

        const float PI = 3.14159265359f;

        for (uint32 i = 0; i <= stacks; i++) {
            float v = static_cast<float>(i) / stacks;          // [0, 1]
            float phi = v * PI;                                // [0, PI]

            for (uint32 j = 0; j <= slices; j++) {
                float u = static_cast<float>(j) / slices;      // [0, 1]
                float theta = u * (2.0f * PI);                 // [0, 2PI]

                Vertex vertex;
                vertex.position.x = radius * std::sin(phi) * std::cos(theta);
                vertex.position.y = radius * std::cos(phi);
                vertex.position.z = radius * std::sin(phi) * std::sin(theta);
                vertex.uv.x = theta / (2.0f * PI);
                vertex.uv.y = v;

                sphere.vertices.push_back(vertex);
            }
        }

        for (uint32 i = 0; i < stacks; i++) {
            for (uint32 j = 0; j < slices; j++) {
                uint32 first = i * (slices + 1) + j;
                uint32 second = first + slices + 1;

                sphere.indices.push_back(first);
                sphere.indices.push_back(second);
                sphere.indices.push_back(first + 1);

                sphere.indices.push_back(second);
                sphere.indices.push_back(second + 1);
                sphere.indices.push_back(first + 1);
            }
        }

        return sphere;
	}

    MeshPrimitive createCube(float size = 1.0f)
    {
        MeshPrimitive cube;
        float halfSize = size * 0.5f;

        // Front face (+Z)
        cube.vertices.push_back({{-halfSize, -halfSize,  halfSize}, {0.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize, -halfSize,  halfSize}, {1.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize,  halfSize,  halfSize}, {1.0f, 1.0f}});
        cube.vertices.push_back({{-halfSize,  halfSize,  halfSize}, {0.0f, 1.0f}});

        // Back face (-Z)
        cube.vertices.push_back({{-halfSize, -halfSize, -halfSize}, {0.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize,  halfSize, -halfSize}, {1.0f, 1.0f}});
        cube.vertices.push_back({{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f}});

        // Right face (+X)
        cube.vertices.push_back({{ halfSize, -halfSize, -halfSize}, {0.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize, -halfSize,  halfSize}, {1.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize,  halfSize,  halfSize}, {1.0f, 1.0f}});
        cube.vertices.push_back({{ halfSize,  halfSize, -halfSize}, {0.0f, 1.0f}});

        // Left face (-X)
        cube.vertices.push_back({{-halfSize, -halfSize, -halfSize}, {0.0f, 0.0f}});
        cube.vertices.push_back({{-halfSize, -halfSize,  halfSize}, {1.0f, 0.0f}});
        cube.vertices.push_back({{-halfSize,  halfSize,  halfSize}, {1.0f, 1.0f}});
        cube.vertices.push_back({{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f}});

        // Top face (+Y)
        cube.vertices.push_back({{-halfSize, halfSize, -halfSize}, {0.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize, halfSize, -halfSize}, {1.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize, halfSize,  halfSize}, {1.0f, 1.0f}});
        cube.vertices.push_back({{-halfSize, halfSize,  halfSize}, {0.0f, 1.0f}});

        // Bottom face (-Y)
        cube.vertices.push_back({{-halfSize, -halfSize, -halfSize}, {0.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f}});
        cube.vertices.push_back({{ halfSize, -halfSize,  halfSize}, {1.0f, 1.0f}});
        cube.vertices.push_back({{-halfSize, -halfSize,  halfSize}, {0.0f, 1.0f}});

        for (uint32 face = 0; face < 6; face++) {
            uint32 base = face * 4;

            cube.indices.push_back(base + 0);
            cube.indices.push_back(base + 1);
            cube.indices.push_back(base + 2);

            cube.indices.push_back(base + 2);
            cube.indices.push_back(base + 3);
            cube.indices.push_back(base + 0);
        }

        return cube;
    }
}

GENESIS_NAMESPACE_END

#endif