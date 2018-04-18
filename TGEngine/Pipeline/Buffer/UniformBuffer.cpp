#include "UniformBuffer.hpp"

void createUniformBuffer(UniformBuffer* buffer){
	buffer->descriptor.count = 1;
	buffer->descriptor.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	VkBuffer uniform_buffer;

	VkBufferCreateInfo uniform_buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		nullptr,
		0,
		buffer->size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr
	};

	last_result = vkCreateBuffer(device, &uniform_buffer_create_info, nullptr, &uniform_buffer);
	HANDEL(last_result)

	buffer->index = getMemoryRequirements(uniform_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	buffer->descriptor.buffer = buffer->index;
	addDescriptor(&buffer->descriptor);
}

void fillUniformBuffer(UniformBuffer* buffer, uint8_t* input, uint32_t size) {
	vkDeviceWaitIdle(device);
	mapMemory(buffer->index, &buffer->memory);
	memcpy(buffer->memory, input, size);
	unmapMemory();
	updateDescriptorSet(&buffer->descriptor, size);
}