#ifndef ENTITY_MODEL_H
#define ENTITY_MODEL_H

#include "../core/types.h"
#include "../core/maths.h"



// defines a box to be rendered in an entity (e.g. one part for the right arm of a player and another one for the second layer of the right arm)
struct EntityElementFace {
    // 0 = right
    // 1 = left
    // 2 = top
    // 3 = bottom
    // 4 = back
    // 5 = front
    u8 dir;

    Vec2<u16> uvFrom;
    Vec2<u16> uvTo;

    EntityElementFace(u8 _dir, u16 _uvFromX, u16 _uvFromY, u16 _uvToX, u16 _uvToY) : dir(_dir), uvFrom(_uvFromX, _uvFromY), uvTo(_uvToX, _uvToY) {}
};

struct EntityElement {
    /*
    // visual representation of c0/c1, assume this is a players arm
    +----+ transform +--------+
    |    |     v     |        |
    | c0>| <-------> |<c1     |
    |    |           |        |
    |    |           |        |
    |    |           |        |
    |    |           |        |
    +----+           +--------+
    ^ block origin
    */

    Vec3<f32> size; // the size of the part

    // since this is a blocky game, im storing c0/c1 as positional offsets, not full matrix offsets as i cant imagine a model needing c0/c1 to have rotational offsets
    // c0/c1 are relative to their connected parts, not the world
    Vec3<f32> c0; // c0 (connection 0) is the offset from the blocks origin
    Vec3<f32> c1; // c1 (connection 1) is the offset from the other part this is connected to
    Mat4<f32> transform; // used for animating and visually moving the part

    u8 faceCount;
    EntityElementFace* faces;

    EntityElement(
        f32 _sizeX,
        f32 _sizeY,
        f32 _sizeZ,

        f32 _c0X,
        f32 _c0Y,
        f32 _c0Z,

        f32 _c1X,
        f32 _c1Y,
        f32 _c1Z,

        u8 _faceCount) :
        size(_sizeX, _sizeY, _sizeZ),

        c0(_c0X, _c0Y, _c0Z),
        c1(_c1X, _c1Y, _c1Z),
        transform(Mat4<f32>::identity()),

        faceCount(_faceCount),
        faces((EntityElementFace*)malloc(sizeof(EntityElementFace) * faceCount))
        {}
};

struct EntityModel {
    u8 elementCount;
    EntityElement* elements;

    EntityModel(u8 _elementCount) :
        elements((EntityElement*)malloc(sizeof(EntityElement) * _elementCount)),
        elementCount(_elementCount)
        {}
};






// minecraft unwraps / lays out textures for blocky parts in a certain order, so, given the size of the block in pixels, we can automatically find the uv coordinates for each face, thats what this function does
void populateUnwrappedEntityElementFaceUVs(EntityElement& element, u16 startX, u16 startY) {
    auto x = element.size.x;
    auto y = element.size.y;
    auto z = element.size.z;
    element.faces[0] = EntityElementFace(0,        0, z,        z, z + y);
    element.faces[1] = EntityElementFace(1,    z * 2, z,    z * 3, z + y);
    element.faces[2] = EntityElementFace(2,    z * 1, 0,    z * 2,     z);
    element.faces[3] = EntityElementFace(3,    z * 2, 0,    z * 3,     z);
    element.faces[4] = EntityElementFace(4,    z * 3, z,    z * 4, z + y);
    element.faces[5] = EntityElementFace(5,    z * 1, z,    z * 2, z + y);
}





#endif