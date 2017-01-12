#include "config.h"
#include "key_info.h"

#include <libusb-1.0/libusb.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct MouseInfo {
	int m_id;
	char m_name[64];
};

struct Delta {
	// For emplace_back
	Delta(ptrdiff_t offset, uint8_t value)
	    : m_offset(offset)
	    , m_value(value)
	{
	}

	ptrdiff_t m_offset;
	uint8_t m_value;
};

MouseInfo mouseInfos[] = { { 0xc24a, "Logitech G600" } };

using LibusbDevice = struct libusb_device;

enum {
	VendorLogitech   = 0x046d,
	Timeout          = 0x1000,
	ConfigSize       = 154,
	KnownMouseModels = sizeof(mouseInfos) / sizeof(MouseInfo),
};

void ApplyDelta(MouseInfo *mouse, LibusbDevice *device,
    const std::vector<Delta> &delta, int mouseID)
{
	G600Data g600Data;
	memset(&g600Data, 0x00, sizeof(g600Data));

	unsigned mouseIDs[3] = { 0xf3, 0xf4, 0xf5 };

	libusb_device_handle *handle = nullptr;
	int res                      = 0;

	// Obtain handle to mouse
	res = libusb_open(device, &handle);
	if (res != 0) {
		std::cout << "An error occured: libusb_detach_kernel_driver: " << res
		          << libusb_error_name(res);
		return;
	}

	// detach mouse from driver so we can access it
	res = libusb_detach_kernel_driver(handle, 1);
	if (res != 0) {
		std::cout << "An error occured: libusb_detach_kernel_driver: " << res
		          << libusb_error_name(res);
		goto close;
	}

	// Claim the mouse
	res = libusb_claim_interface(handle, 1);
	if (res != 0) {
		std::cout << "An error occured: libusb_claim_interface:" << res
		          << libusb_error_name(res);
		goto attach;
	}

	// Query mouse configuration
	// TODO: get rid of magic numbers
	res = libusb_control_transfer(handle, 0xa1, 0x01,
	    0x0300 + mouseIDs[mouseID], 1, (unsigned char *)&g600Data, CONFIG_SIZE,
	    TIMEOUT);

	if (res != ConfigSize) {
		if (res > 0) {
			std::cout << "An error occured: libusb_control_transfer: Too few "
			             "bytes received, only received "
			          << res " bytes, expected " << ConfigSize;
		}
		else {
			std::cout << "An error occured: libusb_control_transfer: " << res
			          << libusb_error_name(res);
		}
		goto release;
	}

	// Apply the changes to the configuration
	uint8_t *start = reinterpret_cast<uint8_t *>(&g600Data);
	for (auto &change : delta) {
		start[change.m_offset] = change.m_value;
	}

	// Update mouse configuration
	res = libusb_control_transfer(handle, 0x21, 0x09,
	    0x0300 + mouseIDs[mouseID], 1, (unsigned char *)&g600Data, ConfigSize,
	    Timeout);

	if (res != ConfigSize) {
		if (res > 0) {
			std::cout << "An error occured: libusb_control_transfer: Too few "
			             "bytes sent, only sent "
			          << res " bytes, expected " ConfigSize;
		}
		else {
			std::cout << "An error occured: libusb_control_transfer: " << res
			          << libusb_error_name(res);
		}
	}

release:
	// release mouse
	res = libusb_release_interface(handle, 1);
	if (res != 0) {
		std::cout << "An error occured: libusb_release_interface: ",
		    << res << libusb_error_name(res);
	}

attach:
	// reattach mouse to kernel
	res = libusb_attach_kernel_driver(handle, 1);
	if (res != 0) {
		std::cout << "An error occured: libusb_attach_kernel_driver: " << res
		          << libusb_error_name(res);
	}

close:
	libusb_close(handle);
}

void SetColor(
    MouseInfo *mouseInfo, struct libusb_device *device, int Type, uint8_t Value)
{
}

void SetSensitivity(
    MouseInfo *mouseInfo, struct libusb_device *device, int Type, uint8_t Value)
{

	/*
	    // TODO: sensitivity
	uint8_t mouseSenses[4] = {0x42, 0x42, 0x44, 0x46};

	uint8_t sens[4] = {0xf0, mouseSenses[2], 0x00, 0x00};
	    // TODO: when do the last two numbers actually change
	    // possible last: 00 00, 5f 6e, ad 62

	    res = libusb_control_transfer (handle, 0x21, 0x09, 0x03f0, 1,
	(unsigned
	char*)&g600data, 154, TIMEOUT);
	    printf("libusb_control_transfer: %i %s\n", res, (res > 0) ?
	"success" :
	    libusb_error_name(res));
	    failif(res != 4);
	*/
}

int main(int argc, char **argv)
{
	G600Data sampleData;
	memset(&sampleData, 0x0, sizeof(sampleData));
	uint8_t *first = &sampleData.m_profileID;

	std::vector<Delta> delta;

	//
	// Handle user input
	//

	std::string usage
	    = "Usage: g600tool <profile id> (set | get) key [value]\n";

	// Check for at least "g600tool <profile id> (set | get)"
	if (argc < 3) {
		std::cout << usage << std::endl;
		return 0;
	}

	// Verify if the specified profile is valid
	if (argv[1][0] != '1' && argv[1][0] != '2' && argv[1][0] != '3') {
		std::cout << "Invalid profile id specified" << std::endl;
		return 0;
	}

	std::string arg2 = argv[2];
	if (arg2 == "set") {
		// Check for first 3 + "key value"
		if (argc < 5) {
			std::cout << usage << std::endl;
			return 0;
		}

		// User wants to bind to a mousekey
		if (!strncmp(argv[3], "mouse", 5)) {
			int key          = atoi(argv[3] + 5) - 1;
			ptrdiff_t byte   = sizeof(uint8_t);
			ptrdiff_t offset = Offsets::Mouse0 + targetKey * byte;

			// User wants to bind a mouse key
			if (!strncmp(argv[4], "mouse", 5)) {
				delta.emplace_back(offset, atoi(argv[4] + 5)); // mouse key
				delta.emplace_back(offset + byte, 0);          // modifier
				delta.emplace_back(offset + 2 * byte, 0);      // keyboard key
			}
			// User wants to bind a keyboard key
			else if (!strncmp(argv[4], "key", 3)) {
				delta.emplace_back(offset, 0);
				delta.emplace_back(offset + 2 * byte, atoi(argv[4] + 3));

				// Check for a modifier
				// TODO: make this more user friendly
				if (argc >= 6) {
					delta.emplace_back(offset + byte, atoi(argv[5]));
				}
				else {
					delta.emplace_back(offset + byte, 0);
				}
			}
		} // if (!strncmp(argv[3], "mouse", 5))
		else if (!strcmp(argv[3], "red")) {
			delta.emplace_back(Offsets::Red, atoi(argv[4]));
			delta.emplace_back(Offsets::Red2, atoi(argv[4]));
		}
		else if (!strcmp(argv[3], "green")) {
			delta.emplace_back(Offsets::Green, atoi(argv[4]));
			delta.emplace_back(Offsets::Green2, atoi(argv[4]));
		}
		else if (!strcmp(argv[3], "blue")) {
			delta.emplace_back(Offsets::Blue, atoi(argv[4]));
			delta.emplace_back(Offsets::Blue2, atoi(argv[4]));
		}
		else if (!strcmp(argv[3], "effect")) {
			delta.emplace_back(Offsets::Effect, atoi(argv[4]));
		}
		else if (!strcmp(argv[3], "effectSpeed")) {
			delta.emplace_back(Offsets::EffectSpeed, atoi(argv[4]));
		}
		else if (!strcmp(argv[3], "pollingRate")) {
			delta.emplace_back(Offsets::PollingRate, atoi(argv[4]));
		}
	}
	else if (arg2 == "get") {
		// Check for first 3 + "key"
		if (argv < 4) {
			std::cout << usage << std::endl;
		}
		std::string arg3 = argv[3];
	}

	//
	// Apply changes to mouse, or query values
	//

	libusb_context *context = nullptr;
	libusb_device **list    = nullptr;
	int res                 = 0;

	res = libusb_init(&context);
	if (res != 0) {
		std::cout << "An error occured: libusb_init: " << res
		          << libusb_error_name(res);
		return -1;
	}

	int numDevices = libusb_get_device_list(context, &list);
	failif(numDevices < 0, printf("libusb_get_device_list: %i\n", numDevices));

	for (int i = 0; i < numDevices; i++) {
		libusb_device *device               = list[i];
		libusb_device_descriptor descriptor = { 0 };

		res = libusb_get_device_descriptor(device, &descriptor);
		if (res != 0) {
			std::cout << "An error occured: libusb_get_device_descriptor: "
			          << res << libusb_error_name(res);
			continue;
		}

		MouseInfo *mouseInfo = nullptr;
		for (int i = 0; i < KNOWN_MOUSE_MODELS; i++) {
			if (descriptor.idProduct == mouseInfos[i].m_id) {
				mouseInfo = &mouseInfos[i];
				break;
			}
		}

		if (mouseInfo == nullptr) {
			continue;
		}

		printf("Device %04x:%04x %s found\n", descriptor.idVendor,
		    descriptor.idProduct, mouseInfo->m_name);

		// Apply changes
		ApplyDelta(mouseInfo, device, delta,
		    -(static_cast<int>('1') + static_cast<int>(argv[1][0])));
		break;
	}

	libusb_free_device_list(list, 1);

	return 0;
}
