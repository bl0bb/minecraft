#include "nbt.h"

NBT* NBT::getCompoundTag(const std::string& key) const {
    if (tagType != TAG_Compound) return nullptr;
    const auto& compound = std::get<std::map<std::string, NBT*>>(value);
    auto it = compound.find(key);
    if (it != compound.end()) return it->second;
    return nullptr;
}