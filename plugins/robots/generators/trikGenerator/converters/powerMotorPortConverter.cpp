#include "powerMotorPortConverter.h"

using namespace trik::converters;

QString PowerMotorPortConverter::splitRegexp()
{
	return "[^\\d|A|B|C|JM\\d|M1]";
}

QString PowerMotorPortConverter::convert(QString const &data) const
{
	switch (data[0].toLatin1()) {
	case 'A':
		return "JM1";
	case 'B':
		return "JM2";
	case 'C':
		/// @todo this M1 disapoints me :(
		return "M1";
	case 'J':
		// Got JM-form port
		return data.length() == 3 ? convert(data[2]) : data;
	case '1':
		return "JM1";
	case '2':
		return "JM2";
	case '3':
		return "M1";
	case '4':
		return "JM3";
	default:
		return data;
	}
}
