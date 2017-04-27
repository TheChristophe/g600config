#ifndef USB_HELPERS_HPP
#define USB_HELPERS_HPP

#include "usbCommands.hpp"

void SetConfig(void *info, int profileID, const G600Data &data);
void GetConfig(void *info, int profileID, G600Data &data);

namespace Set {
	void Colors(void *info, int profileID, int red, int green, int blue);

	/*	std::string arg2 = argv[2];
		if (arg2 == "set") {
			// User wants to bind to a mouse key
			if (!strncmp(argv[3], "mouse", 5)) {
				int key          = atoi(argv[3] + 5) - 1;
				ptrdiff_t byte   = sizeof(uint8_t);
				ptrdiff_t offset = Offsets::Mouse0 + key * byte;

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
			else if (!strcmp(argv[3], "effect")) {
				delta.emplace_back(Offsets::Effect, atoi(argv[4]));
			}
			else if (!strcmp(argv[3], "effectSpeed")) {
				delta.emplace_back(Offsets::EffectSpeed, atoi(argv[4]));
			}
			else if (!strcmp(argv[3], "pollingRate")) {
				delta.emplace_back(Offsets::PollingRate, atoi(argv[4]));
			}
		}*/
};

#endif
