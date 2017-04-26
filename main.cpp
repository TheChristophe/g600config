#include "ui.hpp"

#include "helpers.hpp"
#include "usbCommands.hpp"

#include <FL/fl_ask.H>

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	libusb_context *context = nullptr;
	libusb_device **list    = nullptr;

	TryElseReturn(libusb_init(&context), -1);

	TryElseReturn(libusb_get_device_list(context, &list), -2);
	int numDevices = result;

	int deviceFound       = -1;
	libusb_device *device = nullptr;

	for (int i = 0; i < numDevices; i++) {
		device                              = list[i];
		libusb_device_descriptor descriptor = { 0 };

		Try(libusb_get_device_descriptor(device, &descriptor));
		if (result != 0) {
			continue;
		}

		MouseModel *model = nullptr;
		for (int j = 0; j < KnownMouseModels; j++) {
			if (descriptor.idProduct == mouseModels[j].m_id) {
				model = &mouseModels[j];
				break;
			}
		}

		if (model == nullptr) {
			continue;
		}

		std::cout << "Device " << descriptor.idVendor << ":" << descriptor.idProduct << " " << model->m_name << " found" << std::endl;

		deviceFound = i;
	}

	if (deviceFound == -1) {
		fl_alert("No compatible mouse found, attach one and try again!");
	}
	else {
		MouseInfo mouseInfo;
		mouseInfo.m_model  = &mouseModels[deviceFound];
		mouseInfo.m_device = device;
		UserInterface ui(mouseInfo);
		// It does the rest by itself :D
	}

	Fl::run();

	libusb_free_device_list(list, 1);

	return 0;
}
