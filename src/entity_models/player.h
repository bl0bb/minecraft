#ifndef PLAYER_MODEL_H
#define PLAYER_MODEL_H

#include "../entity/entity_model.h"

// loads a female model
EntityModel getPlayerModel() {
    EntityModel model(6);

    // torso
    model.elements[0] = EntityElement(8, 12, 4,    0, 0, 0,    0, 0, 0,    6);
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

    return model;
}

#endif