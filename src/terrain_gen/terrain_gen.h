#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H

#include <vector>
#include <algorithm>
#include "../FastNoise/FastNoise.h"
#include "../blocks/blocks.h"






enum BiomeType {
    PLAINS,
    DESERT,
    TUNDRA,
    FOREST
};





class Noise {
public:
    // Create separate noise instances for each terrain feature

    // Terrain shaping
    FastNoise continentalness;
    FastNoise erosion;
    FastNoise peaksValleys;
    FastNoise temperature;
    FastNoise humidity;

    // Decoration
    FastNoise plants;
    FastNoise trees;

    static BiomeType GetBiome(float temperature, float humidity) {
        if (temperature > 0.5f) {
            return humidity > 0.5f ? BiomeType::FOREST : BiomeType::DESERT;
        } else {
            return humidity > 0.5f ? BiomeType::PLAINS : BiomeType::TUNDRA;
        }
    }

    Noise() {
        
    }

    ~Noise() {
        
    }

    void updateNoise() {
        // Terrain shaping

        // Continentalness: Broad, low-frequency noise to represent landmasses vs ocean
        continentalness.SetFrequency(0.005f);  // Very low frequency
        continentalness.SetNoiseType(FastNoise::Perlin);
        continentalness.SetFractalType(FastNoise::FBM);
        continentalness.SetFractalOctaves(4);

        // Erosion: Medium-frequency, detail-focused
        erosion.SetFrequency(0.02f);
        erosion.SetNoiseType(FastNoise::Simplex);
        erosion.SetFractalType(FastNoise::RigidMulti);
        erosion.SetFractalOctaves(5);

        // Peaks and Valleys: High contrast, rugged
        peaksValleys.SetFrequency(0.05f);
        peaksValleys.SetNoiseType(FastNoise::Cellular);
        peaksValleys.SetCellularReturnType(FastNoise::Distance);
        peaksValleys.SetFractalOctaves(0);

        // Temperature: Used for biome classification
        temperature.SetNoiseType(FastNoise::Simplex);
        temperature.SetFrequency(0.001f);
        // temperature.SetFractalType(FastNoise::FBM);
        // temperature.SetFractalOctaves(3);
        temperature.SetFractalOctaves(0);

        // Humidity: Also for biome classification
        humidity.SetNoiseType(FastNoise::Simplex);
        humidity.SetFrequency(0.0015f);
        humidity.SetFractalType(FastNoise::Billow);
        humidity.SetFractalOctaves(4);






        // Decoration
        // Plants: Mid-scale variation
        plants.SetFrequency(0.02f);
        plants.SetNoiseType(FastNoise::Perlin);
        plants.SetFractalType(FastNoise::Billow);
        plants.SetFractalOctaves(4);

        // Trees: Mid-scale variation
        trees.SetFrequency(0.1f);
        trees.SetNoiseType(FastNoise::Perlin);
        trees.SetFractalType(FastNoise::Billow);
        trees.SetFractalOctaves(4);
    }

    void setSeed(int seed) {
        // terrain shaping
        continentalness.SetSeed(seed);
        erosion.SetSeed(seed);
        peaksValleys.SetSeed(seed);
        temperature.SetSeed(seed);
        humidity.SetSeed(seed);

        // decoration
        plants.SetSeed(seed);
        trees.SetSeed(seed);
    }

    int* GenerateHeightMap(i64 chunk_x, i64 chunk_y, i64 chunk_z) {
        int* heightMap = new int[CS_2]{0};
    
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                i64 world_x = (chunk_x * CS) + x;
                i64 world_z = (chunk_z * CS) + z;

                f32 cont = continentalness.GetNoise(world_x, world_z);
                f32 eros = erosion.GetNoise(world_x, world_z);
                f32 peaks = peaksValleys.GetNoise(world_x, world_z);
                f32 heightValue = (cont * 0.5f + eros * 0.3f + peaks * 0.2f);
                heightValue = (heightValue + 1.0f) / 2.0f; // Normalize to 0..1
                heightMap[z + x * CS] = (heightValue * CS * 2) - (chunk_y * CS); // (heightValue * CS) - (chunk_y * CS);
            }
        }

        return heightMap;
    }

    void AssignBaseBlocks(EmbeddedVoxel* terrain, i64 chunk_x, i64 chunk_y, i64 chunk_z, const int* heightMap) {
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                int surfaceY = heightMap[z + x * CS];

                if (surfaceY < 0) {
                    continue;
                }

                surfaceY = std::min(surfaceY, CS - 1);

                for (u8 y = 0; y < CS; y++) {
                    BlockType type;

                    i64 world_y = y + (chunk_y * CS);

                    if (world_y > surfaceY) {
                        if (world_y <= CS * 2 * 0.4) {
                            type = BlockTypes::WATER;
                        } else {
                            type = BlockTypes::AIR;
                        }
                    } else if (world_y == surfaceY) {
                        type = BlockTypes::DIRT;  // Placeholder; will be overwritten in surface layer
                    } else if (world_y > surfaceY - 3) {
                        type = BlockTypes::DIRT;
                    } else {
                        type = BlockTypes::STONE;
                    }

                    terrain[get_zxy_index(x, y, z)] = EmbeddedVoxel(type);
                }
            }
        }
    }


    void ApplySurfaceAndBiomes(EmbeddedVoxel* terrain, i64 chunk_x, i64 chunk_y, i64 chunk_z, const int* heightMap) {
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                i64 world_x = (chunk_x * CS) + x;
                i64 world_z = (chunk_z * CS) + z;

                int surfaceY = heightMap[z + x * CS];

                if (surfaceY < 0) {
                    continue;
                }

                surfaceY = std::min(surfaceY, CS - 1);

                float temp = (temperature.GetNoise(world_x, world_z) + 1.0f) / 2.0f;
                float hum = (humidity.GetNoise(world_x, world_z) + 1.0f) / 2.0f;
                BiomeType biome = GetBiome(temp, hum);

                BlockType type;
                switch (biome) {
                    case BiomeType::PLAINS: type = BlockTypes::GRASS; break;
                    case BiomeType::DESERT: type = BlockTypes::SAND; break;
                    case BiomeType::TUNDRA: type = BlockTypes::SNOW; break;
                    case BiomeType::FOREST: type = BlockTypes::GRASS; break;
                }

                terrain[get_zxy_index(x, surfaceY, z)] = EmbeddedVoxel(type);
            }
        }
    }

    void DecorateTerrain(EmbeddedVoxel* terrain, i64 chunk_x, i64 chunk_y, i64 chunk_z, const int* heightMap) {
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                int y = heightMap[z + x * CS];

                if (y < 0) {
                    continue;
                }

                y = std::min(y, CS - 1);

                i64 world_x = (chunk_x * CS) + x;
                i64 world_z = (chunk_z * CS) + z;

                EmbeddedVoxel voxel = terrain[get_zxy_index(x, y, z)];
                BlockType type = voxel.type;

                float temp = (temperature.GetNoise(world_x, world_z) + 1.0f) / 2.0f;
                float hum = (humidity.GetNoise(world_x, world_z) + 1.0f) / 2.0f;
                BiomeType biome = GetBiome(temp, hum);

                // Random placement (e.g., based on hash or noise)
                // float deco = std::fmod(std::abs(std::sin(world_x * 0.1f + world_z * 0.1f)), 1.0f);
                float plant_val = (plants.GetNoise(world_x, world_z) + 1.0f) / 2.0f;
                float tree_val = (trees.GetNoise(world_x, world_z) + 1.0f) / 2.0f;

                if (type == BlockTypes::GRASS && biome == BiomeType::FOREST && tree_val > 0.8f) {
                    // Add a small tree
                    if (y + 5 < CS && (z >= 2 && z + 2 < CS) && (z >= 2 && z + 2 < CS)) {
                        terrain[get_zxy_index(x, y + 1, z)] = EmbeddedVoxel(BlockTypes::OAK_LOG);
                        terrain[get_zxy_index(x, y + 2, z)] = EmbeddedVoxel(BlockTypes::OAK_LOG);
                        terrain[get_zxy_index(x, y + 3, z)] = EmbeddedVoxel(BlockTypes::OAK_LOG);
                        terrain[get_zxy_index(x, y + 4, z)] = EmbeddedVoxel(BlockTypes::OAK_LOG);

                        for (i8 cy = 0; cy < 2; cy++) {
                            for (i8 cx = -2; cx < 3; cx++) {
                                for (i8 cz = -2; cz < 3; cz++) {
                                    if (cx == 0 && cz == 0) {
                                        continue;
                                    }
                                    terrain[get_zxy_index(x + cx, y + 1 + cy, z + cz)] = EmbeddedVoxel(BlockTypes::OAK_LEAVES);
                                }
                            }
                        }
                        for (i8 cy = 0; cy < 2; cy++) {
                            for (i8 cx = -1; cx < 2; cx++) {
                                for (i8 cz = -1; cz < 2; cz++) {
                                    if (cx == 0 && cz == 0) {
                                        continue;
                                    }
                                    terrain[get_zxy_index(x + cx, y + 3 + cy, z + cz)] = EmbeddedVoxel(BlockTypes::OAK_LEAVES);
                                }
                            }
                        }
                        terrain[get_zxy_index(x, y + 3 + 2, z)] = EmbeddedVoxel(BlockTypes::OAK_LEAVES);

                        terrain[get_zxy_index(x + -1, y + 3 + 1, z + -1)] = EmbeddedVoxel(BlockTypes::AIR);
                        terrain[get_zxy_index(x + -1, y + 3 + 1, z +  1)] = EmbeddedVoxel(BlockTypes::AIR);
                        terrain[get_zxy_index(x +  1, y + 3 + 1, z + -1)] = EmbeddedVoxel(BlockTypes::AIR);
                        terrain[get_zxy_index(x +  1, y + 3 + 1, z +  1)] = EmbeddedVoxel(BlockTypes::AIR);
                    }
                } else if (type == BlockTypes::GRASS && plant_val > 0.95f) {
                    if (y + 1 < CS) {
                        terrain[get_zxy_index(x, y + 1, z)] = EmbeddedVoxel(BlockTypes::POPPY);
                    }
                }
            }
        }
    }

    void GenerateFullTerrain(EmbeddedVoxel* terrain, i64 chunk_x, i64 chunk_y, i64 chunk_z) {
        auto heightMap = GenerateHeightMap(chunk_x, chunk_y, chunk_z);
        AssignBaseBlocks(terrain, chunk_x, chunk_y, chunk_z, heightMap);
        ApplySurfaceAndBiomes(terrain, chunk_x, chunk_y, chunk_z, heightMap);
        DecorateTerrain(terrain, chunk_x, chunk_y, chunk_z, heightMap);
        free(heightMap);
    }
};

#endif