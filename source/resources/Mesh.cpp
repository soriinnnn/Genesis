#include <resources/Mesh.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/utils/GraphicsTypes.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace genesis;
using namespace std;
using namespace Assimp;

static vector<Vertex> getVertices(const aiMesh& mesh);
static vector<uint32> getIndices(const aiMesh& mesh);

Mesh::Mesh(const MeshDesc& desc): Resource(desc.resource)
{
    Importer importer;
    const aiScene* scene = importer.ReadFile(
        m_path,
        aiProcess_Triangulate | aiProcess_MakeLeftHanded | aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_GenNormals
    );

    if (!scene) {
        GENESIS_LOG_THROW_ERROR("Failed to load mesh file \"{}\".\nDetails:\n{}", m_path.c_str(), importer.GetErrorString());
    }

    if (scene->mNumMeshes > 1) {
        GENESIS_LOG_WARNING("Mesh \"{}\" contains multiple sub-meshes. Only the first one will be loaded.", m_path.c_str());
    }

    aiMesh* mesh = scene->mMeshes[0];
    vector<Vertex> vertices = getVertices(*mesh);
    vector<uint32> indices = getIndices(*mesh);
    m_vertexBuffer = desc.resource.graphicsDevice.createVertexBuffer({vertices.data(), static_cast<uint32>(vertices.size()), sizeof(Vertex)});
    m_indexBuffer = desc.resource.graphicsDevice.createIndexBuffer({indices.data(), static_cast<uint32>(indices.size())});
}

Mesh::~Mesh() {}

VertexBuffer& Mesh::getVertexBuffer() noexcept
{
    return *m_vertexBuffer;
}

IndexBuffer& Mesh::getIndexBuffer() noexcept
{
    return *m_indexBuffer;
}

/* STATIC FUNCTION DEFINITIONS */

static vector<Vertex> getVertices(const aiMesh& mesh) 
{
    vector<Vertex> vertices;

    vertices.reserve(mesh.mNumVertices);
    for (uint32 i = 0; i < mesh.mNumVertices; i++) {
        Vertex v;

        v.position = {mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z};
        v.normal = {mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z};
        if (mesh.mTextureCoords[0]) {
            v.texture = {mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y};
        }
        else {
            v.texture = {0.0f, 0.0f};
        }
        vertices.push_back(v);
    }

    return vertices;
}

static vector<uint32> getIndices(const aiMesh& mesh)
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