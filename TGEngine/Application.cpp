#pragma once

#include "Application.h"

namespace Pipeline {

	using namespace std;

	void createApplication(Application app) {

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = app.title;
		appInfo.applicationVersion = app.version;
		appInfo.pEngineName = "TGEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		vector<VkLayerProperties> lay_props;
		uint32_t layprop_cou = 0;
		vkEnumerateInstanceLayerProperties(&layprop_cou, nullptr);
		lay_props.resize(layprop_cou);
		vkEnumerateInstanceLayerProperties(&layprop_cou, lay_props.data());

		int xc = 0;

		vector<char*> val_pon = {};

		for (size_t gh = 0; gh < app.layers_to_enable.size(); gh++)
		{
			string point = app.layers_to_enable[gh];
			for (size_t cf = 0; cf < layprop_cou; cf++)
			{
				string name = lay_props[cf].layerName;
				if (name == point) {
					val_pon.resize(xc + 1);
					val_pon[xc] = app.layers_to_enable[gh];
					xc++;
					break;
				}
			}
		}
		uint32_t exten_cout_used = 0;
		const char** exts = glfwGetRequiredInstanceExtensions(&exten_cout_used);

		VkInstanceCreateInfo InscreatInfo = {};
		InscreatInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		InscreatInfo.pApplicationInfo = &appInfo;
		if (val_pon.size() > 0) {
			InscreatInfo.enabledLayerCount = val_pon.size();
			InscreatInfo.ppEnabledLayerNames = val_pon.data();
		}
		if (exten_cout_used > 0) {
			InscreatInfo.enabledExtensionCount = exten_cout_used;
			InscreatInfo.ppEnabledExtensionNames = exts;
		}

		handel(vkCreateInstance(&InscreatInfo, nullptr, app.instance));

		handel(glfwCreateWindowSurface(*app.instance, (*app.window).window, nullptr, app.KHR));
	}

	void destroyApplictaion(Application app) {
		vkDestroySurfaceKHR(*app.instance, *app.KHR, nullptr);
		vkDestroyInstance(*app.instance, nullptr);
	}

}