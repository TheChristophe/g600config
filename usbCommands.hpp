#ifndef USBCOMMANDS_HPP
#define USBCOMMANDS_HPP

#include <libusb-1.0/libusb.h>

#include <cstddef>
#include <cstdint>
#include <vector>

// Structs

struct G600Data {
	// List of keys:
	// 1, 2, 3 - Mouse Left, Right, Middle
	// 4, 5 - Scroll Left, Right
	// 6 - G-Switch (extra key on the right)
	// 7:20 - G7:G20 (labelled on mouse)

	uint8_t m_profileID; // f3, f4 or f5

	// I do not yet know the difference between this and the second color set
	// below
	uint8_t m_red; // 0x00 - 0x63
	uint8_t m_green;
	uint8_t m_blue;

	uint8_t m_effect;      // 0x00 off, 0x01 blink, 0x02 rainbow
	uint8_t m_effectSpeed; // 0 fast - 7 slow

	uint8_t m_unknown2[5];

	uint8_t m_pollingRate;        // 0 1000Hz - 7 125Hz
	uint8_t m_switchSensitivity;  // equal to a value of m_sensivities
	uint8_t m_defaultSensitivity; // 0x1-0x4
	uint8_t m_sensitivities[4];   // multiples of 50, minimum in Logitech's tool
	                              // is 0x4

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
} __attribute__((packed)); // total size 154, assuming the packed works

struct MouseModel {
	int m_id;
	char m_name[64];
};

struct MouseInfo {
	MouseModel *m_model;
	struct libusb_device *m_device;
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

// Declarations

extern MouseModel mouseModels[1];

// Constants

enum {
	VendorLogitech   = 0x046d,
	ConfigSize       = 154,    // wLength
	Timeout          = 0x1000, // timeout
	KnownMouseModels = sizeof(mouseModels) / sizeof(MouseModel),

	// RT bmRequestType, R bRequest
	GetDataRT = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, // 0xa1
	GetDataR  = LIBUSB_REQUEST_CLEAR_FEATURE,                                                // I would really like to ask the Logitech devs why this random ass request and not GET_CONFIGURATION

	SetDataRT = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
	SetDataR  = LIBUSB_REQUEST_SET_CONFIGURATION,
};

namespace Offsets {
	enum {
		ProfileID          = 0x0000,
		Red                = 0x0001,
		Green              = 0x0002,
		Blue               = 0x0003,
		Effect             = 0x0004,
		EffectSpeed        = 0x0005,
		PollingRate        = 0x000b,
		SwitchSensitivity  = 0x000c,
		DefaultSensitivity = 0x000d,
		Sensitivity0       = 0x000e,
		Mouse0             = 0x001f,
		Red2               = 0x005b,
		Green2             = 0x005c,
		Blue2              = 0x005d,
		AltDelta           = 0x003f,
	};
};

// Functions

int ApplyDelta(MouseModel *mouse, struct libusb_device *device, const std::vector<Delta> &delta, int profileID);
int ApplyData(MouseModel *mouse, struct libusb_device *device, const G600Data &data, int profileID);

int ReadData(MouseModel *mouse, struct libusb_device *device, G600Data &destination, int profileID);

#endif
