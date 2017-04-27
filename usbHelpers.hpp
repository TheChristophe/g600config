#ifndef USB_HELPERS_HPP
#define USB_HELPERS_HPP

#include "usbCommands.hpp"

void SetConfig(void *info, int profileID, const G600Data &data);
void GetConfig(void *info, int profileID, G600Data &data);

namespace Set {
	void Colors(void *info, int profileID, int red, int green, int blue);
};

#endif
