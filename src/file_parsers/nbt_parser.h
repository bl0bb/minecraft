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






enum NBT_Tag {
    TAG_End = 0,
    TAG_Byte = 1,
    TAG_Short = 2,
    TAG_Int = 3,
    TAG_Long = 4,
    TAG_Float = 5,
    TAG_Double = 6,
    TAG_Byte_Array = 7,
    TAG_String = 8,
    TAG_List = 9,
    TAG_Compound = 10
};

class NBTReader {
public:
    explicit NBTReader(const std::vector<char>& data)
        : buffer(data), pos(0) {}

    void parse() {
        while (true) {
            bool foundTag = readTag(true);
            if (!foundTag) {
                break;
            }
        }
    }

private:
    const std::vector<char>& buffer;
    size_t pos;

    u8 readByte() {
        checkBounds(1);
        return static_cast<u8>(buffer[pos++]);
    }

    i16 readShort() {
        checkBounds(2);
        i16 value = (buffer[pos] << 8) | (buffer[pos + 1] & 0xFF);
        pos += 2;
        return value;
    }

    i32 readInt() {
        checkBounds(4);
        i32 value = (buffer[pos] << 24) | ((buffer[pos + 1] & 0xFF) << 16) |
                        ((buffer[pos + 2] & 0xFF) << 8) | (buffer[pos + 3] & 0xFF);
        pos += 4;
        return value;
    }

    i64 readLong() {
        checkBounds(8);
        i64 value = 0;
        for (int i = 0; i < 8; ++i) {
            value <<= 8;
            value |= static_cast<u8>(buffer[pos++]);
        }
        return value;
    }

    f32 readFloat() {
        u32 i = readInt();
        f32 f;
        std::memcpy(&f, &i, sizeof(f));
        return f;
    }

    f64 readDouble() {
        u64 i = readLong();
        f64 d;
        std::memcpy(&d, &i, sizeof(d));
        return d;
    }

    std::string readString() {
        i16 len = readShort();
        checkBounds(len);
        std::string str(buffer.begin() + pos, buffer.begin() + pos + len);
        pos += len;
        return str;
    }

    std::vector<char> readByteArray() {
        i32 len = readInt();
        printf("array len: %i\n", len);
        checkBounds(len);
        std::vector<char> byteArray(buffer.begin() + pos, buffer.begin() + pos + len);
        pos += len;
        return byteArray;
    }

    void readList() {
        u8 tagType = readByte();
        i32 size = readInt();
        std::cout << "List of type " << static_cast<int>(tagType) << " with " << size << " elements:\n";

        for (int i = 0; i < size; i++) {
            performTagOperation(tagType);
        }
    }

    bool readTag(bool withName) {
        if (pos >= buffer.size()) {
            printf("END OF FILE\n");
            return false;
        }

        u8 tagType = readByte();
        if (tagType == TAG_End) {
            // this should NEVER happen
            printf("TAG END\n");
            return false;
        }

        std::string name;
        if (withName) {
            name = readString();
            std::cout << "Tag Name: " << name << "\n";
        }

        performTagOperation(tagType);

        return true;
    }

    void performTagOperation(u8 tagType) {
        switch (tagType) {
            case TAG_Byte:
                std::cout << "Byte: " << static_cast<int>(readByte()) << "\n";
                break;
            case TAG_Short:
                std::cout << "Short: " << readShort() << "\n";
                break;
            case TAG_Int:
                std::cout << "Int: " << readInt() << "\n";
                break;
            case TAG_Long:
                std::cout << "Long: " << readLong() << "\n";
                break;
            case TAG_Float:
                std::cout << "Float: " << readFloat() << "\n";
                break;
            case TAG_Double:
                std::cout << "Double: " << readDouble() << "\n";
                break;
            case TAG_String:
                std::cout << "String: " << readString() << "\n";
                break;
            case TAG_Byte_Array:
                std::cout << "Byte Array" << "\n";
                {
                    std::vector<char> byteArray = readByteArray();
                    std::cout << "Byte Array (length " << byteArray.size() << "): ";
                    for (char byte : byteArray) {
                        std::cout << static_cast<int>(byte) << " ";
                    }
                    std::cout << "\n";
                }
                break;
            case TAG_List:
                std::cout << "List" << "\n";
                readList();
                break;
            case TAG_Compound:
                std::cout << "Start Compound\n";
                while (true) {
                    size_t savePos = pos;
                    if (buffer[savePos] == TAG_End) {
                        // one empty byte marking end of compound
                        pos++;
                        std::cout << "End Compound\n";
                        break;
                    }
                    readTag(true);
                }
                break;
            default:
                printf("Unsupported tag type: %i\n", tagType);
                // throw std::runtime_error("Unsupported tag type");
        }
    }

    void checkBounds(size_t size) {
        if (pos + size > buffer.size()) {
            printf("%i %i %i\n", pos, size, buffer.size());
            throw std::out_of_range("Unexpected end of buffer");
        }
    }
};

#endif