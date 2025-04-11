#include "ChunkMesher.h"

#include <array>
#include <iostream>
#include <unordered_map>

namespace engine::world::voxel {
    struct FaceVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 uv;
    };

    struct VoxelTypeInfo {
        int topTexture;
        int bottomTexture;
        int sideTexture;
    };

    std::unordered_map<VoxelType, VoxelTypeInfo> voxelTextures = {
    { VoxelType::VoxelType_Grass,  {0, 2, 3} },  // top=grass, bottom=dirt, side=grass_side
    { VoxelType::VoxelType_Default,   {1, 1, 1} },
    };

    const std::array<FaceVertex, 4>& getFaceVertices(int face) {
        static const std::array<std::array<FaceVertex, 4>, 6> faces = {
            // 0 - Back face (Z-)
            std::array<FaceVertex, 4>{{
                {{0, 0, 0}, { 0,  0, -1}, {0, 0, 0}},
                {{1, 0, 0}, { 0,  0, -1}, {1, 0, 0}},
                {{1, 1, 0}, { 0,  0, -1}, {1, 1, 0}},
                {{0, 1, 0}, { 0,  0, -1}, {0, 1, 0}},
            }},
            // 1 - Front face (Z+)
            std::array<FaceVertex, 4>{{
                {{1, 0, 1}, { 0,  0,  1}, {1, 0, 0}},
                {{0, 0, 1}, { 0,  0,  1}, {0, 0, 0}},
                {{0, 1, 1}, { 0,  0,  1}, {0, 1, 0}},
                {{1, 1, 1}, { 0,  0,  1}, {1, 1, 0}},
            }},
            // 2 - Bottom face (Y-)
            std::array<FaceVertex, 4>{{
                {{0, 0, 1}, { 0, -1,  0}, {0, 0, 0}},
                {{1, 0, 1}, { 0, -1,  0}, {1, 0, 0}},
                {{1, 0, 0}, { 0, -1,  0}, {1, 1, 0}},
                {{0, 0, 0}, { 0, -1,  0}, {0, 1, 0}},
            }},
            // 3 - Top face (Y+)
            std::array<FaceVertex, 4>{{
                {{0, 1, 0}, { 0,  1,  0}, {0, 0, 0}},
                {{1, 1, 0}, { 0,  1,  0}, {1, 0, 0}},
                {{1, 1, 1}, { 0,  1,  0}, {1, 1, 0}},
                {{0, 1, 1}, { 0,  1,  0}, {0, 1, 0}},
            }},
            // 4 - Left face (X-)
            std::array<FaceVertex, 4>{{
                {{0, 0, 1}, {-1,  0,  0}, {1, 0, 0}},
                {{0, 0, 0}, {-1,  0,  0}, {0, 0, 0}},
                {{0, 1, 0}, {-1,  0,  0}, {0, 1, 0}},
                {{0, 1, 1}, {-1,  0,  0}, {1, 1, 0}},
            }},
            // 5 - Right face (X+)
            std::array<FaceVertex, 4>{{
                {{1, 0, 0}, { 1,  0,  0}, {0, 0, 0}},
                {{1, 0, 1}, { 1,  0,  0}, {1, 0, 0}},
                {{1, 1, 1}, { 1,  0,  0}, {1, 1, 0}},
                {{1, 1, 0}, { 1,  0,  0}, {0, 1, 0}},
            }},
        };
        return faces[face];
    }

    glm::ivec3 getFaceOffset(int face) {
        static const glm::ivec3 offsets[6] = {
            { 0,  0, -1}, // 0 - Back (Z-)
            { 0,  0,  1}, // 1 - Front (Z+)
            { 0, -1,  0}, // 2 - Bottom (Y-)
            { 0,  1,  0}, // 3 - Top (Y+)
            {-1,  0,  0}, // 4 - Left (X-)
            { 1,  0,  0}, // 5 - Right (X+)
        };
        return offsets[face];
    }

    bool isFaceVisible(const Chunk& chunk, const glm::ivec3& pos, int face) {
        glm::ivec3 neighborPos = pos + getFaceOffset(face);

        if (!chunk.inBounds(neighborPos)) {
            // Outside chunk = visible by default (unless you check neighbor chunks)
            return true;
        }

        Voxel neighbor = chunk.getVoxel(neighborPos);
        return neighbor.type == VoxelType::VoxelType_Air;
    }


    void ChunkMesher::generateChunkMesh(Chunk& chunk)
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> uv3D;
        std::vector<uint32_t> indices;

        uint32_t indexOffset = 0;
        for (int y = 0; y < chunk.maxHeight; y++) {
            for (int x = 0; x < Chunk::SizeX; x++) {
                for (int z = 0; z < Chunk::SizeZ; z++) {
                    glm::ivec3 voxelPos {x, y, z};
                    Voxel voxel = chunk.getVoxel(voxelPos);
                    if (voxel.type == VoxelType::VoxelType_Air) continue;
                    VoxelTypeInfo typeInfo = voxelTextures[voxel.type];
                    for (int face = 0; face < 6; ++face) {
                        // Check if the face should be rendered (e.g., if neighbor voxel is air or out-of-bounds)
                        if (!isFaceVisible(chunk, voxelPos, face))
                            continue;

                        // For the current face, get the predefined vertices data
                        // (positions are relative to the voxel, normals, and uv coordinates)
                        const auto& faceVerts = getFaceVertices(face);

                        int textureIndex;
                        switch (face) {
                        case 0: textureIndex = typeInfo.sideTexture; break;  // -Z
                        case 1: textureIndex = typeInfo.sideTexture; break; // +Z
                        case 2: textureIndex = typeInfo.bottomTexture; break; // -Y
                        case 3: textureIndex = typeInfo.topTexture; break;    // +Y
                        case 4: textureIndex = typeInfo.sideTexture; break;   // -X
                        case 5: textureIndex = typeInfo.sideTexture; break;  // +X
                        }

                        // For each of the 4 vertices of the face
                        for (int i = 0; i < 4; ++i) {
                            // Calculate the world-space position for this vertex
                            glm::vec3 worldPos = glm::vec3(voxelPos) + faceVerts[i].position;
                            vertices.push_back(worldPos);

                            // Normal for the face (same for all four vertices)
                            const glm::vec3& n = faceVerts[i].normal;
                            normals.push_back(n);

                            // UV coordinates (or any 3D texture coordinate, if used)
                            glm::vec3 uv = faceVerts[i].uv;
                            uv.z = float(textureIndex); // Each voxel type maps to a texture layer
                            uv3D.push_back(uv);
                        }

                        // Define two triangles for the quad (4 vertices per face)
                        indices.push_back(indexOffset + 0);
                        indices.push_back(indexOffset + 1);
                        indices.push_back(indexOffset + 2);
                        indices.push_back(indexOffset + 2);
                        indices.push_back(indexOffset + 3);
                        indices.push_back(indexOffset + 0);

                        indexOffset += 4;
                    }
                }
            }
        }

        chunk.mesh = std::make_unique<asset::Mesh>();
        chunk.mesh->upload(vertices, normals, {}, uv3D, indices);
    }

    std::vector<glm::ivec3> ChunkMesher::findVoxelsToRender(const Chunk* chunk)
    {
        std::vector<glm::ivec3> voxelsToRender;

        for (int y = 0; y < chunk->maxHeight; y++) {
            for (int x = 0; x < Chunk::SizeX; x++) {
                for (int z = 0; z < Chunk::SizeZ; z++) {
                    glm::ivec3 voxelPos {x, y, z};
                    Voxel voxel = chunk->getVoxel(voxelPos);
                    if (voxel.type == VoxelType::VoxelType_Air) continue;
                    if (z > 0 && chunk->getVoxel(x, y, z - 1).type != VoxelType::VoxelType_Air && z < Chunk::SizeZ - 1 && chunk->getVoxel(x, y, z + 1).type != VoxelType::VoxelType_Air &&
                        y > 0 && chunk->getVoxel(x, y - 1, z).type != VoxelType::VoxelType_Air && y < Chunk::SizeY - 1 && chunk->getVoxel(x, y + 1, z).type != VoxelType::VoxelType_Air &&
                        x > 0 && chunk->getVoxel(x - 1, y, z).type != VoxelType::VoxelType_Air && x < Chunk::SizeX - 1 && chunk->getVoxel(x + 1, y, z).type != VoxelType::VoxelType_Air) {
                        continue;
                    }
                    voxelsToRender.push_back(voxelPos);
                }
            }
        }

        return voxelsToRender;
    }
}
