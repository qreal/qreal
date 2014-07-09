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
		return "JM1";
	case 'B':
		return "JM2";
	case 'C':
		return "JM3";


    case 'M':
        return data.length() == 2 ? convert(data[1]) : data;
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
