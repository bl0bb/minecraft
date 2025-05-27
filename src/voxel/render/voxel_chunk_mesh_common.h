#ifndef VOXEL_CHUNK_MESH_COMMON_H
#define VOXEL_CHUNK_MESH_COMMON_H

#include "../../core/types.h"

// data1
// x =           0 -  4  (5)      (32)
// y =           5 -  9  (5)      (32)
// z =          10 - 14  (5)      (32)
// face_x =     15 - 18  (4)      (16)
// face_y =     19 - 22  (4)      (16)
// face_depth = 23 - 26  (4)      (16)
// face_w =     27 - 30  (4)      (16)
// face_h =     31 - 34  (4)      (16)
// uv_x =       35 - 38  (4)      (16)
// uv_y =       39 - 42  (4)      (16)
// uv_w =       43 - 46  (4)      (16)
// uv_h =       47 - 50  (4)      (16)
// uv_rot =     51 - 52  (2)       (4)
// dir =        53 - 55  (3)       (8)
// type =       56 - 63  (8)     (256)

// data2
// light =       0 - 19 (20) (1048576)
// ao =         20 - 28  (9)     (512)

struct VoxelFace {
    u64 data1;
    u64 data2;
    u64 data3;
    u64 data4;

    VoxelFace() {}
    
    VoxelFace(u64 x, u64 y, u64 z, u64 face_x, u64 face_y, u64 face_depth, u64 face_w, u64 face_h, u64 uv_x, u64 uv_y, u64 uv_w, u64 uv_h, u64 uv_rot, u64 dir, u64 type, RGBIS4 light[9], u16 ao) :
    data1(  (    x           <<  0) |
            (    y           <<  5) |
            (    z           << 10) |
            (    face_x      << 15) |
            (    face_y      << 19) |
            (    face_depth  << 23) |
            (    face_w      << 27) |
            (    face_h      << 31) |
            (    uv_x        << 35) |
            (    uv_y        << 39) |
            (    uv_w        << 43) |
            (    uv_h        << 47) |
            (    uv_rot      << 51) |
            (    dir         << 53) |
            (    type        << 56)),

    data2(  (u64(light[0])   <<  0) |
            (u64(light[1])   << 20) |
            (u64(light[2])   << 40) |
            (u64(light[3])   << 60)),

    data3(  (u64(light[3])   >>  4) |
            (u64(light[4])   << 16) |
            (u64(light[5])   << 36) |
            (u64(light[6])   << 56)),

    data4(  (u64(light[6])   >>  8) |
            (u64(light[7])   << 12) |
            (u64(light[8])   << 32) |
            (u64(ao)         << 52))
    {}

    inline  u8 getX() const {           return (data1 >>         0) & (     32 - 1); }
    inline  u8 getY() const {           return (data1 >>         5) & (     32 - 1); }
    inline  u8 getZ() const {           return (data1 >>        10) & (     32 - 1); }
    inline  u8 getFaceX() const {       return (data1 >>        15) & (     16 - 1); }
    inline  u8 getFaceY() const {       return (data1 >>        19) & (     16 - 1); }
    inline  u8 getFaceDepth() const {   return (data1 >>        23) & (     16 - 1); }
    inline  u8 getFaceW() const {       return (data1 >>        27) & (     16 - 1); }
    inline  u8 getFaceH() const {       return (data1 >>        31) & (     16 - 1); }
    inline  u8 getUVX() const {         return (data1 >>        35) & (     16 - 1); }
    inline  u8 getUVY() const {         return (data1 >>        39) & (     16 - 1); }
    inline  u8 getUVW() const {         return (data1 >>        43) & (     16 - 1); }
    inline  u8 getUVH() const {         return (data1 >>        47) & (     16 - 1); }
    inline  u8 getUVRot() const {       return (data1 >>        51) & (      4 - 1); }
    inline  u8 getDir() const {         return (data1 >>        53) & (      8 - 1); }
    inline  u8 getType() const {        return (data1 >>        56) & (    256 - 1); }

    // NOT TESTED
    inline u32 getLight0() const {      return (data2 >>         0) & (1048576 - 1); }
    inline u32 getLight1() const {      return (data2 >>        20) & (1048576 - 1); }
    inline u32 getLight2() const {      return (data2 >>        40) & (1048576 - 1); }
    inline u32 getLight3() const {      return (data2 >>        60) & (1048576 - 1) | ((data3 & ((1048576 - 1) >> 4)) << 4); }
    inline u32 getLight4() const {      return (data3 >>        16) & (1048576 - 1); }
    inline u32 getLight5() const {      return (data3 >>        36) & (1048576 - 1); }
    inline u32 getLight6() const {      return (data3 >>        56) & (1048576 - 1) | ((data4 & ((1048576 - 1) >> 8)) << 8); }
    inline u32 getLight7() const {      return (data4 >>        12) & (1048576 - 1); }
    inline u32 getLight8() const {      return (data4 >>        32) & (1048576 - 1); }

    inline u16 getAo() const {          return (data2 >>        52) & (    512 - 1); }
};

#endif