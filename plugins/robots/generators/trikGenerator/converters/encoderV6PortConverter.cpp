#include "encoderV6PortConverter.h"

using namespace trik::converters;

QString EncoderV6PortConverter::convert(QString const &data) const
{
	if (data == "A") {
		return "B1";
	} else if (data == "B") {
		return "B2";
	} else if (data == "C") {
		return "B2";
	} else if (data == "1") {
		return "B1";
	} else if (data == "2") {
		return "B2";
	} else if (data == "3") {
		return "B3";
	} else if (data == "4") {
		return "B4";
	} else if (data == "M1") {
		return "B1";
	} else if (data == "M2") {
		return "B2";
	} else if (data == "M3") {
		return "B3";
	} else if (data == "M4") {
		return "B4";
	}

	return data;
}
