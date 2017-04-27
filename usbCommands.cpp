#include "usbCommands.hpp"

#include "helpers.hpp"
#include "keys.hpp"

#include <cstring>

#define failif(x)
#define TIMEOUT 0x1000

MouseModel mouseModels[] = { { 0xc24a, "Logitech G600" } };

int ApplyDelta(MouseModel *mouse, struct libusb_device *device, const std::vector<Delta> &delta, int profileID)
{
	// Struct with mouse settings data
	G600Data g600Data{ 0xf3 };

	// Data to send the mouse to modify the first, second or third mouse profile
	// Curious thing, why are they [0xf3, 0xf4, 0xf5]
	//   and not [0x0, 0x1, 0x2] | [0x1, 0x2, 0x3] ?
	unsigned profileIDs[3] = { 0xf3, 0xf4, 0xf5 };

	libusb_device_handle *handle = nullptr;

	uint8_t *start = reinterpret_cast<uint8_t *>(&g600Data);

	// Obtain handle to mouse
	TryElseReturn(libusb_open(device, &handle), -1);

	int interface = 1;

#ifndef WIN32
	bool kernelAttached = false;
	if (libusb_kernel_driver_active(handle, interface) == 1) {
		kernelAttached = true;
		TryElseGoto(libusb_detach_kernel_driver(handle, interface), close);
	}

#endif

	// Claim the mouse
	TryElseGoto(libusb_claim_interface(handle, interface), close);

	// Query mouse configuration
	Try(libusb_control_transfer(handle, GetDataRT, GetDataR, 0x0300 + profileIDs[profileID], 1, (unsigned char *)&g600Data,
	    ConfigSize, Timeout));

	if (result != ConfigSize) {
		std::cout << "Error, too few bytes received, expected " << ConfigSize << std::endl;
		goto release;
	}

	// Apply the changes to the configuration
	for (auto &change : delta) {
		start[change.m_offset] = change.m_value;
	}

	// Update mouse configuration
	Try(libusb_control_transfer(handle, SetDataRT, SetDataR, 0x0300 + profileIDs[profileID], 1,
	    (unsigned char *)&g600Data, ConfigSize, Timeout));

	if (result != ConfigSize) {
		std::cout << "Error, too few bytes sent, sent " << result << ", expected " << ConfigSize << std::endl;
	}

release:
	// release mouse
	Try(libusb_release_interface(handle, interface));

attach:
#ifndef WIN32
	// reattach mouse to kernel
	if (kernelAttached) {
		Try(libusb_attach_kernel_driver(handle, interface));
	}
#endif

close:
	libusb_close(handle);
}

int ApplyData(MouseModel *mouse, struct libusb_device *device, const G600Data &data, int profileID)
{
	// Data to send the mouse to modify the first, second or third mouse profile
	unsigned profileIDs[3] = { 0xf3, 0xf4, 0xf5 };

	libusb_device_handle *handle = nullptr;

	// Obtain handle to mouse
	TryElseReturn(libusb_open(device, &handle), -1);

	int interface = 1;

#ifndef WIN32
	bool kernelAttached = false;
	if (libusb_kernel_driver_active(handle, interface) == 1) {
		kernelAttached = true;
		TryElseGoto(libusb_detach_kernel_driver(handle, interface), close);
	}

#endif

	// Claim the mouse
	TryElseGoto(libusb_claim_interface(handle, interface), close);

	// Update mouse configuration
	Try(libusb_control_transfer(handle, SetDataRT, SetDataR, 0x0300 + profileIDs[profileID], 1,
	    (unsigned char *)&data, ConfigSize, Timeout));

	if (result != ConfigSize) {
		std::cout << "Error, too few bytes sent, sent " << result << ", expected " << ConfigSize << std::endl;
	}

release:
	// release mouse
	Try(libusb_release_interface(handle, interface));

attach:
#ifndef WIN32
	// reattach mouse to kernel
	if (kernelAttached) {
		Try(libusb_attach_kernel_driver(handle, interface));
	}
#endif

close:
	libusb_close(handle);
}

int ReadData(MouseModel *mouse, struct libusb_device *device, G600Data &data, int profileID)
{
	// Data to send the mouse to modify the first, second or third mouse profile
	unsigned profileIDs[3] = { 0xf3, 0xf4, 0xf5 };

	libusb_device_handle *handle = nullptr;

	// Obtain handle to mouse
	TryElseReturn(libusb_open(device, &handle), -1);

	int interface = 1;

#ifndef WIN32
	bool kernelAttached = false;
	if (libusb_kernel_driver_active(handle, interface) == 1) {
		kernelAttached = true;
		TryElseGoto(libusb_detach_kernel_driver(handle, interface), close);
	}

#endif

	// Claim the mouse
	TryElseGoto(libusb_claim_interface(handle, interface), close);

	// Query mouse configuration
	Try(libusb_control_transfer(handle, GetDataRT, GetDataR, 0x0300 + profileIDs[profileID], 1, (unsigned char *)&data,
	    ConfigSize, Timeout));

	if (result != ConfigSize) {
		std::cout << "Error, too few bytes received, expected " << ConfigSize << std::endl;
	}

release:
	// release mouse
	Try(libusb_release_interface(handle, interface));

attach:
#ifndef WIN32
	// reattach mouse to kernel
	if (kernelAttached) {
		Try(libusb_attach_kernel_driver(handle, interface));
	}
#endif

close:
	libusb_close(handle);
}
