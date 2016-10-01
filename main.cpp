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

#define VENDOR_LOGITECH 0x046d
#define TIMEOUT 0x1000
#define CONFIG_SIZE 154

#define failif(condition, tocall)                                                      \
	do {                                                                               \
		if (condition) {                                                               \
			tocall;                                                                    \
			fprintf(stderr, "on line #%i, failif(%s) failed\n", __LINE__, #condition); \
			exit(1);                                                                   \
		}                                                                              \
	} while (false)

struct MouseInfo {
	int m_id;
	char m_name[64];
};

MouseInfo mouseInfos[] = { { 0xc24a, "Logitech G600" } };
#define KNOWN_MOUSE_MODELS sizeof(mouseInfos) / sizeof(MouseInfo)

void configureMouse(MouseInfo *mouseInfo, struct libusb_device *device, const std::vector<std::pair<int, uint8_t>> &delta, int mouseID)
{
	G600Data g600Data;
	memset(&g600Data, 0x00, sizeof(g600Data));

	unsigned mouseIDs[3] = { 0xf3, 0xf4, 0xf5 };

	libusb_device_handle *handle = nullptr;
	int res                      = 0;

	// obtain handle
	res = libusb_open(device, &handle);
	failif(res != 0, printf("libusb_detach_kernel_driver: %i %s\n", res, libusb_error_name(res)));

	// detach mouse from driver so we can access it
	res = libusb_detach_kernel_driver(handle, 1);
	failif(res != 0, printf("libusb_detach_kernel_driver: %i %s\n", res, libusb_error_name(res)));

	// claim the mouse
	res = libusb_claim_interface(handle, 1);
	failif(res != 0, printf("libusb_claim_interface: %i %s\n", res, libusb_error_name(res)));

	// query mouse configuration
	res = libusb_control_transfer(handle, 0xa1, 0x01, 0x0300 + mouseIDs[mouseID], 1, (unsigned char *)&g600Data, CONFIG_SIZE, TIMEOUT);
	failif(res != 154, printf("libusb_control_transfer: %i %s\n", res, (res > 0) ? "success" : libusb_error_name(res)));

	// apply the changes to the configuration
	uint8_t *start = reinterpret_cast<uint8_t *>(&g600Data);
	for (auto &change : delta) {
		start[change.first] = change.second;
	}

	// update mouse configuration
	res = libusb_control_transfer(handle, 0x21, 0x09, 0x0300 + mouseIDs[mouseID], 1, (unsigned char *)&g600Data, CONFIG_SIZE, TIMEOUT);
	failif(res != 154, printf("libusb_control_transfer: %i %s\n", res, (res > 0) ? "success" : libusb_error_name(res)));

	// release mouse
	res = libusb_release_interface(handle, 1);
	failif(res != 0, printf("libusb_release_interface: %i %s\n", res, libusb_error_name(res)));

	// reattach mouse to kernel
	res = libusb_attach_kernel_driver(handle, 1);
	failif(res != 0, printf("libusb_attach_kernel_driver: %i %s\n", res, libusb_error_name(res)););
}

void setColor(MouseInfo *mouseInfo, struct libusb_device *device, int Type, uint8_t Value)
{
}

void setSensitivity(MouseInfo *mouseInfo, struct libusb_device *device, int Type, uint8_t Value)
{

	/*
		// TODO: sensitivity
	uint8_t mouseSenses[4] = {0x42, 0x42, 0x44, 0x46};

	uint8_t sens[4] = {0xf0, mouseSenses[2], 0x00, 0x00};
		// TODO: when do the last two numbers actually change
		// possible last: 00 00, 5f 6e, ad 62

		res = libusb_control_transfer (handle, 0x21, 0x09, 0x03f0, 1, (unsigned char*)&g600data, 154, TIMEOUT);
		printf("libusb_control_transfer: %i %s\n", res, (res > 0) ? "success" :
		libusb_error_name(res));
		failif(res != 4);
	*/
}

int main(int argc, char **argv)
{
	G600Data sampleData;
	memset(&sampleData, 0x0, sizeof(sampleData));
	uint8_t *first = &sampleData.m_profileID;

	std::vector<std::pair<int, uint8_t>> delta;

	const char usage[] = "Usage: g600tool <profile id> (set | get) key value\n";

	if ((argc >= 2) && (argv[1][0] == '1' || argv[1][0] == '2' || argv[1][0] == '3')) {
		if (argc >= 5 && !strcmp(argv[2], "set")) {
			if (!strncmp(argv[3], "mouse", 5)) {
				int targetKey    = atoi(argv[3] + 5) - 1;
				int targetOffset = Offsets::Mouse0 + targetKey * 3;
				if (!strncmp(argv[4], "mouse", 5)) {

					delta.emplace_back(targetOffset, atoi(argv[4] + 5));
					delta.emplace_back(targetOffset + 1, 0);
					delta.emplace_back(targetOffset + 2, 0);
				}
				else if (!strncmp(argv[4], "key", 3)) {
					delta.emplace_back(targetOffset, 0);
					delta.emplace_back(targetOffset + 2, atoi(argv[4] + 3));

					if (argc == 6) {
						delta.emplace_back(targetOffset + 1, atoi(argv[5]));
					}
					else {
						delta.emplace_back(targetOffset + 1, 0);
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
		} // if (!strcmp(argv[2], "set"))
		else if (argc >= 5 && !strcmp(argv[2], "get")) {
		}
		else {
			printf(usage);
			return 0;
		}

	} // if ((argc >= 2) && (argv[1][0] == '1' || argv[1][0] == '2' || argv[1][0] == '3'))
	else {
		printf(usage);
		return 0;
	}

	libusb_context *context = nullptr;
	libusb_device **list    = nullptr;
	int res                 = 0;

	res = libusb_init(&context);
	failif(res != 0, printf("libusb_init: %i %s\n", res, libusb_error_name(res)));

	int numDevices = libusb_get_device_list(context, &list);
	failif(numDevices < 0, printf("libusb_get_device_list: %i\n", numDevices));

	for (int i = 0; i < numDevices; i++) {
		libusb_device *device               = list[i];
		libusb_device_descriptor descriptor = { 0 };

		res = libusb_get_device_descriptor(device, &descriptor);
		failif(res != 0, printf("libusb_get_device_descriptor: %i %s\n", res, libusb_error_name(res)));

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

		printf("Device %04x:%04x %s found\n", descriptor.idVendor, descriptor.idProduct, mouseInfo->m_name);

		// do stuff
		configureMouse(mouseInfo, device, delta, (int)'1' - (int)argv[1][0]);
		break;
	}

	libusb_free_device_list(list, 1);

	return 0;
}
