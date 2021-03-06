#pragma once

#include "../../Stdbase.hpp"
#include "../../vlib/VulkanBuffer.hpp"
#include "../../pipeline/Device.hpp"
#include "Memory.hpp"

struct StagingBuffer {
	INPUT 
	VkDeviceSize size;
	VkBuffer* destination;

	OUTPUT
	VkBuffer staging_buffer;
	VkDeviceMemory staging_buffer_device_memory;
};

extern std::vector<StagingBuffer*> staging_buffer;

void createStagingBuffer(StagingBuffer* staging_buffer);

void destroyStagingBuffer();