#include "servoMotorPortConverter.h"

using namespace trik::converters;

QString ServoMotorPortConverter::splitRegexp()
{
	return "[^\\d|A|B|C|JE\\d]";
}

QString ServoMotorPortConverter::convert(QString const &data) const
{
	switch (data[0].toLatin1()) {
	case 'A':
		return "JE1";
	case 'B':
		return "JE2";
	case 'C':
		return "JE3";
	case 'J':
		// Got JE-form port
		return data.length() == 3 ? convert(data[2]) : data;
	case '1':
		return "JE1";
	case '2':
		return "JE2";
	case '3':
		return "JE3";
	case '4':
		return "JE4";
	default:
		return data;
	}
}
