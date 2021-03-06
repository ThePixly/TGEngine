#include "Pipe.hpp"

std::vector<VkPipeline> pipelines; 
std::vector<VkPipelineLayout> layouts;

void createPipeline() {
	TG_VECTOR_APPEND_NORMAL(layouts, VkPipelineLayout())

	Window* win = window_list[0];
	vlib_scissor.extent.height = (uint32_t) (vlib_viewport.height = (float) win->height);
	vlib_scissor.extent.width = (uint32_t) (vlib_viewport.width = (float) win->width);

	vlib_layout_info.setLayoutCount = 1;
	vlib_layout_info.pSetLayouts = &descriptor_set_layouts[last_size];
	last_result = vkCreatePipelineLayout(device, &vlib_layout_info, nullptr, &layouts[last_size]);
	HANDEL(last_result);

	vlib_vertex_input_state.vertexAttributeDescriptionCount = (uint32_t)description_attributes.size();
	vlib_vertex_input_state.pVertexAttributeDescriptions = description_attributes.data();

	vlib_multisample_state.rasterizationSamples = used_msaa_flag;

	vlib_graphics_pipeline_create_info.stageCount = (uint32_t)shaders.size();
	vlib_graphics_pipeline_create_info.pStages = shaders.data();
	vlib_graphics_pipeline_create_info.layout = layouts[last_size];
	vlib_graphics_pipeline_create_info.renderPass = render_pass;
	TG_VECTOR_APPEND_NORMAL(pipelines, VkPipeline())
	last_result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &vlib_graphics_pipeline_create_info, nullptr, &pipelines[last_size]);
	HANDEL(last_result);
}

void destroyPipeline() {
	for each (VkPipelineLayout var in layouts)
	{
		vkDestroyPipelineLayout(device, var, nullptr);
	}
	for each (VkPipeline var in pipelines)
	{
		vkDestroyPipeline(device, var, nullptr);
	}
	layouts.clear();
	pipelines.clear();
}