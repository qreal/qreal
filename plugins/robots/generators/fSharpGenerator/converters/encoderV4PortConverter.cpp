#include "encoderV4PortConverter.h"

using namespace fSharp::converters;

QString EncoderV4PortConverter::convert(QString const &data) const
{
	if (data == "A") {
		return "JB1";
	} else if (data == "B") {
		return "JB2";
	} else if (data == "C") {
		return "JB2";
	} else if (data == "1") {
		return "JB1";
	} else if (data == "2") {
		return "JB2";
	} else if (data == "3") {
		return "JB3";
	} else if (data == "4") {
		return "JB4";
	} else if (data == "JM1") {
		return "JB1";
	} else if (data == "JM2") {
		return "JB2";
	} else if (data == "M1") {
		return "JB3";
	} else if (data == "JM3") {
		return "JB4";
	}

	return data;
}
