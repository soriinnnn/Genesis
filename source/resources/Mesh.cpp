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

Mesh::Mesh(const MeshDesc& mdesc, const ResourceDesc& rdesc): Resource(rdesc), m_graphicsDevice(mdesc.graphicsDevice) {}

Mesh::~Mesh() {}

VertexBuffer& Mesh::getVertexBuffer()
{
    if (!isLoaded()) {
        GENESIS_LOG_THROW_ERROR("Resource is not loaded: {}", m_path);
    }
    return *m_vertexBuffer;
}

IndexBuffer& Mesh::getIndexBuffer()
{
    if (!isLoaded()) {
        GENESIS_LOG_THROW_ERROR("Resource is not loaded: {}", m_path);
    }
    return *m_indexBuffer;
}

void Mesh::onLoad()
{
    Importer importer;
    const aiScene* scene = importer.ReadFile(
        m_path,
        aiProcess_Triangulate | aiProcess_MakeLeftHanded | aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_GenNormals
    );

    if (!scene) {
        GENESIS_LOG_THROW_ERROR("Failed to load mesh.\nDetails:\n{}", importer.GetErrorString());
    }

    if (scene->mNumMeshes > 1) {
        GENESIS_LOG_WARNING("Mesh \"{}\" contains multiple sub-meshes. Only the first one will be loaded.", m_path);
    }

    aiMesh* mesh = scene->mMeshes[0];
    vector<Vertex> vertices = getVertices(*mesh);
    vector<uint32> indices = getIndices(*mesh);
    m_vertexBuffer = m_graphicsDevice.createVertexBuffer({vertices.data(), static_cast<uint32>(vertices.size()), sizeof(Vertex)});
    m_indexBuffer = m_graphicsDevice.createIndexBuffer({indices.data(), static_cast<uint32>(indices.size())});
}

void Mesh::onUnload()
{
    m_vertexBuffer.reset();
    m_indexBuffer.reset();
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