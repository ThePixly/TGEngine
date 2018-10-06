#include "CommandBuffer.hpp"


VkCommandPool command_pool;
std::vector<VkCommandBuffer> command_buffers;
VkDeviceSize offsets = 0;
bool started = true;

void createCommandBuffer() {
	command_buffers.resize(image_count);

	if (!command_pool) {
		VkCommandPoolCreateInfo commmand_pool_create_info = {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			queue_index
		};

		last_result = vkCreateCommandPool(device, &commmand_pool_create_info, nullptr, &command_pool);
		HANDEL(last_result)
	}

	VkCommandBufferAllocateInfo command_buffer_allocate_info = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			command_pool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			image_count
	};
	last_result = vkAllocateCommandBuffers(device, &command_buffer_allocate_info, command_buffers.data());
	HANDEL(last_result)
}

void singleTimeCommand() {
	VkCommandBufferAllocateInfo command_buffer_allocate_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		nullptr,
		command_pool,
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		1
	};
	VkCommandBuffer buffer;
	last_result = vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &buffer);
	HANDEL(last_result);

	VkCommandBufferBeginInfo beginInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		nullptr,
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		nullptr
	};
	last_result = vkBeginCommandBuffer(buffer, &beginInfo);
	HANDEL(last_result);

	VkImageMemoryBarrier image_memory_barrier = {
		VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		nullptr,
		0,
		0,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_QUEUE_FAMILY_IGNORED,
		VK_QUEUE_FAMILY_IGNORED,
		VK_NULL_HANDLE,
		{
			VK_IMAGE_ASPECT_COLOR_BIT,
			0,
			1,
			0,
			1
		}
	};

	for each(Texture* tex in texture_buffers) {
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		image_memory_barrier.image = tex->image;
		image_memory_barrier.srcAccessMask = 0;
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		vkCmdPipelineBarrier(buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

		VkBufferImageCopy image_copy = {
			0,
			0,
			0,
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0,
				0,
				1
			},
			{
				0,
				0,
				0
			},
			{
				tex->width,
				tex->height,
				1
			}
		};

		vkCmdCopyBufferToImage(
			buffer,
			tex->buffer,
			tex->image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&image_copy
		);

		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		image_memory_barrier.image = tex->image;
		image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		vkCmdPipelineBarrier(buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
	}

	image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	image_memory_barrier.image = depth_image;
	image_memory_barrier.srcAccessMask = 0;
	image_memory_barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;;
	image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	vkCmdPipelineBarrier(buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

	last_result = vkEndCommandBuffer(buffer);
	HANDEL(last_result);

	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
		nullptr,
		0,
		nullptr,
		nullptr,
		1,
		&buffer,
		0,
		nullptr,
	};

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(device, command_pool, 1, &buffer);

	for each(Texture* tex in texture_buffers) {
		destroyBufferofTexture(tex);
	}
}

void fillCommandBuffer(VertexBuffer* vbuffer, uint32_t index) {
	VkClearValue clear[2] = {
		{
		    {
		    	1,
		    	1,
		    	1,
		    	1
		    }
	    },
     	{
	        {
				1.0f, 
				0
			}
        }
	};


	VkCommandBuffer buffer = command_buffers[index];

	VkCommandBufferInheritanceInfo command_buffer_inheritance_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
			nullptr,
			render_pass,
			0,
			frame_buffer[index],
			VK_FALSE,
			0,
			0
	};

	VkCommandBufferBeginInfo command_buffer_begin_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		nullptr,
		VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		&command_buffer_inheritance_info
	};
	last_result = vkBeginCommandBuffer(buffer, &command_buffer_begin_info);
	HANDEL(last_result);

	VkRenderPassBeginInfo render_pass_begin_info = {
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		nullptr,
		render_pass,
		frame_buffer[index],
		{
			0,
			0,
			window_list[0]->width,
			window_list[0]->height
		},
		2,
		clear
	};
	vkCmdBeginRenderPass(buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptor_set, 0, nullptr);

	vkCmdBindVertexBuffers(buffer, 0, 1, &buffers[vbuffer->vertex_buffer_index], &offsets);

	vkCmdDraw(buffer, vbuffer->count_of_points, 1, 0, 0);

	vkCmdEndRenderPass(buffer);

	last_result = vkEndCommandBuffer(buffer);
	HANDEL(last_result)
}

void destroyCommandBuffer() {
	vkFreeCommandBuffers(device, command_pool, command_buffers.size(), command_buffers.data());
	vkDestroyCommandPool(device, command_pool, nullptr);
}