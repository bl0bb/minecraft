#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H

#include <vector>
#include <algorithm>
#include "../FastNoise/FastNoise.h"
#include "../voxel/voxel_mesher.h"
#include "../voxel/voxel.h"
#include "../blocks.h"








class Noise {
public:
    FastNoise noise;

    Noise() {
        
    }

    ~Noise() {
        
    }

    void generateTerrain(EmbeddedVoxel* voxels, int offsetX, int offsetY, int offsetZ, int seed) {
        noise.SetSeed(seed);

        for (int x = 0; x < CS; x++) {
            for (int y = 0; y < CS; y++) {
                for (int z = 0; z < CS; z++) {
                    u64 world_x = (offsetX * CS) + x;
                    u64 world_y = (offsetY * CS) + y;
                    u64 world_z = (offsetZ * CS) + z;

                    f32 y_delta = f32((offsetY * CS) + y) / f32(CS * 4);




                    // bumps
                    f32 world_noise = 0;

                    noise.SetFrequency(0.001f);
                    noise.SetNoiseType(FastNoise::Perlin);
                    world_noise += ((noise.GetNoise(world_x, world_y, world_z) + 1.0f) / 2.0f) * 0.5f;

                    noise.SetFrequency(0.01f);
                    noise.SetNoiseType(FastNoise::Perlin);
                    world_noise += ((noise.GetNoise(world_x, world_y, world_z) + 1.0f) / 2.0f) * 0.3f;

                    noise.SetFrequency(0.04f);
                    noise.SetNoiseType(FastNoise::Perlin);
                    world_noise += ((noise.GetNoise(world_x, world_y, world_z) + 1.0f) / 2.0f) * 0.2f;

                    world_noise /= pow(y_delta * 1.0f, 0.2f);




                    // cliffs
                    f32 cliff_noise = 0;

                    if (true) {
                        noise.SetFrequency(0.01f);
                        noise.SetNoiseType(FastNoise::Perlin);
                        f32 tmp_noise = (noise.GetNoise(world_x, world_y, world_z) + 1.0f);
                        cliff_noise = (1.0f - pow(1.0f - tmp_noise * 2.0f, 3.0f)) / 2.0f;
                        cliff_noise /= pow(y_delta * 4.0f, 2.0f);
                    } else {
                        cliff_noise = 1.0f;
                    }






                    f32 final_val = world_noise * cliff_noise;






                    if (final_val > 1.0f) {
                        int i = get_zxy_index(x, y, z);
                        voxels[i] = BlockType::STONE + 1;
                    }
                }
            }
        }

        // for (int x = 0; x < CS; x++) {
        //     for (int z = 0; z < CS; z++) {
        //         for (int y = CS - 1 - 1; y >= 0; y--) {
        //             int this_i = get_zxy_index(x, y, z);
        //             if (voxels[this_i] == 0) {
        //                 continue;
        //             }
        //             int above_i = get_zxy_index(x, y + 1, z);
        //             if (voxels[above_i] == 0) {
        //                 voxels[this_i] = BlockType::GRASS + 1;
        //                 for (int i = 0; i < 3; i++) {
        //                     int below_i = get_zxy_index(x, y - 1 - i, z);
        //                     if (voxels[below_i] != 0) {
        //                         voxels[below_i] = BlockType::DIRT + 1;
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }

        // voxels[get_zxy_index(0, 31, 0)] = BlockType::STONE + 1;
    }
};

#endif