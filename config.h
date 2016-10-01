#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

struct Offsets {
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
	uint8_t m_sensitivities[4];   // multiples of 50, minimum in Logitech's tool is 0x4

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

#endif
