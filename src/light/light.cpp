// TODO: ugly ahh way of getting element at index in queue
template <typename T>
T getElementAt(std::queue<T> q, size_t index) {
    if (index >= q.size()) {
        throw std::out_of_range("Index out of range");
    }
    for (size_t i = 0; i < index; ++i) {
        q.pop();
    }
    return q.front();
}

void ChunkLight::add_propagate(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, AllLightQueue& lightQueue, u32 mask, u32 offset, LightPropagationType type) {
    while (!lightQueue.empty()) {
        auto [pos, _] = lightQueue.front();
        lightQueue.pop();

        RGBIS4* lightPtr;
        if (!VoxelWorlds::getVoxel(voxelLightWorld, pos.x, pos.y, pos.z, &lightPtr)) {
            continue;
        }
        RGBIS4 light = *lightPtr;

        u32 val = (light & mask) >> offset;

        // propagate in reverse of enum Direction order so DOWN is first
        // this will improve sunlight propagation speed
        for (u8 i = 0; i < 6; i++) {
            Vec3<i64> newPos = pos + directions[i];

            RGBIS4* newLight;
            if (!VoxelWorlds::getVoxel(voxelLightWorld, newPos.x, newPos.y, newPos.z, &newLight)) {
                continue;
            }

            EmbeddedVoxel* voxel;
            if (!VoxelWorlds::getVoxel(voxelWorld, newPos.x, newPos.y, newPos.z, &voxel)) {
                continue;
            }

            u32 newVal = (*newLight & mask) >> offset;

            BlockVoxelData newBlock = BLOCK_VOXEL_DATA[voxel->type];

            bool sunlight_down = type == SUN_LIGHT && i == 0;

            // u8 actualDir;
            // if (i == 0) actualDir = 3;
            // else if (i == 1) actualDir = 2;
            // else if (i == 2) actualDir = 0;
            // else if (i == 3) actualDir = 1;
            // else if (i == 4) actualDir = 4;
            // else actualDir = 5;

            if ((newVal != 0 || newBlock.transparent/* || blockMesh.culls(actualDir)*/) && ((sunlight_down && newVal < val) || (newVal + 1 < val))) {
                // sunlight does not get dimmer as it propagates down
                i32 delta = sunlight_down ? 0 : -1;
                
                *newLight = (*newLight & ~mask) | ((((light & mask) >> offset) + delta) << offset);
                // *newLight = (*newLight & ~mask) | (std::max(i32((light & mask) >> offset) + delta, i32(0)) << offset);

                lightQueue.push({newPos, 0});
            }
        }
    }
}

void ChunkLight::remove_propagate(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, AllLightQueue& lightQueue, AllLightQueue& propQueue, u32 mask, u32 offset, LightPropagationType type) {
    (void)voxelWorld;
    while (!lightQueue.empty()) {
        auto [pos, value] = lightQueue.front();
        lightQueue.pop();

        for (u8 i = 0; i < 6; i++) {
            Vec3<i64> newPos = pos + directions[i];

            RGBIS4* newLight;
            if (!VoxelWorlds::getVoxel(voxelLightWorld, newPos.x, newPos.y, newPos.z, &newLight)) {
                continue;
            }
            u32 newValue = (*newLight & mask) >> offset;

            bool sunlight_down = type == SUN_LIGHT && i == 0;

            if ((*newLight & mask) != 0 && (newValue < value || sunlight_down)) {
                *newLight = *newLight & ~mask;
                lightQueue.push({newPos, newValue});
            } else if (newValue >= value) {
                propQueue.push({newPos, 0});
            }
        }
    }
}

void ChunkLight::add_channel(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos, u8 value, u32 mask, u32 offset, LightPropagationType type) {
    AllLightQueue lightQueue;

    RGBIS4* light;
    if (!VoxelWorlds::getVoxel(voxelLightWorld, pos.x, pos.y, pos.z, &light)) {
        return;
    }

    // set color of source block
    VoxelWorlds::placeVoxel(voxelLightWorld, pos.x, pos.y, pos.z, (*light & ~mask) | (((u32)value) << offset));

    lightQueue.push({pos, 0});
    add_propagate(voxelWorld, voxelLightWorld, lightQueue, mask, offset, type);
}

void ChunkLight::remove_channel(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos, u32 mask, u32 offset, LightPropagationType type) {
    AllLightQueue lightQueue;
    AllLightQueue propQueue;

    RGBIS4* light;
    if (!VoxelWorlds::getVoxel(voxelLightWorld, pos.x, pos.y, pos.z, &light)) {
        return;
    }
    RGBIS4 oldLight = *light;
    *light = (oldLight & ~mask);

    lightQueue.push({pos, (oldLight & mask) >> offset});
    remove_propagate(voxelWorld, voxelLightWorld, lightQueue, propQueue, mask, offset, type);
    add_propagate(voxelWorld, voxelLightWorld, propQueue, mask, offset, type);
}

void ChunkLight::add_light(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos, RGBIS4 light) {
    for (u8 i = 0; i < 4; i++) {
        u32 offset = i * 4;
        u32 mask = 0xF << offset;
        add_channel(voxelWorld, voxelLightWorld, pos, (light & mask) >> offset, mask, offset, DEFAULT_LIGHT);
    }
}

void ChunkLight::remove_light(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos) {
    for (u8 i = 0; i < 5; i++) {
        bool sunlight = i == 4;

        u32 offset = i * 4;
        u32 mask = 0xF << offset;
        remove_channel(voxelWorld, voxelLightWorld, pos, mask, offset, sunlight ? SUN_LIGHT : DEFAULT_LIGHT);
    }
}

void ChunkLight::update_light(const VoxelBlockWorld& voxelWorld, const VoxelHeightWorld& heightWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos) {
    AllLightQueue lightQueue;

    // 0..4 for each channel
    // 0 = R
    // 1 = G
    // 2 = B
    // 3 = I
    // 4 = SUN
    for (u8 i = 0; i < 5; i++) {
        lightQueue = AllLightQueue();

        u8 offset = i * 4;
        u32 mask = 0xF << offset;

        bool sunlight = i == 4;
        
        for (u8 d = 0; d < 6; d++) {
            Vec3<i64> newPos = pos + directions[d];

            EmbeddedVoxel* voxel;
            if (!VoxelWorlds::getVoxel(voxelWorld, newPos.x, newPos.y, newPos.z, &voxel)) {
                continue;
            }

            lightQueue.push({newPos, 0});
        }

        if (sunlight && pos.y > heightWorld.heightAt(pos.x, pos.z)) {
            RGBIS4* light;
            if (VoxelWorlds::getVoxel(voxelLightWorld, pos.x, pos.y, pos.z, &light)) {
                *light = Colors::setSunlight(*light, Colors::COLOR4_MAX);
                lightQueue.push({pos, 0});
            }
        }

        add_propagate(voxelWorld, voxelLightWorld, lightQueue, mask, offset, sunlight ? SUN_LIGHT : DEFAULT_LIGHT);
    }
}

void ChunkLight::apply_light(const VoxelBlockWorld& voxelWorld, const VoxelHeightWorld& heightWorld, VoxelLightWorld& lightWorld, const VoxelBlockChunk& chunk, const VoxelHeightChunk& heightChunk, VoxelLightChunk& voxelLightChunk) {
    AllLightQueue sunlightQueue;
    AllLightQueue lightQueue;
    for (u8 x = 0; x < CS; x++) {
        for (u8 z = 0; z < CS; z++) {
            i64 height = heightChunk.heightAt(x, z);

            // dont change to u8
            // keep signed
            // always signed
            // not u8, u16, u32 or u64
            // because on the last iteration (when y is 0)
            // it subtracts one and checks if we should continue the loop
            // which causes it to underflow into highest possible value
            // (e.g. with u8: 0 - 1 = 255)
            for (i8 y = CS - 1; y >= 0; y--) {
                Vec3<i8> pos_chunk = Vec3<i8>(x, y, z);
                Vec3<i64> pos_world = chunk.pos * CS + pos_chunk;
                if (pos_world.y > height) {
                    voxelLightChunk.voxels[get_zxy_index(x, y, z)] = Colors::setSunlight(voxelLightChunk.voxels[get_zxy_index(x, y, z)], Colors::COLOR4_MAX);

                    // try to propagate: right left back front
                    for (u8 d = 2; d < 6; d++) {
                        Vec3<i8> dir_pos_chunk = pos_chunk + directions[d];
                        Vec3<i64> dir_pos_world = pos_world + directions[d];

                        // printf("%i\n", VoxelWorlds::isInChunkBounds(dir_pos_chunk));

                        bool hasY = false;
                        i64 foundY;
                        if (VoxelWorlds::isInChunkBounds(dir_pos_chunk)) {
                            hasY = true;
                            foundY = heightChunk.heightAt(dir_pos_chunk.x, dir_pos_chunk.z);
                        } else {
                            Vec2<i64> heightChunkPos = Vec2<i64>(dir_pos_world.x, dir_pos_world.z);
                            if (heightWorld.isChunkPosInWorld(heightChunkPos)) {
                                hasY = true;
                                foundY = heightWorld.heightAt(dir_pos_world.x, dir_pos_world.z);
                            }
                        }

                        if (hasY && pos_world.y < foundY) {
                            sunlightQueue.push({pos_world, 0}); // TODO: dir_pos_world?
                        }
                    }
                }

                EmbeddedVoxel block = chunk.voxels[get_zxy_index(pos_chunk.x, pos_chunk.y, pos_chunk.z)];
                BlockVoxelData blockData = BLOCK_VOXEL_DATA[block.type];
                if (blockData.can_emit_light) {
                    RGBIS4 blockLight = blockData.get_light();
                    voxelLightChunk.voxels[get_zxy_index(pos_chunk.x, pos_chunk.y, pos_chunk.z)] = blockLight;
                    lightQueue.push({pos_world, blockLight});
                }
            }
        }
    }

    add_propagate(voxelWorld, lightWorld, sunlightQueue, Colors::COLOR4_MAX << 16, 16, SUN_LIGHT);



    
    // do the silly
    AllLightQueue queue;
    for (i32 i = 0; i < 4; i++) {
        u32 offset = i * 4;
        u32 mask = Colors::COLOR4_MAX << offset;

        queue = AllLightQueue();

        for (i32 j = 0; j < lightQueue.size(); j++) {
            auto [pos, light] = getElementAt(lightQueue, j);
            if (light & mask) {
                queue.push({pos, 0});
            }
        }

        add_propagate(voxelWorld, lightWorld, queue, mask, offset, DEFAULT_LIGHT);
    }
}