#include "usbHelpers.hpp"

#include "usbCommands.hpp"

void SetConfig(void *info, int profileID, const G600Data &data)
{
	MouseInfo *mouseInfo = (MouseInfo *)info;
	ApplyData(mouseInfo->m_model, mouseInfo->m_device, data, profileID);
}

void GetConfig(void *info, int profileID, G600Data &data)
{
	MouseInfo *mouseInfo = (MouseInfo *)info;
	ReadData(mouseInfo->m_model, mouseInfo->m_device, data, profileID);
}

namespace Set {

	void Colors(void *info, int profileID, int red, int green, int blue)
	{
		MouseInfo *mouseInfo = (MouseInfo *)info;

		std::vector<Delta> delta;
		delta.emplace_back(Offsets::Red, red);
		delta.emplace_back(Offsets::Red2, red);
		delta.emplace_back(Offsets::Green, green);
		delta.emplace_back(Offsets::Green2, green);
		delta.emplace_back(Offsets::Blue, blue);
		delta.emplace_back(Offsets::Blue2, blue);

		ApplyDelta(mouseInfo->m_model, mouseInfo->m_device, delta, profileID);
	}
};
