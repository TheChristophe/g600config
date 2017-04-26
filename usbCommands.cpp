#include "usbCommands.hpp"

#include "helpers.hpp"

MouseModel mouseModels[] = { { 0xc24a, "Logitech G600" } };

int ApplyDelta(MouseModel *mouse, libusb_device *device, const std::vector<Delta> &delta, int mouseID)
{
	// Struct with mouse settings data
	G600Data g600Data;
	memset(&g600Data, 0x00, sizeof(g600Data));

	// Data to send the mouse to modify the first, second or third mouse profile
	// Curious thing, why are they [0xf3, 0xf4, 0xf5]
	//   and not [0x0, 0x1, 0x2] | [0x1, 0x2, 0x3] ?
	unsigned mouseIDs[3] = { 0xf3, 0xf4, 0xf5 };

	libusb_device_handle *handle = nullptr;

	uint8_t *start = nullptr;

	// Obtain handle to mouse
	TryElseReturn(libusb_open(device, &handle), -1);

#ifndef WIN32
	// detach mouse from driver so we can access it
	TryElseGoto(libusb_detach_kernel_driver(handle, 1), close);
#endif

	// Claim the mouse
	TryElseGoto(libusb_claim_interface(handle, 1), attach);

	// Query mouse configuration
	// TODO: get rid of magic numbers
	TryElseGoto(libusb_control_transfer(handle, GetDataRT, GetDataR,
	                0x0300 + mouseIDs[mouseID], 1, (unsigned char *)&g600Data,
	                ConfigSize, Timeout),
	    release);

	if (result != ConfigSize) {
		std::cout << "Error, too few bytes received, expected " << ConfigSize << std::endl;
		goto release;
	}

	// Apply the changes to the configuration
	start = reinterpret_cast<uint8_t *>(&g600Data);
	for (auto &change : delta) {
		start[change.m_offset] = change.m_value;
	}

	// Update mouse configuration
	Try(libusb_control_transfer(handle, SetDataRT, SetDataR,
	    0x0300 + mouseIDs[mouseID], 1,
	    (unsigned char *)&g600Data, ConfigSize, Timeout));

	if (result != ConfigSize) {
		std::cout << "Error, too few bytes sent, expected " << ConfigSize << std::endl;
	}

release:
	// release mouse
	Try(libusb_release_interface(handle, 1));

attach:
#ifndef WIN32
	// reattach mouse to kernel
	Try(libusb_attach_kernel_driver(handle, 1));
#endif

close:
	libusb_close(handle);
}
