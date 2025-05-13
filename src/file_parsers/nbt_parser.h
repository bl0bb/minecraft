#ifndef NBT_PARSER_H
#define NBT_PARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <zlib.h>

#include "../core/types.h"
#include "../nbt/nbt.h"





// gzip helpers
// gzipped files have 2 magic bytes at the start: 0x1F and 0x8B
bool isGzipped(std::istream &in) {
    // Save the current position of the file pointer
    std::streampos current_pos = in.tellg();

    unsigned char byte1 = 0;
    unsigned char byte2 = 0;

    in.read(reinterpret_cast<char*>(&byte1), 1);
    in.read(reinterpret_cast<char*>(&byte2), 1);

    // Restore the file pointer to its original position
    in.seekg(current_pos);

    return byte1 == 0x1F && byte2 == 0x8B;
}

std::vector<char> decompressGzipFile(std::istream &in) {
    // Read the entire compressed file into a buffer
    std::vector<char> compressedData((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    // Prepare output buffer
    std::vector<char> decompressedData;
    decompressedData.resize(4096); // Initial size

    z_stream strm = {};
    strm.next_in = reinterpret_cast<Bytef*>(compressedData.data());
    strm.avail_in = compressedData.size();

    // 16 + MAX_WBITS enables gzip decoding
    if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK) {
        throw std::runtime_error("inflateInit2 failed");
    }

    int ret;
    do {
        if (strm.total_out >= decompressedData.size()) {
            decompressedData.resize(decompressedData.size() * 2);
        }

        strm.next_out = reinterpret_cast<Bytef*>(decompressedData.data() + strm.total_out);
        strm.avail_out = decompressedData.size() - strm.total_out;

        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_MEM_ERROR || ret == Z_DATA_ERROR || ret == Z_NEED_DICT) {
            inflateEnd(&strm);
            throw std::runtime_error("inflate failed");
        }
    } while (ret != Z_STREAM_END);

    inflateEnd(&strm);

    decompressedData.resize(strm.total_out);  // Trim unused space
    return decompressedData;
}


// helpers
std::vector<char> streamToString(std::istream &in) {
    std::vector<char> result;
    char ch;
    
    // Read the stream one character at a time and add to the vector
    while (in.get(ch)) {
        result.push_back(ch);
    }

    return result;
}




class NBTReader {
public:
    explicit NBTReader(const std::vector<char>& data)
        : buffer(data), pos(0) {}

    NBT* parse() {
        return readTag(true);
    }

private:
    const std::vector<char>& buffer;
    size_t pos;

    uint8_t readByte() {
        checkBounds(1);
        return static_cast<uint8_t>(buffer[pos++]);
    }

    int16_t readShort() {
        checkBounds(2);
        int16_t value = (buffer[pos] << 8) | (buffer[pos + 1] & 0xFF);
        pos += 2;
        return value;
    }

    int32_t readInt() {
        checkBounds(4);
        int32_t value = (buffer[pos] << 24) | ((buffer[pos + 1] & 0xFF) << 16) |
                        ((buffer[pos + 2] & 0xFF) << 8) | (buffer[pos + 3] & 0xFF);
        pos += 4;
        return value;
    }

    int64_t readLong() {
        checkBounds(8);
        int64_t value = 0;
        for (int i = 0; i < 8; ++i) {
            value <<= 8;
            value |= static_cast<uint8_t>(buffer[pos++]);
        }
        return value;
    }

    float readFloat() {
        uint32_t i = readInt();
        float f;
        std::memcpy(&f, &i, sizeof(f));
        return f;
    }

    double readDouble() {
        uint64_t i = readLong();
        double d;
        std::memcpy(&d, &i, sizeof(d));
        return d;
    }

    std::string readString() {
        int16_t len = readShort();
        checkBounds(len);
        std::string str(buffer.begin() + pos, buffer.begin() + pos + len);
        pos += len;
        return str;
    }

    NBT* readTag(bool withName) {
        uint8_t tagType = readByte();
        if (tagType == TAG_End)
            return nullptr;

        std::string name;
        if (withName) name = readString();

        return performTagLogic(tagType, name);
    }

    NBT* performTagLogic(u8 tagType, std::string& name) {
        switch (tagType) {
            case TAG_Byte:
                return new NBT(TAG_Byte, name, static_cast<int8_t>(readByte()));
            case TAG_Short:
                return new NBT(TAG_Short, name, readShort());
            case TAG_Int:
                return new NBT(TAG_Int, name, readInt());
            case TAG_Long:
                return new NBT(TAG_Long, name, readLong());
            case TAG_Float:
                return new NBT(TAG_Float, name, readFloat());
            case TAG_Double:
                return new NBT(TAG_Double, name, readDouble());
            case TAG_String:
                return new NBT(TAG_String, name, readString());
            case TAG_Byte_Array: {
                int32_t len = readInt();
                checkBounds(len);
                std::vector<int8_t> arr(len);
                for (int i = 0; i < len; ++i)
                    arr[i] = static_cast<int8_t>(readByte());
                return new NBT(TAG_Byte_Array, name, arr);
            }
            case TAG_List: {
                uint8_t elemType = readByte();
                int32_t length = readInt();

                // std::cout << "List of type " << static_cast<int>(elemType) << " with " << length << " elements:\n";

                std::vector<NBT*> list;
                for (int i = 0; i < length; i++) {
                    std::string name = "";
                    auto item = performTagLogic(elemType, name);
                    list.push_back(item);
                }
                return new NBT(TAG_List, name, list);
            }
            case TAG_Compound: {
                std::map<std::string, NBT*> compound;
                while (true) {
                    size_t savePos = pos;
                    if (buffer[savePos] == TAG_End) {
                        pos++;
                        break;
                    }
                    auto child = readTag(true);
                    if (child)
                        compound[child->name] = child;
                }
                return new NBT(TAG_Compound, name, compound);
            }
            default:
                throw std::runtime_error("Unsupported tag type: " + std::to_string(tagType));
        }
    }

    void checkBounds(size_t size) {
        if (pos + size > buffer.size()) {
            throw std::out_of_range("Unexpected end of buffer");
        }
    }
};

#endif