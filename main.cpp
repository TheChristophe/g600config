
#include <cstdio>
#include <cstdlib>

#include <libusb-1.0/libusb.h>

#define VENDOR_LOGITECH 0x046d
#define TIMEOUT 0x1000

#define failif(condition)                                                              \
	do {                                                                               \
		if (condition) {                                                               \
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

struct Key {
	struct Mouse {
		enum {
			Left               = 0x1,
			Right              = 0x2,
			Middle             = 0x3,
			Back               = 0x4,
			Forwards           = 0x5,
			IncreaseResolution = 0x11,
			DecreaseResolution = 0x12,
			NextResolution     = 0x13,
			G600ModeSwitch     = 0x14,
			SwitchResolution   = 0x15,
			ResetResolution    = 0x16,
			GSwitch            = 0x17,
		};
	};
	struct Keyboard {
		enum {
			Right = 0x4f,
			Left  = 0x50
		};
	};
};

struct G600Data {
	// List of keys:
	// 1, 2, 3 - Mouse 1, 2, 3
	// 4, 5 - Scroll Left, Right
	// 6 - G-Switch
	// 7:20 - G7:G20

	uint8_t m_profileID; // f3, f4 or f5
	uint8_t m_red;       // 0x00 - 0x63
	uint8_t m_green;
	uint8_t m_blue;

	uint8_t m_effect;      // 0x00 off, 0x01 blink, 0x02 rainbow
	uint8_t m_effectSpeed; // 0 fast - 7 slow

	uint8_t m_unknown2[5];

	uint8_t m_pollingRate;        // 0 1000Hz - 7 125Hz
	uint8_t m_switchSensitivity;  // equal to a value of m_sensivities
	uint8_t m_defaultSensitivity; // 0x1-0x4
	uint8_t m_sensivities[4];     // multiples of 50, minimum in Logitech's tool is 0x4

	uint8_t m_unknown3[13];

	struct {
		uint8_t m_mouseKey;
		uint8_t m_modifier;
		uint8_t m_keyboardKey;
	} m_keys[20];

	uint8_t m_red2;   // 0x00 - 0xff
	uint8_t m_green2; // 0x00 - 0xff
	uint8_t m_blue2;  // 0x00 - 0xff

	struct {
		uint8_t m_mouseKey;
		uint8_t m_modifier;
		uint8_t m_keyboardKey;
	} m_altKeys[20];
} __attribute__((packed)); // total size 154, hope the packed works

static void configureMouse(MouseInfo *mouseInfo, struct libusb_device *device)
{
	G600Data g600data{
		// Misc
		0xf3,
		0x7f,
		0x00,
		0x00,
		0x00,
		0x00,
		{ 0x0 },
		0x00,
		0x80,
		0x02,
		{ 0x04, 0x08, 0x10, 0x00 },
		{ 0x0 },
		// Keybinds
		{
		    { Key::Mouse::Left, 0x00, 0x00 },
		    { Key::Mouse::Right, 0x00, 0x00 },
		    { Key::Mouse::Middle, 0x00, 0x00 },
		    { Key::Mouse::Back, 0x00, 0x00 },
		    { Key::Mouse::Forwards, 0x00, 0x00 },
		    { Key::Mouse::Middle, 0x00, 0x00 } },
		// Weird RGB
		0xff,
		0x00,
		0xff,
		// Alt keybinds
		{
		    { Key::Mouse::Left, 0x00, 0x00 },
		    { Key::Mouse::Right, 0x00, 0x00 },
		    { Key::Mouse::Middle, 0x00, 0x00 },
		    { Key::Mouse::Back, 0x00, 0x00 },
		    { Key::Mouse::Forwards, 0x00, 0x00 },
		    { Key::Mouse::Middle, 0x00, 0x00 } }
	};

	libusb_device_handle *handle = nullptr;
	int res                      = 0;

	res = libusb_open(device, &handle);
	printf("libusb_detach_kernel_driver: %i %s\n", res, libusb_error_name(res));
	failif(res != 0);

	res = libusb_detach_kernel_driver(handle, 1);
	printf("libusb_detach_kernel_driver: %i %s\n", res, libusb_error_name(res));
	failif(res != 0);

	res = libusb_claim_interface(handle, 1);
	printf("libusb_claim_interface: %i %s\n", res, libusb_error_name(res));
	failif(res != 0);

	// 154 is the size of the packet
	res = libusb_control_transfer(handle, 0x21, 0x09, 0x03f3, 1, (unsigned char *)&g600data, 154, TIMEOUT);
	printf("libusb_control_transfer: %i %s\n", res, (res > 0) ? "success" : libusb_error_name(res));
	failif(res != 154);

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

	res = libusb_release_interface(handle, 1);
	printf("libusb_release_interface: %i %s\n", res, libusb_error_name(res));
	failif(res != 0);

	res = libusb_attach_kernel_driver(handle, 1);
	printf("libusb_attach_kernel_driver: %i %s\n", res, libusb_error_name(res));
	failif(res != 0);
}

int main(int argc, char **argv)
{
	libusb_context *context = nullptr;
	libusb_device **list    = nullptr;
	int res                 = 0;

	res = libusb_init(&context);
	printf("libusb_init: %i %s\n", res, libusb_error_name(res));
	failif(res != 0);

	int numDevices = libusb_get_device_list(context, &list);
	printf("libusb_get_device_list: %i\n", numDevices);
	failif(numDevices < 0);

	for (int i = 0; i < numDevices; i++) {
		libusb_device *device               = list[i];
		libusb_device_descriptor descriptor = { 0 };

		res = libusb_get_device_descriptor(device, &descriptor);
		printf("libusb_get_device_descriptor: %i %s\n", res, libusb_error_name(res));
		failif(res != 0);

		MouseInfo *mouseInfo = nullptr;
		printf("%x\n", descriptor.idProduct);
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
		configureMouse(mouseInfo, device);
		break;
	}

	libusb_free_device_list(list, 1);

	return 0;
}
