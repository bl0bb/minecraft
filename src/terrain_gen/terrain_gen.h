#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H

#include <vector>
#include <algorithm>
#include "../FastNoise/FastNoise.h"
#include "../blocks/blocks.h"




const f32 continentalnessAmplitudes[] = {
    1.0f,
    0.5f,
    0.5f,
    0.5f,
    1.0f,
    1.0f,
    1.0f,
};

const f32 pvAmplitudes[] = {
    1.0f,
    1.0f,
    0.0f,
    1.0f,
    1.0f,
};




enum class TerrainContinentalness : u8 {
    MUSHROOM_FIELDS,
    DEEP_OCEAN,
    OCEAN,
    COAST,
    NEAR_INLAND,
    MID_INLAND,
    FAR_INLAND,
};


enum class TerrainPV : u8 {
    VALLEYS,
    LOW,
    MID,
    HIGH,
    PEAKS,
};



enum class BiomeType : u8 {
    // none
    NONE,
    // offshore biomes
    OCEAN,
    DEEP_OCEAN,
    WARM_OCEAN,
    LUKEWARM_OCEAN,
    DEEP_LUKEWARM_OCEAN,
    COLD_OCEAN,
    DEEP_COLD_OCEAN,
    FROZEN_OCEAN,
    DEEP_FROZEN_OCEAN,
    MUSHROOM_FIELDS,
    // highland biomes
    JAGGED_PEAKS,
    FROZEN_PEAKS,
    STONY_PEAKS,
    MEADOW,
    CHERRY_GROVE,
    GROVE,
    SNOWY_SLOPES,
    WINDSWEPT_HILLS,
    WINDSWEPT_GRAVELLY_HILLS,
    WINDSWEPT_FOREST,
    // woodland biomes
    FOREST,
    FLOWER_FOREST,
    TAIGA,
    OLD_GROWTH_PINE_TAIGA,
    OLD_GROWTH_SPRUCE_TAIGA,
    SNOWY_TAIGA,
    BIRCH_FOREST,
    OLD_GROWTH_BIRCH_FOREST,
    DARK_FOREST,
    PALE_GARDEN,
    JUNGLE,
    SPARSE_JUNGLE,
    BAMBOO_JUNGLE,
    // wetland biomes
    RIVER,
    FROZEN_RIVER,
    SWAMP,
    MANGROVE_SWAMP,
    BEACH,
    SNOWY_BEACH,
    STONY_SHORE,
    // flatland biomes
    PLAINS,
    SUNFLOWER_PLAINS,
    SNOWY_PLAINS,
    ICE_SPIKES,
    // arid land biomes
    DESERT,
    SAVANNA,
    SAVANNA_PLATEAU,
    WINDSWEPT_SAVANNA,
    BADLANDS,
    WOODED_BADLANDS,
    ERODED_BADLANDS,
};







class TerrainGen {
public:
    // Create separate noise instances for each terrain feature

    // Terrain shaping
    FastNoise temperatureNoise;
    FastNoise humidityNoise;
    FastNoise continentalnessNoise;
    FastNoise erosionNoise;
    FastNoise weirdnessNoise;

    TerrainGen() {
        
    }

    ~TerrainGen() {
        
    }




    // get values from noise
    TerrainContinentalness getContinent(f32 cont) {
        if      (cont >= -1.2f   && cont <= -1.05f ) return TerrainContinentalness::MUSHROOM_FIELDS;
        else if (cont >= -1.05f  && cont <= -0.455f) return TerrainContinentalness::DEEP_OCEAN;
        else if (cont >= -0.455f && cont <= -0.19f ) return TerrainContinentalness::OCEAN;
        else if (cont >= -0.19f  && cont <= -0.11f ) return TerrainContinentalness::COAST;
        else if (cont >= -0.11f  && cont <=  0.03f ) return TerrainContinentalness::NEAR_INLAND;
        else if (cont >=  0.03f  && cont <=  0.3f  ) return TerrainContinentalness::MID_INLAND;
        else if (cont >=  0.3f   && cont <=  1.0f  ) return TerrainContinentalness::FAR_INLAND;
        printf("buh 1\n");
        return TerrainContinentalness::OCEAN;
    }

    TerrainPV getPV(f32 pv) {
        if      (pv >= -1.0f  && pv <= -0.85f) return TerrainPV::VALLEYS;
        else if (pv >= -0.85f && pv <= -0.2f ) return TerrainPV::LOW;
        else if (pv >= -0.2f  && pv <=  0.2f ) return TerrainPV::MID;
        else if (pv >=  0.2f  && pv <=  0.7f ) return TerrainPV::HIGH;
        else if (pv >=  0.7f  && pv <=  1.0f ) return TerrainPV::PEAKS;
        printf("buh 2\n");
        return TerrainPV::PEAKS;
    }




    // biome categories

    // beach biomes
    BiomeType getBeachBiomes(u8 temp) {
        if (temp == 0) {
            return BiomeType::SNOWY_BEACH;
        } else if (temp < 4) {
            return BiomeType::BEACH;
        } else {
            return BiomeType::DESERT;
        }
    }

    // badland biomes
    BiomeType getBadlandBiomes(u8 hum, u8 weird) {
        if (hum < 2) {
            if (weird < 0) {
                return BiomeType::BADLANDS;
            } else {
                return BiomeType::ERODED_BADLANDS;
            }
        } else if (hum == 2) {
            return BiomeType::BADLANDS;
        } else {
            return BiomeType::WOODED_BADLANDS;
        }
    }

    // middle biomes
    BiomeType getMiddleBiomes(u8 hum, u8 temp, u8 weird) {
        if (temp == 0) {
            if (hum == 0) {
                if (weird < 0) {
                    return BiomeType::SNOWY_PLAINS;
                } else {
                    return BiomeType::ICE_SPIKES;
                }
            } else if (hum == 1) {
                return BiomeType::SNOWY_PLAINS;
            } else if (hum == 2) {
                if (weird < 0) {
                    return BiomeType::SNOWY_PLAINS;
                } else {
                    return BiomeType::SNOWY_TAIGA;
                }
            } else if (hum == 3) {
                return BiomeType::SNOWY_TAIGA;
            } else if (hum == 4) {
                return BiomeType::TAIGA;
            }
        } else if (temp == 1) {
            if (hum < 2) {
                return BiomeType::PLAINS;
            } else if (hum == 2) {
                return BiomeType::FOREST;
            } else if (hum == 3) {
                return BiomeType::TAIGA;
            } else if (hum == 4) {
                if (weird < 0) {
                    return BiomeType::OLD_GROWTH_SPRUCE_TAIGA;
                } else {
                    return BiomeType::OLD_GROWTH_PINE_TAIGA;
                }
            }
        } else if (temp == 2) {
            if (hum == 0) {
                if (weird < 0) {
                    return BiomeType::FLOWER_FOREST;
                } else {
                    return BiomeType::SUNFLOWER_PLAINS;
                }
            } else if (hum == 1) {
                return BiomeType::PLAINS;
            } else if (hum == 2) {
                return BiomeType::FOREST;
            } else if (hum == 3) {
                if (weird < 0) {
                    return BiomeType::BIRCH_FOREST;
                } else {
                    return BiomeType::OLD_GROWTH_BIRCH_FOREST;
                }
            } else if (hum == 4) {
                return BiomeType::DARK_FOREST;
            }
        } else if (temp == 3) {
            if (hum < 2) {
                return BiomeType::SAVANNA;
            } else if (hum == 2) {
                if (weird < 0) {
                    return BiomeType::FOREST;
                } else {
                    return BiomeType::PLAINS;
                }
            } else if (hum == 3) {
                if (weird < 0) {
                    return BiomeType::JUNGLE;
                } else {
                    return BiomeType::SPARSE_JUNGLE;
                }
            } else if (hum == 4) {
                if (weird < 0) {
                    return BiomeType::JUNGLE;
                } else {
                    return BiomeType::BAMBOO_JUNGLE;
                }
            }
        } else if (temp == 4) {
            return BiomeType::DESERT;
        }

        printf("buh 3\n");
        return BiomeType::DESERT;
    }

    // plateau biomes
    BiomeType getPlateauBiomes(u8 hum, u8 temp, u8 weird) {
        if (temp == 0) {
            if (hum == 0) {
                if (weird < 0) {
                    return BiomeType::SNOWY_PLAINS;
                } else {
                    return BiomeType::ICE_SPIKES;
                }
            } else if (hum < 3) {
                return BiomeType::SNOWY_PLAINS;
            } else {
                return BiomeType::SNOWY_TAIGA;
            }
        } else if (temp == 1) {
            if (hum == 0) {
                if (weird < 0) {
                    return BiomeType::MEADOW;
                } else {
                    return BiomeType::CHERRY_GROVE;
                }
            } else if (hum == 1) {
                return BiomeType::MEADOW;
            } else if (hum == 2) {
                if (weird < 0) {
                    return BiomeType::FOREST;
                } else {
                    return BiomeType::MEADOW;
                }
            } else if (hum == 3) {
                if (weird < 0) {
                    return BiomeType::TAIGA;
                } else {
                    return BiomeType::MEADOW;
                }
            } else {
                if (weird < 0) {
                    return BiomeType::OLD_GROWTH_SPRUCE_TAIGA;
                } else {
                    return BiomeType::OLD_GROWTH_PINE_TAIGA;
                }
            }
        } else if (temp == 2) {
            if (hum < 2) {
                if (weird < 0) {
                    return BiomeType::MEADOW;
                } else {
                    return BiomeType::CHERRY_GROVE;
                }
            } else if (hum == 2) {
                if (weird < 0) {
                    return BiomeType::MEADOW;
                } else {
                    return BiomeType::FOREST;
                }
            } else if (hum == 3) {
                if (weird < 0) {
                    return BiomeType::MEADOW;
                } else {
                    return BiomeType::BIRCH_FOREST;
                }
            } else {
                return BiomeType::PALE_GARDEN;
            }
        } else if (temp == 3) {
            if (hum < 2) {
                return BiomeType::SAVANNA_PLATEAU;
            } else if (hum < 4) {
                return BiomeType::FOREST;
            } else {
                return BiomeType::JUNGLE;
            }
        } else if (temp == 4) {
            if (hum < 2) {
                if (weird < 0) {
                    return BiomeType::BADLANDS;
                } else {
                    return BiomeType::ERODED_BADLANDS;
                }
            } else if (hum == 2) {
                return BiomeType::BADLANDS;
            } else {
                return BiomeType::ERODED_BADLANDS;
            }
        }

        printf("buh 4\n");
        return BiomeType::DESERT;
    }

    // shattered biomes
    BiomeType getShatteredBiomes(u8 hum, u8 temp, u8 weird) {
        if (temp < 2) {
            if (hum < 2) {
                return BiomeType::WINDSWEPT_GRAVELLY_HILLS;
            } else if (hum == 2) {
                return BiomeType::WINDSWEPT_HILLS;
            } else {
                return BiomeType::WINDSWEPT_FOREST;
            }
        } else if (temp == 2) {
            if (hum < 3) {
                return BiomeType::WINDSWEPT_HILLS;
            } else {
                return BiomeType::WINDSWEPT_FOREST;
            }
        } else if (temp == 3) {
            if (hum < 2) {
                return BiomeType::SAVANNA;
            } else if (hum == 2) {
                if (weird < 0) {
                    return BiomeType::FOREST;
                } else {
                    return BiomeType::PLAINS;
                }
            } else if (hum == 3) {
                if (weird < 0) {
                    return BiomeType::JUNGLE;
                } else {
                    return BiomeType::SPARSE_JUNGLE;
                }
            } else {
                if (weird < 0) {
                    return BiomeType::JUNGLE;
                } else {
                    return BiomeType::BAMBOO_JUNGLE;
                }
            }
        } else {
            return BiomeType::DESERT;
        }
    }



    // biome selection
    BiomeType getNonInlandBiome(TerrainContinentalness cont, u8 temp) {
        if (cont == TerrainContinentalness::MUSHROOM_FIELDS) {
            return BiomeType::MUSHROOM_FIELDS;
        }
        if (temp == 0) {
            if (cont == TerrainContinentalness::OCEAN) {
                return BiomeType::FROZEN_OCEAN;
            } else if (cont == TerrainContinentalness::DEEP_OCEAN) {
                return BiomeType::DEEP_FROZEN_OCEAN;
            }
        } else if (temp == 1) {
            if (cont == TerrainContinentalness::OCEAN) {
                return BiomeType::COLD_OCEAN;
            } else if (cont == TerrainContinentalness::DEEP_OCEAN) {
                return BiomeType::DEEP_COLD_OCEAN;
            }
        } else if (temp == 2) {
            if (cont == TerrainContinentalness::OCEAN) {
                return BiomeType::OCEAN;
            } else if (cont == TerrainContinentalness::DEEP_OCEAN) {
                return BiomeType::DEEP_OCEAN;
            }
        } else if (temp == 3) {
            if (cont == TerrainContinentalness::OCEAN) {
                return BiomeType::LUKEWARM_OCEAN;
            } else if (cont == TerrainContinentalness::DEEP_OCEAN) {
                return BiomeType::DEEP_LUKEWARM_OCEAN;
            }
        } else if (temp == 4) {
            if (cont == TerrainContinentalness::OCEAN) {
                return BiomeType::WARM_OCEAN;
            } else if (cont == TerrainContinentalness::DEEP_OCEAN) {
                return BiomeType::WARM_OCEAN;
            }
        }
        return BiomeType::NONE;
    }

    BiomeType getInlandBiome(TerrainContinentalness cont, TerrainPV pv, u8 erosion, u8 temp, u8 hum, u8 weird) {
        if (pv == TerrainPV::VALLEYS) {
            if (cont == TerrainContinentalness::COAST) {
                if (temp == 0) {
                    return BiomeType::FROZEN_RIVER;
                } else {
                    return BiomeType::RIVER;
                }
            } else if (cont == TerrainContinentalness::NEAR_INLAND) {
                if (erosion < 6) {
                    if (temp == 0) {
                        return BiomeType::FROZEN_RIVER;
                    } else {
                        return BiomeType::RIVER;
                    }
                } else {
                    if (temp == 0) {
                        return BiomeType::FROZEN_RIVER;
                    } else if (temp < 3) {
                        return BiomeType::SWAMP;
                    } else {
                        return BiomeType::MANGROVE_SWAMP;
                    }
                }
            } else if (cont == TerrainContinentalness::MID_INLAND) {
                if (erosion < 2) {
                    if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion < 6) {
                    if (temp == 0) {
                        return BiomeType::FROZEN_RIVER;
                    } else {
                        return BiomeType::RIVER;
                    }
                } else {
                    if (temp == 0) {
                        return BiomeType::FROZEN_RIVER;
                    } else if (temp < 3) {
                        return BiomeType::SWAMP;
                    } else {
                        return BiomeType::MANGROVE_SWAMP;
                    }
                }
            } else if (cont == TerrainContinentalness::FAR_INLAND) {
                if (erosion < 2) {
                    if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion < 6) {
                    if (temp == 0) {
                        return BiomeType::FROZEN_RIVER;
                    } else {
                        return BiomeType::RIVER;
                    }
                } else {
                    if (temp == 0) {
                        return BiomeType::FROZEN_RIVER;
                    } else if (temp < 3) {
                        return BiomeType::SWAMP;
                    } else {
                        return BiomeType::MANGROVE_SWAMP;
                    }
                }
            }
        } else if (pv == TerrainPV::LOW) {
            if (cont == TerrainContinentalness::COAST) {
                if (erosion < 3) {
                    return BiomeType::STONY_SHORE;
                } else if (erosion < 5) {
                    return getBeachBiomes(temp);
                } else if (erosion == 5) {
                    if (weird < 0) {
                        return getBeachBiomes(temp);
                    } else {
                        if (temp < 2 || hum == 4) {
                            return getMiddleBiomes(hum, temp, weird);
                        } else {
                            return BiomeType::WINDSWEPT_SAVANNA;
                        }
                    }
                } else {
                    return getBeachBiomes(temp);
                }
            } else if (cont == TerrainContinentalness::NEAR_INLAND) {
                if (erosion < 2) {
                    if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion < 5) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    if (weird < 0 || temp < 2 || hum == 4) {
                        return getShatteredBiomes(hum, temp, weird);
                    } else if (weird > 0 && temp > 1 && hum < 4) {
                        return BiomeType::WINDSWEPT_SAVANNA;
                    } else {
                        printf("wwww 5\n");
                        return BiomeType::WINDSWEPT_SAVANNA;
                    }
                } else {
                    if (temp == 0) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else if (temp < 3) {
                        return BiomeType::SWAMP;
                    } else {
                        return BiomeType::MANGROVE_SWAMP;
                    }
                }
            } else if (cont == TerrainContinentalness::MID_INLAND || cont == TerrainContinentalness::FAR_INLAND) {
                if (erosion < 2) {
                    if (temp == 0) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion < 4) {
                    if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion < 6) {
                    return getMiddleBiomes(hum, temp, weird);
                } else {
                    if (temp == 0) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else if (temp < 3) {
                        return BiomeType::SWAMP;
                    } else {
                        return BiomeType::MANGROVE_SWAMP;
                    }
                }
            }
        } else if (pv == TerrainPV::MID) {
            if (cont == TerrainContinentalness::COAST) {
                if (erosion < 3) {
                    return BiomeType::STONY_SHORE;
                } else if (erosion == 3) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 4) {
                    if (weird < 0) {
                        return getBeachBiomes(temp);
                    } else {
                        return getMiddleBiomes(hum, temp, weird);
                    }
                } else if (erosion == 5) {
                    if (weird < 0) {
                        return getBeachBiomes(temp);
                    } else {
                        if (temp < 2 || hum == 4) {
                            return getMiddleBiomes(hum, temp, weird);
                        } else {
                            return BiomeType::WINDSWEPT_SAVANNA;
                        }
                    }
                } else {
                    if (weird < 0) {
                        return getBeachBiomes(temp);
                    } else {
                        return getMiddleBiomes(hum, temp, weird);
                    }
                }
            } else if (cont == TerrainContinentalness::NEAR_INLAND) {
                if (erosion == 0) {
                    if (temp < 3) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else {
                        return getPlateauBiomes(hum, temp, weird);
                    }
                } else if (erosion == 1) {
                    if (temp == 0) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion < 5) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    if (weird < 0 || temp < 2 || hum == 4) {
                        return getShatteredBiomes(hum, temp, weird);
                    } else if (weird > 0 && temp > 1 && hum < 4) {
                        return BiomeType::WINDSWEPT_SAVANNA;
                    } else {
                        printf("wwww 4\n");
                        return BiomeType::WINDSWEPT_SAVANNA;
                    }
                } else {
                    if (temp == 0) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else if (temp < 3) {
                        return BiomeType::SWAMP;
                    } else {
                        return BiomeType::MANGROVE_SWAMP;
                    }
                }
            } else if (cont == TerrainContinentalness::MID_INLAND) {
                if (erosion == 0) {
                    if (temp < 3) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else {
                        return getPlateauBiomes(hum, temp, weird);
                    }
                } else if (erosion == 1) {
                    if (temp == 0) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion < 4) {
                    if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 4) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    return getShatteredBiomes(hum, temp, weird);
                } else {
                    if (temp == 0) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else if (temp < 3) {
                        return BiomeType::SWAMP;
                    } else {
                        return BiomeType::MANGROVE_SWAMP;
                    }
                }
            } else if (cont == TerrainContinentalness::FAR_INLAND) {
                if (erosion == 0) {
                    if (temp < 3) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else {
                        return getPlateauBiomes(hum, temp, weird);
                    }
                } else if (erosion == 1) {
                    if (temp == 0) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else {
                        return getPlateauBiomes(hum, temp, weird);
                    }
                } else if (erosion == 2) {
                    return getPlateauBiomes(hum, temp, weird);
                } else if (erosion == 3) {
                    if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 4) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    return getShatteredBiomes(hum, temp, weird);
                } else {
                    if (temp == 0) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else if (temp < 3) {
                        return BiomeType::SWAMP;
                    } else {
                        return BiomeType::MANGROVE_SWAMP;
                    }
                }
            }
        } else if (pv == TerrainPV::HIGH) {
            if (cont == TerrainContinentalness::COAST) {
                if (erosion < 5) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    if (weird < 0 || temp < 2 || hum == 4) {
                        return getShatteredBiomes(hum, temp, weird);
                    } else if (weird > 0 && temp > 1 && hum < 4) {
                        return BiomeType::WINDSWEPT_SAVANNA;
                    } else {
                        printf("wwww 3\n");
                        return BiomeType::WINDSWEPT_SAVANNA;
                    }
                } else {
                    return getMiddleBiomes(hum, temp, weird);
                }
            } else if (cont == TerrainContinentalness::NEAR_INLAND) {
                if (erosion == 0) {
                    if (temp < 3) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else {
                        return getPlateauBiomes(hum, temp, weird);
                    }
                } else if (erosion == 1) {
                    if (temp == 0) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 5) {
                    if (weird < 0 || temp < 2 || hum == 4) {
                        return getShatteredBiomes(hum, temp, weird);
                    } else if (weird > 0 && temp > 1 && hum < 4) {
                        return BiomeType::WINDSWEPT_SAVANNA;
                    } else {
                        printf("wwww 2\n");
                        return BiomeType::WINDSWEPT_SAVANNA;
                    }
                } else {
                    return getMiddleBiomes(hum, temp, weird);
                }
            } else if (cont == TerrainContinentalness::MID_INLAND) {
                if (erosion == 0) {
                    if (temp < 3) {
                        if (weird < 0) {
                            return BiomeType::JAGGED_PEAKS;
                        } else {
                            return BiomeType::FROZEN_PEAKS;
                        }
                    } else if (temp == 3) {
                        return BiomeType::STONY_PEAKS;
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 1) {
                    if (temp < 3) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else {
                        return getPlateauBiomes(hum, temp, weird);
                    }
                } else if (erosion == 2) {
                    return getPlateauBiomes(hum, temp, weird);
                } else if (erosion == 3) {
                    if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 4) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    return getShatteredBiomes(hum, temp, weird);
                } else {
                    return getMiddleBiomes(hum, temp, weird);
                }
            } else if (cont == TerrainContinentalness::FAR_INLAND) {
                if (erosion == 0) {
                    if (temp < 3) {
                        if (weird < 0) {
                            return BiomeType::JAGGED_PEAKS;
                        } else {
                            return BiomeType::FROZEN_PEAKS;
                        }
                    } else if (temp == 3) {
                        return BiomeType::STONY_PEAKS;
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 1) {
                    if (temp < 3) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else {
                        return getPlateauBiomes(hum, temp, weird);
                    }
                } else if (erosion < 4) {
                    return getPlateauBiomes(hum, temp, weird);
                } else if (erosion == 4) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    return getShatteredBiomes(hum, temp, weird);
                } else {
                    return getMiddleBiomes(hum, temp, weird);
                }
            }
        } else if (pv == TerrainPV::PEAKS) {
            if (cont == TerrainContinentalness::COAST || cont == TerrainContinentalness::NEAR_INLAND) {
                if (erosion == 0) {
                    if (temp < 3) {
                        if (weird < 0) {
                            return BiomeType::JAGGED_PEAKS;
                        } else {
                            return BiomeType::FROZEN_PEAKS;
                        }
                    } else if (temp == 3) {
                        return BiomeType::STONY_PEAKS;
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 1) {
                    if (temp == 0) {
                        if (hum < 2) {
                            return BiomeType::SNOWY_SLOPES;
                        } else {
                            return BiomeType::GROVE;
                        }
                    } else if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion < 5) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    if (weird < 0 || temp < 2 || hum == 4) {
                        return getShatteredBiomes(hum, temp, weird);
                    } else if (weird > 0 && temp > 1 && hum < 4) {
                        return BiomeType::WINDSWEPT_SAVANNA;
                    } else {
                        printf("wwwww 1\n");
                        return BiomeType::WINDSWEPT_SAVANNA;
                    }
                } else {
                    return getMiddleBiomes(hum, temp, weird);
                }
            } else if (cont == TerrainContinentalness::MID_INLAND) {
                if (erosion < 2) {
                    if (temp < 3) {
                        if (weird < 0) {
                            return BiomeType::JAGGED_PEAKS;
                        } else {
                            return BiomeType::FROZEN_PEAKS;
                        }
                    } else if (temp == 3) {
                        return BiomeType::STONY_PEAKS;
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 2) {
                    return getPlateauBiomes(hum, temp, weird);
                } else if (erosion == 3) {
                    if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 4) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    return getShatteredBiomes(hum, temp, weird);
                } else {
                    return getMiddleBiomes(hum, temp, weird);
                }
            } else if (cont == TerrainContinentalness::FAR_INLAND) {
                if (erosion < 2) {
                    if (temp < 3) {
                        if (weird < 0) {
                            return BiomeType::JAGGED_PEAKS;
                        } else {
                            return BiomeType::FROZEN_PEAKS;
                        }
                    } else if (temp == 3) {
                        return BiomeType::STONY_PEAKS;
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 2) {
                    return getPlateauBiomes(hum, temp, weird);
                } else if (erosion == 3) {
                    if (temp < 4) {
                        return getMiddleBiomes(hum, temp, weird);
                    } else {
                        return getBadlandBiomes(hum, weird);
                    }
                } else if (erosion == 4) {
                    return getMiddleBiomes(hum, temp, weird);
                } else if (erosion == 5) {
                    return getShatteredBiomes(hum, temp, weird);
                } else {
                    return getMiddleBiomes(hum, temp, weird);
                }
            }
        }

        printf("NUHUH\n");
        return BiomeType::PLAINS;
    }



    BiomeType getBiome(TerrainContinentalness cont, TerrainPV pv, u8 erosion, u8 temp, u8 hum, u8 weird) {
        BiomeType nonInlandBiome = getNonInlandBiome(cont, temp);
        if (nonInlandBiome != BiomeType::NONE) {
            return nonInlandBiome;
        }
        return getInlandBiome(cont, pv, erosion, temp, hum, weird);
    }



    f32 calculatePV(f32 x) {
        return 1.0f - std::abs(3.0f * std::abs(x) - 2.0f);
    }



    void updateNoise() {
        // Terrain shaping

        // Continentalness: Broad, low-frequency noise to represent landmasses vs ocean
        continentalnessNoise.SetFrequency(0.001f);
        continentalnessNoise.SetNoiseType(FastNoise::Perlin);
        continentalnessNoise.SetFractalType(FastNoise::FBM);
        continentalnessNoise.SetFractalOctaves(4);

        // Erosion: Medium-frequency, detail-focused
        erosionNoise.SetFrequency(0.01f);
        erosionNoise.SetNoiseType(FastNoise::Perlin);
        erosionNoise.SetFractalType(FastNoise::FBM);
        erosionNoise.SetFractalOctaves(4);

        // Weirdness
        weirdnessNoise.SetFrequency(0.02f);
        weirdnessNoise.SetNoiseType(FastNoise::Perlin);
        weirdnessNoise.SetFractalType(FastNoise::FBM);
        weirdnessNoise.SetFractalOctaves(4);

        // Temperature: Used for biome classification
        temperatureNoise.SetNoiseType(FastNoise::Perlin);
        temperatureNoise.SetFrequency(0.02f);
        temperatureNoise.SetFractalType(FastNoise::FBM);
        temperatureNoise.SetFractalOctaves(4);

        // Humidity: Also for biome classification
        humidityNoise.SetNoiseType(FastNoise::Simplex);
        humidityNoise.SetFrequency(0.03f);
        humidityNoise.SetFractalType(FastNoise::FBM);
        humidityNoise.SetFractalOctaves(4);
    }

    void setSeed(int seed) {
        // terrain shaping
        continentalnessNoise.SetSeed(seed);
        erosionNoise.SetSeed(seed);
        weirdnessNoise.SetSeed(seed);
        temperatureNoise.SetSeed(seed);
        humidityNoise.SetSeed(seed);
    }

    int* generateHeightMap(i64 chunk_x, i64 chunk_y, i64 chunk_z) {
        int* heightMap = new int[CS_2]{0};
    
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                i64 world_x = (chunk_x * CS) + x;
                i64 world_z = (chunk_z * CS) + z;





                f32 heightValue = 0.0f;

                f32 cont = continentalnessNoise.GetNoise(world_x, world_z);
                f32 eros = erosionNoise.GetNoise(world_x, world_z);
                f32 peaks = calculatePV(weirdnessNoise.GetNoise(world_x, world_z));


                heightValue += continentalnessAmplitudes[u8(getContinent(cont))];
                heightValue += continentalnessAmplitudes[u8(getContinent(cont))];






                heightValue = (heightValue + 1.0f) / 2.0f; // Normalize to 0..1
                heightMap[z + x * CS] = heightValue * 64.0f; // (heightValue * CS * 2) - (chunk_y * CS); // (heightValue * CS) - (chunk_y * CS);
            }
        }

        return heightMap;
    }

    void assignBaseBlocks(EmbeddedVoxel* terrain, i64 chunk_x, i64 chunk_y, i64 chunk_z, const int* heightMap) {
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                int surfaceY = heightMap[z + x * CS];

                if (surfaceY < 0) {
                    continue;
                }

                surfaceY = std::min(surfaceY, CS - 1);

                for (u8 y = 0; y < CS; y++) {
                    BlockType type;

                    i64 world_y = y + (chunk_y * CS);

                    if (world_y > surfaceY) {
                        if (world_y <= CS * 2 * 0.4) {
                            type = BlockTypes::WATER;
                        } else {
                            type = BlockTypes::AIR;
                        }
                    } else if (world_y == surfaceY) {
                        type = BlockTypes::DIRT;  // Placeholder; will be overwritten in surface layer
                    } else if (world_y > surfaceY - 3) {
                        type = BlockTypes::DIRT;
                    } else {
                        type = BlockTypes::STONE;
                    }

                    terrain[get_zxy_index(x, y, z)] = EmbeddedVoxel(type);
                }
            }
        }
    }


    void applySurfaceAndBiomes(EmbeddedVoxel* terrain, i64 chunk_x, i64 chunk_y, i64 chunk_z, const int* heightMap) {
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                i64 world_x = (chunk_x * CS) + x;
                i64 world_z = (chunk_z * CS) + z;

                int surfaceY = heightMap[z + x * CS];

                if (surfaceY < 0) {
                    continue;
                }

                surfaceY = std::min(surfaceY, CS - 1);


                f32 contF = (continentalnessNoise.GetNoise(world_x, world_z) + 1.0f) / 2.0f;
                TerrainContinentalness cont = getContinent(contF);
                f32 erosion = (erosionNoise.GetNoise(world_x, world_z) + 1.0f) / 2.0f;
                f32 weird = (weirdnessNoise.GetNoise(world_x, world_z) + 1.0f) / 2.0f;
                f32 pvF = calculatePV(weird);
                TerrainPV pv = getPV(pvF);
                f32 temp = (temperatureNoise.GetNoise(world_x, world_z) + 1.0f) / 2.0f;
                f32 hum = (humidityNoise.GetNoise(world_x, world_z) + 1.0f) / 2.0f;

                BiomeType biome = getBiome(cont, pv, erosion, temp, hum, weird);

                BlockType type = BlockTypes::GRASS;
                // switch (biome) {
                //     case BiomeType::PLAINS: type = BlockTypes::GRASS; break;
                //     case BiomeType::DESERT: type = BlockTypes::SAND; break;
                //     case BiomeType::FOREST: type = BlockTypes::GRASS; break;
                // }

                terrain[get_zxy_index(x, 0, z)] = EmbeddedVoxel(type);
            }
        }
    }

    void generateFullTerrain(EmbeddedVoxel* terrain, i64 chunk_x, i64 chunk_y, i64 chunk_z) {
        auto heightMap = generateHeightMap(chunk_x, chunk_y, chunk_z);
        assignBaseBlocks(terrain, chunk_x, chunk_y, chunk_z, heightMap);
        applySurfaceAndBiomes(terrain, chunk_x, chunk_y, chunk_z, heightMap);
        delete heightMap;
    }
};

#endif