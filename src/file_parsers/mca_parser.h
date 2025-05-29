#ifndef MCA_PARSER_H
#define MCA_PARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#include "nbt_parser.h"

// Utility function to decompress zlib data
std::string decompressZlib(const std::vector<uint8_t>& compressedData) {
    z_stream zs{};
    zs.total_in = zs.avail_in = compressedData.size();
    zs.next_in = const_cast<Bytef*>(compressedData.data());

    inflateInit(&zs);

    int ret;
    char buffer[32768];
    std::string output;

    do {
        zs.avail_out = sizeof(buffer);
        zs.next_out = reinterpret_cast<Bytef*>(buffer);
        ret = inflate(&zs, 0);
        if (output.size() < zs.total_out) {
            output.append(buffer, zs.total_out - output.size());
        }
    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        throw std::runtime_error("Zlib decompression failed");
    }

    return output;
}

void printChunkBlocks(const std::string& decompressedData, int chunkX, int chunkZ) {
    NBTReader nbtReader(decompressedData);
    NBT* root = nbtReader.parse();

    // auto root = nbtParserParse(decompressedData);
    
    auto level = std::get<std::map<std::string, NBT*>>(root->getCompoundTag("Level")->value);

    if (level.find("Sections") == level.end()) return;
    for (const auto& section : std::get<std::vector<NBT*>>(level.at("Sections")->value)) {
        int sectionY = std::get<i32>(section->getCompoundTag("Y")->value);

        // Palette: vector of block names
        const auto& paletteList = std::get<std::vector<NBT*>>(section->getCompoundTag("Palette")->value);
        std::vector<std::string> palette;
        for (const auto& p : paletteList) {
            palette.push_back(std::get<std::string>(p->getCompoundTag("Name")->value));
        }

        const auto& blockStates = std::get<std::vector<NBT*>>(section->getCompoundTag("BlockStates")->value);
        int bitsPerBlock = std::max(4, (int)std::ceil(std::log2(palette.size())));
        uint64_t mask = (1ULL << bitsPerBlock) - 1;

        int index = 0;
        for (int y = 0; y < 16; ++y) {
            for (int z = 0; z < 16; ++z) {
                for (int x = 0; x < 16; ++x) {
                    int bitIndex = index * bitsPerBlock;
                    int longIndex = bitIndex / 64;
                    int startBit = bitIndex % 64;

                    uint64_t data = std::get<i64>(blockStates[longIndex]->value);
                    uint64_t nextData = (longIndex + 1 < blockStates.size()) ? std::get<i64>(blockStates[longIndex + 1]->value) : 0;

                    uint64_t combined = (data >> startBit) | (nextData << (64 - startBit));
                    int paletteIndex = combined & mask;

                    std::string blockName = paletteIndex < palette.size() ? palette[paletteIndex] : "unknown";

                    std::cout << "Block at (" << (chunkX * 16 + x) << ", "
                              << (sectionY * 16 + y) << ", "
                              << (chunkZ * 16 + z) << ") = " << blockName << "\n";
                    index++;
                }
            }
        }
    }
}

void parseMCA(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Unable to open .mca file");

    for (int i = 0; i < 1024; ++i) {
        // Read chunk offset and size
        file.seekg(i * 4, std::ios::beg);
        uint8_t loc[4];
        file.read(reinterpret_cast<char*>(loc), 4);
        int offset = (loc[0] << 16) | (loc[1] << 8) | loc[2];
        int size = loc[3];

        if (offset == 0 || size == 0) continue;

        file.seekg(offset * 4096, std::ios::beg);
        uint8_t lengthBytes[4];
        file.read(reinterpret_cast<char*>(lengthBytes), 4);
        uint32_t length = (lengthBytes[0] << 24) | (lengthBytes[1] << 16) |
                          (lengthBytes[2] << 8) | lengthBytes[3];

        uint8_t compressionType;
        file.read(reinterpret_cast<char*>(&compressionType), 1);
        if (compressionType != 2) {
            std::cerr << "Unsupported compression type " << (int)compressionType << "\n";
            continue;
        }

        std::vector<uint8_t> compressed(length - 1);
        file.read(reinterpret_cast<char*>(compressed.data()), compressed.size());

        try {
            std::string decompressed = decompressZlib(compressed);
            printChunkBlocks(decompressed, i % 32, i / 32);
        } catch (const std::exception& ex) {
            std::cerr << "Failed to parse chunk " << i << ": " << ex.what() << "\n";
        }
    }
}


#endif