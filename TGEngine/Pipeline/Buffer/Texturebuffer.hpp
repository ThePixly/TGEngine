#pragma once

#include "Memory.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "..\..\..\stb\stb_image.h"

struct Texture {
	INPUT 
	char* texture_path;
	
	OUTPUT 
	int width;
	int height;
	int channel;
	stbi_uc* image_data;
	VkImage image;
	VkDeviceMemory d_memory;
	VkMemoryRequirements requierments;
	void* memory;

	DEPRECATED
	VkExtent2D texture_extend;
	uint32_t depth;
	uint32_t color_type;
	uint32_t compression_methode;
	uint32_t filter_methode;
	uint32_t interlace_methode;
	END_DEPRECATED

};

extern std::vector<Texture*> texture_buffers;

SINCE(0, 0, 2)
void createTexture(Texture* tex);

SINCE(0, 0, 2)
void initAllTextures();

SINCE(0, 0, 2)
void destroyTexture(Texture* tex);

SINCE(0, 0, 2)
void destroyAllTextures();