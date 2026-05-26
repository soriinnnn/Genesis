#include <resources/Mesh.h>
#include <graphics/GraphicsDevice.h>
#include <core/utils/Macros.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace genesis;
using namespace std;
using namespace Assimp;

static uint32 getVertexStride(uint32 components);
static vector<uint8> getVertices(const aiMesh& mesh, uint32 components);
static vector<uint32> getIndices(const aiMesh& mesh);

Mesh::Mesh(const MeshDesc& desc): Resource(desc.resource)
{
    auto& graphicsContext = desc.resource.graphicsContext;

    Importer importer;
    const aiScene* scene = importer.ReadFile(
        m_path,
        aiProcess_Triangulate | 
        aiProcess_MakeLeftHanded | 
        aiProcess_FlipUVs | 
        aiProcess_FlipWindingOrder | 
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality
    );

    if (!scene) {
        GENESIS_LOG_THROW_ERROR("Failed to load mesh file \"{}\".\nDetails:\n{}", m_path.c_str(), importer.GetErrorString());
    }
    if (scene->mNumMeshes > 1) {
        GENESIS_LOG_WARNING("Mesh \"{}\" contains multiple sub-meshes. Only the first one will be loaded.", m_path.c_str());
    }
    aiMesh* mesh = scene->mMeshes[0];

    uint32 stride = getVertexStride(desc.components);
    if (stride == 0) {
        GENESIS_LOG_THROW_ERROR("Failed to create mesh \"{}\": Vertex components mask is empty.", m_path.c_str());
    }

    vector<uint8> vertices = getVertices(*mesh, desc.components);
    m_vertexBuffer = graphicsContext.graphicsDevice.createVertexBuffer({vertices.data(), static_cast<uint32>(vertices.size()) / stride, stride});

    vector<uint32> indices = getIndices(*mesh);
    m_indexBuffer = graphicsContext.graphicsDevice.createIndexBuffer({indices.data(), static_cast<uint32>(indices.size())});
}

Mesh::~Mesh() {}

const VertexBuffer& Mesh::getVertexBuffer() const noexcept
{
    return *m_vertexBuffer;
}

const IndexBuffer& Mesh::getIndexBuffer() const noexcept
{
    return *m_indexBuffer;
}

/* STATIC FUNCTION DEFINITIONS */

vector<uint8> getVertices(const aiMesh& mesh, uint32 components) 
{
    vector<uint8> vertices(mesh.mNumVertices * getVertexStride(components), 0);

    uint8* data = vertices.data();
    for (uint32 i = 0; i < mesh.mNumVertices; i++) {
        if (components & GENESIS_VERTEX_COMPONENT_POSITION) {
            Vec3 pos{0.0f, 0.0f, 0.0f};

            if (mesh.HasPositions()) {
                pos.x = mesh.mVertices[i].x;
                pos.y = mesh.mVertices[i].y;
                pos.z = mesh.mVertices[i].z;
            }

            memcpy(data, &pos, sizeof(Vec3));
            data += sizeof(Vec3);
        }
        if (components & GENESIS_VERTEX_COMPONENT_NORMAL) {
            Vec3 normal{0.0f, 0.0f, 0.0f};

            if (mesh.HasNormals()) {
                normal.x = mesh.mNormals[i].x;
                normal.y = mesh.mNormals[i].y;
                normal.z = mesh.mNormals[i].z;
            }

            memcpy(data, &normal, sizeof(Vec3));
            data += sizeof(Vec3);
        }
        if (components & GENESIS_VERTEX_COMPONENT_TANGENT) {
            Vec3 tangent{0.0f, 0.0f, 0.0f};

            if (mesh.HasTangentsAndBitangents()) {
                tangent.x = mesh.mTangents[i].x;
                tangent.y = mesh.mTangents[i].y;
                tangent.z = mesh.mTangents[i].z;
            }

            memcpy(data, &tangent, sizeof(Vec3));
            data += sizeof(Vec3);
        }
        if (components & GENESIS_VERTEX_COMPONENT_BITANGENT) {
            Vec3 bitangent{0.0f, 0.0f, 0.0f};

            if (mesh.HasTangentsAndBitangents()) {
                bitangent.x = mesh.mBitangents[i].x;
                bitangent.y = mesh.mBitangents[i].y;
                bitangent.z = mesh.mBitangents[i].z;
            }

            memcpy(data, &bitangent, sizeof(Vec3));
            data += sizeof(Vec3);
        }
        if (components & GENESIS_VERTEX_COMPONENT_TEXCOORD) {
            Vec2 texture{0.0f, 0.0f};

            if (mesh.HasTextureCoords(0)) {
                texture.x = mesh.mTextureCoords[0][i].x;
                texture.y = mesh.mTextureCoords[0][i].y;
            }
            
            memcpy(data, &texture, sizeof(Vec2));
            data += sizeof(Vec2);
        }
        if (components & GENESIS_VERTEX_COMPONENT_COLOR) {
            Vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

            if (mesh.HasVertexColors(0)) {
                color.x = mesh.mColors[0][i].r;
                color.y = mesh.mColors[0][i].g;
                color.z = mesh.mColors[0][i].b;
                color.w = mesh.mColors[0][i].a;
            }

            memcpy(data, &color, sizeof(Vec4));
            data += sizeof(Vec4);
        }
    }

    return vertices;
}

vector<uint32> getIndices(const aiMesh& mesh)
{
    vector<uint32> indices;

    indices.reserve(mesh.mNumFaces * 3);
    for (uint32 i = 0; i < mesh.mNumFaces; i++) {
        aiFace face = mesh.mFaces[i];

        for (uint32 j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return indices;
}

uint32 getVertexStride(uint32 components)
{
    uint32 stride = 0;

    if (components & GENESIS_VERTEX_COMPONENT_POSITION)  stride += GENESIS_VERTEX_COMPONENT_POSITION_SIZE;
    if (components & GENESIS_VERTEX_COMPONENT_NORMAL)    stride += GENESIS_VERTEX_COMPONENT_NORMAL_SIZE;
    if (components & GENESIS_VERTEX_COMPONENT_TANGENT)   stride += GENESIS_VERTEX_COMPONENT_TANGENT_SIZE;
    if (components & GENESIS_VERTEX_COMPONENT_BITANGENT) stride += GENESIS_VERTEX_COMPONENT_BITANGENT_SIZE;
    if (components & GENESIS_VERTEX_COMPONENT_TEXCOORD)  stride += GENESIS_VERTEX_COMPONENT_TEXCOORD_SIZE;
    if (components & GENESIS_VERTEX_COMPONENT_COLOR)     stride += GENESIS_VERTEX_COMPONENT_COLOR_SIZE;

    return stride;
}