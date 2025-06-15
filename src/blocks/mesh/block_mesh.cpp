#include "block_mesh.h"

bool BlockMesh::culls(u8 dir) {
    for (u8 i = 0; i < elementCount; i++) {
        BlockElement& element = elements[i];
        for (u8 j = 0; j < element.facesCount; j++) {
            BlockFace& face = element.faces[j];
            if (face.dir != dir) {
                continue;
            }
            if (face.culls) {
                return true;
            }
        }
    }
    return false;
}