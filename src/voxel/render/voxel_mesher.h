#ifndef VOXEL_MESHER_H
#define VOXEL_MESHER_H

#include <vector>
#include <unordered_map>

#include "../../core/types.h"
#include "../../core/bits.h"
#include "../../core/array.h"

#include "../../blocks/blocks.h"
#include "../../blocks/mesh/block_mesh.h"

#include "../light/voxel_light_world.h"
#include "../block/voxel_block_world.h"
#include "../blockstate/voxel_blockstate_world.h"

#include "../../blocks/mesh/block_meshes.h"

#include "voxel_chunk_mesh_common.h"





const Vec2<i8> NEIGHBOR_DIRS[] = {
    { 1,  1},
    { 1,  0},
    { 1, -1},
    { 0, -1},
    {-1, -1},
    {-1,  0},
    {-1,  1},
    { 0,  1},
    { 0,  0},
};



constexpr inline void dim_to_pos(u8& x, u8& y, u8& z, u8 dim_1, u8 dim_2, u8 dim_3, u8 axis) {
    if (axis == 0) {
        x = dim_1;
        y = dim_2;
        z = dim_3;
    } else if (axis == 1) {
        z = dim_1;
        x = dim_2;
        y = dim_3;
    } else {
        x = dim_1;
        z = dim_2;
        y = dim_3;
    }
}

template<typename T>
constexpr inline void getFaceOffset(T& x, T& y, T& z, u8 axis) {
    if (axis == 0) {
        x++;
    } else if (axis == 1) {
        x--;
    } else if (axis == 2) {
        y++;
    } else if (axis == 3) {
        y--;
    } else if (axis == 4) {
        z++;
    } else {
        z--;
    }
}

inline Mat4<f32> getFaceOrig(u8 axis) {
    Mat4<f32> orig = Mat4<f32>::identity();
    orig = orig.translate(0.5, 0.5, 0.5);
    if (axis == 0) {
        orig = orig.rotate(0, Math::deg_to_rad<f32>(-90), 0);
    } else if (axis == 1) {
        orig = orig.rotate(0, Math::deg_to_rad<f32>(90), 0);
    } else if (axis == 2) {
        orig = orig.rotate(Math::deg_to_rad<f32>(90), 0, 0);
    } else if (axis == 3) {
        orig = orig.rotate(Math::deg_to_rad<f32>(-90), 0, 0);
    } else if (axis == 4) {
        orig = orig.rotate(0, Math::deg_to_rad<f32>(180), 0);
    } else {
        orig = orig.rotate(0, Math::deg_to_rad<f32>(0), 0);
    }
    orig = orig.translate(-0.5, -0.5, -0.5);
    return orig;
}


u32 generate_voxel_mesh(const VoxelBlockWorld& voxelWorld, const VoxelBlockStateWorld& voxelBlockStateWorld, const VoxelLightWorld& voxelLightWorld, const Vec3<i64>& chunkPos, VoxelFace* vertices) {
    VoxelBlockChunk&           chunk =           voxelWorld.chunks[          voxelWorld.chunkPosToChunkIndex(chunkPos.x, chunkPos.y, chunkPos.z)];
    BlockStateVoxelChunk& stateChunk = voxelBlockStateWorld.chunks[voxelBlockStateWorld.chunkPosToChunkIndex(chunkPos.x, chunkPos.y, chunkPos.z)];
    VoxelLightChunk&      lightChunk =      voxelLightWorld.chunks[     voxelLightWorld.chunkPosToChunkIndex(chunkPos.x, chunkPos.y, chunkPos.z)];
    (void)lightChunk;
    
    // how the below 3 arrays work
    // each element represents a potential visible face to be rendered
    // a block on the x axis has a right face (positive) and a left face (negative)

    // presence of faces
    // voxel as binary for each x,y,z axis, positive and negative
    u64 axis_cols[3 * CS_P2 * 2] = {0};
    // opaque voxel as binary for each x,y,z axis, positive and negative
    // keep track of opaque faces as they should be rendered separately from the rest of the faces to make the transparent parts work correctly
    u64 opaque_cols[3 * CS_P2 * 2] = {0};
    // merge voxel as binary for each x,y,z axis, positive and negative
    // a merge voxel: if two merge voxels are next to each other, the faces should not be visible (e.g. two regular voxels next to each other have the touching faces hidden)
    u64 merge_cols[3 * CS_P2 * 2] = {0};

    // the cull mask to perform greedy slicing, based on solids on previous axis_cols
    u64 col_face_masks[3 * CS_P2 * 2] = {0};

    // load voxels and block states with padding for quick lookup
    EmbeddedVoxel* blocks[CS_P3] = {nullptr};
    BlockStateStruct* blockStates[CS_P3] = {nullptr};



    auto addVoxelAxis = [&voxelBlockStateWorld, &axis_cols, &blocks, &blockStates, &merge_cols, &opaque_cols](EmbeddedVoxel* voxel, u8 x, u8 y, u8 z, i64 world_x, i64 world_y, i64 world_z) {
        BlockVoxelData blockData = BLOCK_VOXEL_DATA[voxel->type];

        BlockStateStruct* state = VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, world_x, world_y, world_z)->state;
        BlockMesh blockMesh = BLOCK_MESHES[blockData.meshType](*state);

        blocks[get_zxy_index_p(x, y, z)] = voxel;
        blockStates[get_zxy_index_p(x, y, z)] = state;

        axis_cols[z + (y * CS_P) + CS_P2 * 0] |= (u64)1 << x;
        axis_cols[z + (y * CS_P) + CS_P2 * 1] |= (u64)1 << x;
        axis_cols[x + (z * CS_P) + CS_P2 * 2] |= (u64)1 << y;
        axis_cols[x + (z * CS_P) + CS_P2 * 3] |= (u64)1 << y;
        axis_cols[x + (y * CS_P) + CS_P2 * 4] |= (u64)1 << z;
        axis_cols[x + (y * CS_P) + CS_P2 * 5] |= (u64)1 << z;

        // z,y - x axis
        if (blockMesh.culls(0)) {
            // positive
            merge_cols[z + (y * CS_P) + CS_P2 * 0] |= (u64)1 << x;
        }
        if (blockMesh.culls(1)) {
            // negative
            merge_cols[z + (y * CS_P) + CS_P2 * 1] |= (u64)1 << x;
        }

        // x,z - y axis
        if (blockMesh.culls(2)) {
            // positive
            merge_cols[x + (z * CS_P) + CS_P2 * 2] |= (u64)1 << y;
        }
        if (blockMesh.culls(3)) {
            // negative
            merge_cols[x + (z * CS_P) + CS_P2 * 3] |= (u64)1 << y;
        }

        // x,y - z axis
        if (blockMesh.culls(4)) {
            // positive
            merge_cols[x + (y * CS_P) + CS_P2 * 4] |= (u64)1 << z;
        }
        if (blockMesh.culls(5)) {
            // negative
            merge_cols[x + (y * CS_P) + CS_P2 * 5] |= (u64)1 << z;
        }

        if (blockData.transparent == false) {
            opaque_cols[z + (y * CS_P) + CS_P2 * 0] |= (u64)1 << x;
            opaque_cols[z + (y * CS_P) + CS_P2 * 1] |= (u64)1 << x;
            opaque_cols[x + (z * CS_P) + CS_P2 * 2] |= (u64)1 << y;
            opaque_cols[x + (z * CS_P) + CS_P2 * 3] |= (u64)1 << y;
            opaque_cols[x + (y * CS_P) + CS_P2 * 4] |= (u64)1 << z;
            opaque_cols[x + (y * CS_P) + CS_P2 * 5] |= (u64)1 << z;
        }
    };


    // build binary representation for every solid voxel y,x,z axis
    // voxels in this chunk
    for (u8 y = 0; y < CS; y++) {
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                u8 ax = x + 1;
                u8 ay = y + 1;
                u8 az = z + 1;

                i64 world_x = ax - 1 + chunkPos.x * CS;
                i64 world_y = ay - 1 + chunkPos.y * CS;
                i64 world_z = az - 1 + chunkPos.z * CS;

                EmbeddedVoxel* voxel = &chunk.voxels[get_zxy_index(x, y, z)];
                if (voxel->type == BlockTypes::AIR) {
                    continue;
                }

                // TODO: check if voxel is solid (not see through)
                addVoxelAxis(voxel, ax, ay, az, world_x, world_y, world_z);
            }
        }
    }


    // voxels in neighboring chunks
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 dim_1 = 0; dim_1 < CS_P; dim_1++) {
            for (u8 dim_2 = 0; dim_2 < CS_P; dim_2++) {
                for (u8 dim_3 = 0; dim_3 < CS_P; dim_3 += CS_P - 1) {
                    u8 x, y, z;
                    if (axis == 0) {
                        z = dim_1;
                        x = dim_3;
                        y = dim_2;
                    } else if (axis == 1) {
                        x = dim_1;
                        y = dim_3;
                        z = dim_2;
                    } else {
                        x = dim_1;
                        z = dim_3;
                        y = dim_2;
                    }

                    i64 world_x = x - 1 + chunkPos.x * CS;
                    i64 world_y = y - 1 + chunkPos.y * CS;
                    i64 world_z = z - 1 + chunkPos.z * CS;

                    
                    // TODO: check if voxel is solid (not see through)
                    EmbeddedVoxel* voxel;
                    bool has_voxel = VoxelWorlds::getVoxel(voxelWorld, world_x, world_y, world_z, &voxel);

                    if (has_voxel == false) {
                        continue;
                    }
                    if (voxel->type == BlockTypes::AIR) {
                        continue;
                    }

                    
                    addVoxelAxis(voxel, x, y, z, world_x, world_y, world_z);
                }
            }
        }
    }




    
    // face culling
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 j = 0; j < 2; j++) {
            u8 dir = axis * 2 + j;
            for (u16 i = 0; i < CS_P2; i++) {
                u64 axis_col = axis_cols[(CS_P2 * dir) + i];
                u64 merge_col = merge_cols[(CS_P2 * dir) + i];
                u64 opaque_col = opaque_cols[(CS_P2 * dir) + i];

                u64 other_axis_col;
                u64 other_merge_col;
                u64 other_opaque_col;

                if (j == 0) {
                    // positive
                    other_axis_col = axis_col >> 1;
                    other_merge_col = merge_col >> 1;
                    other_opaque_col = opaque_col >> 1;
                } else {
                    // negative
                    other_axis_col = axis_col << 1;
                    other_merge_col = merge_col << 1;
                    other_opaque_col = opaque_col << 1;
                }

                // get the mask that contains the faces to be rendered
                u64 mask = axis_col & (              // only create a face if this is an actual block
                    (merge_col & ~other_axis_col) |  // merge meets air
                    (merge_col & ~other_merge_col) | // merge meets non-merge
                    (~merge_col)                     // non-merge will not merge with anything, so if this face is a non-merge, always return true
                );

                col_face_masks[(CS_P2 * dir) + i] = mask;
            }
        }
    }








    // meshing
    
    // index
    u32 vertexIdx = 0;

    // generate quads
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 j = 0; j < 2; j++) {
            u8 dir = axis * 2 + j;
            for (u8 dim_2 = 0; dim_2 < CS; dim_2++) {
                for (u8 dim_1 = 0; dim_1 < CS; dim_1++) {
                    u32 index = (dim_1 + 1) + ((dim_2 + 1) * CS_P) + (CS_P2 * dir);
                    u64 face_mask = col_face_masks[index];

                    // remove padding right
                    face_mask >>= 1;
                    // remove padding left
                    face_mask &= ~(u64(1) << CS);

                    while (face_mask) {
                        u8 dim_3 = pop_lsb(face_mask);

                        u8 x, y, z;
                        if (axis == 0) {
                            z = dim_1;
                            y = dim_2;
                            x = dim_3;
                        } else if (axis == 1) {
                            x = dim_1;
                            z = dim_2;
                            y = dim_3;
                        } else {
                            x = dim_1;
                            y = dim_2;
                            z = dim_3;
                        }




                        // ao / light
                        u16 ao_mask = 0;
                        RGBIS4 lightSources[9];
                        for (i8 neighbor_i = 8; neighbor_i >= 0; neighbor_i--) {
                            Vec2<i8> neighbor_offset = NEIGHBOR_DIRS[neighbor_i];
                            Vec3<i64> neighbor_pos;

                            if (dir == 0)       neighbor_pos = { 1, neighbor_offset.y, neighbor_offset.x};
                            else if (dir == 1)  neighbor_pos = {-1, neighbor_offset.y, neighbor_offset.x};
                            else if (dir == 2)  neighbor_pos = {neighbor_offset.x,  1, neighbor_offset.y};
                            else if (dir == 3)  neighbor_pos = {neighbor_offset.x, -1, neighbor_offset.y};
                            else if (dir == 4)  neighbor_pos = {neighbor_offset.x, neighbor_offset.y,  1};
                            else                neighbor_pos = {neighbor_offset.x, neighbor_offset.y, -1};









                            // raw position
                            Vec3<i64> voxel_pos_raw = Vec3<i64>(x, y, z) + neighbor_pos;
                            // position in chunk (add one cuz of padding)
                            Vec3<i64> voxel_pos = Vec3<i64>(1) + voxel_pos_raw;
                            // voxel pos in the world
                            Vec3<i64> voxel_pos_world = (chunk.pos * CS) + voxel_pos_raw;

                            EmbeddedVoxel* voxel = blocks[get_zxy_index_p(voxel_pos.x, voxel_pos.y, voxel_pos.z)];
                            if (!voxel) {
                                RGBIS4* lightPtr;
                                lightSources[neighbor_i] = VoxelWorlds::getVoxel(voxelLightWorld, voxel_pos_world.x, voxel_pos_world.y, voxel_pos_world.z, &lightPtr) ? *lightPtr : Colors::createRGBIS4(Colors::COLOR4_MAX);
                                continue;
                            }

                            // ao
                            // TODO: is hardcoded air checking needed?
                            if (voxel->type == BlockTypes::AIR) {
                                RGBIS4* lightPtr;
                                lightSources[neighbor_i] = VoxelWorlds::getVoxel(voxelLightWorld, voxel_pos_world.x, voxel_pos_world.y, voxel_pos_world.z, &lightPtr) ? *lightPtr : Colors::createRGBIS4(Colors::COLOR4_MAX);
                                continue;
                            }

                            BlockVoxelData blockData = BLOCK_VOXEL_DATA[voxel->type];

                            if (blockData.transparent) {
                                RGBIS4* lightPtr;
                                lightSources[neighbor_i] = VoxelWorlds::getVoxel(voxelLightWorld, voxel_pos_world.x, voxel_pos_world.y, voxel_pos_world.z, &lightPtr) ? *lightPtr : Colors::createRGBIS4(Colors::COLOR4_MAX);
                                continue;
                            }

                            BlockStateStruct* state = blockStates[get_zxy_index_p(voxel_pos.x, voxel_pos.y, voxel_pos.z)];
                            BlockMesh blockMesh = BLOCK_MESHES[blockData.meshType](*state);
                            if (!blockMesh.culls(0)) {
                                RGBIS4* lightPtr;
                                lightSources[neighbor_i] = VoxelWorlds::getVoxel(voxelLightWorld, voxel_pos_world.x, voxel_pos_world.y, voxel_pos_world.z, &lightPtr) ? *lightPtr : Colors::createRGBIS4(Colors::COLOR4_MAX);
                                continue;
                            }






                            
                            // light
                            lightSources[neighbor_i] = lightSources[8];



                            // ao
                            ao_mask |= 1 << neighbor_i;
                        }



                        // i64 world_x = x + chunkPos.x * CS;
                        // i64 world_y = y + chunkPos.y * CS;
                        // i64 world_z = z + chunkPos.z * CS;
                        // BlockStateStruct* state = VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, world_x, world_y, world_z)->state;

                        BlockStateStruct* state = stateChunk.voxels[get_zxy_index(x, y, z)].state;

                        BlockVoxelData blockData = BLOCK_VOXEL_DATA[chunk.voxels[get_zxy_index(x, y, z)].type];
                        
                        BlockTexture blockTexture = blockData.get_texture(*state, dir);
                        BlockMesh blockMesh = BLOCK_MESHES[blockData.meshType](*state);

                        for (u8 i = 0; i < blockMesh.elementCount; i++) {
                            BlockElement& element = blockMesh.elements[i];

                            Vec3<f32> rot = element.rot();

                            Mat4<f32> origMat = Mat4<f32>::identity();
                            if (rot != Vec3<f32>(0, 0, 0)) {
                                origMat = origMat.rotate(Math::deg_to_rad<f32>(rot.x), Math::deg_to_rad<f32>(rot.y), Math::deg_to_rad<f32>(rot.z));
                            }
                            
                            for (u8 j = 0; j < element.facesCount; j++) {
                                BlockFace& face = element.faces[j];
                                if (face.dir != dir) {
                                    continue;
                                }

                                f32 fromX = element.fromX / 16.0f;
                                f32 fromY = element.fromY / 16.0f;
                                f32 fromZ = element.fromZ / 16.0f;

                                f32 toX = element.toX / 16.0f;
                                f32 toY = element.toY / 16.0f;
                                f32 toZ = element.toZ / 16.0f;

                                f32 sizeX = toX - fromX;
                                f32 sizeY = toY - fromY;
                                f32 sizeZ = toZ - fromZ;

                                f32 width;
                                f32 height;
                                if (dir & 0b100) {
                                    width = element.toX - element.fromX;
                                    height = element.toY - element.fromY;
                                } else if (dir & 0b10) {
                                    width = element.toX - element.fromX;
                                    height = element.toZ - element.fromZ;
                                } else {
                                    width = element.toZ - element.fromZ;
                                    height = element.toY - element.fromY;
                                }

                                Mat4<f32> startPos = Mat4<f32>::identity().translate(fromX, fromY, fromZ);

                                Mat4<f32> translated = origMat * Mat4<f32>::identity().translateWorld(element.rotOrig() / 16.0f * -1.0f) * startPos; // move to the rotation origin and rotate, then 

                                Mat4<f32> faceOrig = getFaceOrig(dir); // vertex pos for face
                                Vec3<f32> origPos = faceOrig.extractPosition(); // get only the pos
                                translated = translated.translate(origPos.x * sizeX, origPos.y * sizeY, origPos.z * sizeZ); // move the face to fit the cube
                                translated = translated * faceOrig.translateWorld(origPos * -1.0f); // remove the position from the face matrix and rotate the face to fit the cube

                                translated = translated.translateWorld(element.rotOrig() / 16.0f); // move back from the rotation origin

                                Vec3<f32> translatedPos = translated.extractPosition() * 16.0f;
                                Vec3<f32> translatedRot = translated.getEulerAnglesXYZ();

                                vertices[vertexIdx++] = VoxelFace(
                                    x, y, z,

                                    translatedPos.x, translatedPos.y, translatedPos.z,

                                    width, height,

                                    translatedRot.x, translatedRot.y, translatedRot.z,

                                    face.uvRot,

                                    face.uvFromX, face.uvFromY,

                                    face.uvToX - face.uvFromX, face.uvToY - face.uvFromY,

                                    dir,
                                    blockTexture,
                                    lightSources,
                                    ao_mask
                                );
                            }
                        }
                    }
                }
            }
        }
    }

    return vertexIdx;
}

#endif