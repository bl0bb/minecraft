#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "core/maths.h"
#include "core/camera.h"
#include "core/shader.h"

#include "voxel/voxel_mesher.h"
#include "voxel/voxel_renderer.h"

// TODO: add quad support for rendering and for obj importing??










inline int get_zxy_index(int x, int y, int z) {
    return z + (x * CS_P) + (y * CS_P2);
}










constexpr u16 WINDOW_WIDTH = 1024;
constexpr u16 WINDOW_HEIGHT = 1024;

int main() {
    MeshData meshData;
    meshData.faces = new std::vector<uint64_t>(10000);

    u8* voxels = new u8[CS_P3]{0};
    memset(voxels, 0, CS_P3);

    // int r = CS_P / 2;
    // for (int x = -r; x < r; x++) {
    //     for (int y = -r; y < r; y++) {
    //         for (int z = -r; z < r; z++) {
    //             if (std::sqrt(x * x + y * y + z * z) < 30.0f) {
    //                 voxels[get_zxy_index(x + r, y + r, z + r)] = 1;
    //             }
    //         }
    //     }
    // }

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            for (int z = 0; z < 5; z++) {
                voxels[get_zxy_index(20 + x, 20 + y, 20 + z)] = 1;
            }
        }
    }

    generate_voxel_mesh(voxels, meshData);

    return 0;
}