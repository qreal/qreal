#include "engineV6PortConverter.h"

using namespace fSharp::converters;

QString PowerV6MotorPortConverter::splitRegexp()
{
	return "[^\\d|A|B|C|JM\\d|M1|JE\\d]";
}

QString PowerV6MotorPortConverter::convert(QString const &data) const
{
	switch (data[0].toLatin1()) {
	case 'A':
		return "M1";
	case 'B':
		return "M2";
	case 'C':
		return "M3";
	case '1':
		return "M1";
	case '2':
		return "M2";
	case '3':
		return "M3";
	case '4':
		return "M4";
	default:
		return data;
	}
}
