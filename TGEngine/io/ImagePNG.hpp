#pragma once

#include "../Stdbase.hpp"
#include "../io/Files.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

#define BUFFER_COMPARE(buffer, name) memcmp(buffer.data() + 4, name, 4) == 0

#define COLOR_TYPE_GREYSCALE 0
#define COLOR_TYPE_TRUECOLOR 2
#define COLOR_TYPE_INDEXED 3 
#define COLOR_TYPE_GREYSCALE_ALPHA 4
#define COLOR_TYPE_TRUECOLOR_ALPHA 6

#define READ_CHUNK uint32_t chunksize = int_from_big_endian(buffer.data(), 0);\
buffer.resize(chunksize);\
fread(buffer.data(), sizeof(uint8_t), chunksize, fileptr);

SINCE(0, 0, 2)
DEPRECATED
void loadPNGData(Texture* texture);