#ifndef PLAYER_MODEL_H
#define PLAYER_MODEL_H

#include "../entity/entity_model.h"

// loads a female model
EntityModel getPlayerModel() {
    EntityModel model(12);

    // torso
    model.elements[0] = EntityElement(8, 12, 4,    5.0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[0], 16, 16);

    // head
    model.elements[1] = EntityElement(8,  8, 8,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[1], 0, 0);

    // right arm
    model.elements[2] = EntityElement(3, 12, 4,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[2], 40, 16);

    // left arm
    model.elements[3] = EntityElement(3, 12, 4,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[3], 32, 48);

    // right leg
    model.elements[4] = EntityElement(4, 12, 4,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[4], 0, 16);

    // left leg
    model.elements[5] = EntityElement(4, 12, 4,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[5], 16, 48);

    // clothes
    // torso
    model.elements[6] = EntityElement(8, 12, 4,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[6], 16, 32);
    model.elements[6].size += Vec3<f32>(0.5);

    // head
    model.elements[7] = EntityElement(8,  8, 8,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[7], 32, 0);
    model.elements[7].size += Vec3<f32>(0.5); // TODO: 1.0f

    // right arm
    model.elements[8] = EntityElement(3, 12, 4,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[8], 40, 32);
    model.elements[8].size += Vec3<f32>(0.5);

    // left arm
    model.elements[9] = EntityElement(3, 12, 4,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[9], 48, 48);
    model.elements[9].size += Vec3<f32>(0.5);

    // right leg
    model.elements[10] = EntityElement(4, 12, 4,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[10], 0, 32);
    model.elements[10].size += Vec3<f32>(0.5);

    // left leg
    model.elements[11] = EntityElement(4, 12, 4,    0, 0, 0,    0, 0, 0,    6);
    populateUnwrappedEntityElementFaceUVs(model.elements[11], 0, 48);
    model.elements[11].size += Vec3<f32>(0.5);

    return model;
}

#endif