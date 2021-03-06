#pragma once

#include "../Stdbase.hpp"
#include "window/Window.hpp"
#include "Device.hpp"
#include "PrePipeline.hpp"
#include "Framebuffer.hpp"
#include "CommandBuffer.hpp"
#include "Pipe.hpp"
#include "RenderPass.hpp"
#include "DepthStencil.hpp"
#include "../game_content/Camera.hpp"
#include "buffer/IndexBuffer.hpp"
#include "buffer/VertexBuffer.hpp"

extern VkSwapchainKHR swapchain;
extern VkSurfaceCapabilitiesKHR surface_capabilities;
extern VkImage color_image;
extern VkImageView color_image_view;
extern VkDeviceMemory color_image_memory;

SINCE(0, 0, 1)
void createSwapchain();

SINCE(0, 0, 4)
void createColorResouce();

SINCE(0, 0, 4)
void destroyColorResouce();

SINCE(0, 0, 3)
void recreateSwapchain(IndexBuffer* ibuffer, VertexBuffer* vbuffer);

SINCE(0, 0, 1)
void destroySwapchain();