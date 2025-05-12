#ifndef NBT_H
#define NBT_H

#include "../core/types.h"

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>

enum NBT_Tag : u8 {
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




class NBT;
using NBTValue = std::variant<
    i8,
    i16,
    i32,
    i64,
    f32,
    f64,
    std::string,
    std::vector<i8>, // Byte_Array
    std::vector<NBT*>, // List (of any type)
    std::map<std::string, NBT*> // Compound
>;
class NBT {
public:
    NBT_Tag tagType;
    std::string name;
    NBTValue value;

    NBT(NBT_Tag type, const std::string& name, NBTValue val)
        : tagType(type), name(name), value(std::move(val)) {}

    const NBTValue& getValue() const { return value; }

    NBT* getCompoundTag(const std::string& key) const {
        if (tagType != TAG_Compound) return nullptr;
        const auto& compound = std::get<std::map<std::string, NBT*>>(value);
        auto it = compound.find(key);
        if (it != compound.end()) return it->second;
        return nullptr;
    }
};

#endif