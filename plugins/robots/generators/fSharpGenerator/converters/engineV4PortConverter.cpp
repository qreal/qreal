#include "engineV4PortConverter.h"

using namespace fSharp::converters;

QString PowerV4MotorPortConverter::splitRegexp()
{
	return "[^\\d|A|B|C|JM\\d|JE\\d]";
}

QString PowerV4MotorPortConverter::convert(QString const &data) const
{
	/// @todo: use port aliases for this purpose
	switch (data[0].toLatin1()) {
	case 'A':
		return "JM1";
	case 'B':
		return "JM2";
	case 'C':
		return "M1";
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
