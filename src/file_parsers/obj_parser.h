#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "../core/mesh.h"

mesh objparser_parse_obj(const u32 file_count, const char* file);

#endif