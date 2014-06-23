#include "engineV6PortConverter.h"

using namespace trik::converters;

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

/// @todo: JE1 suddenly gets converted to JM1, commented out.
//	case 'J':
//		// Got JM- or JE-form port
//		return data.length() == 3 ? convert(data[2]) : data;
	case '1':
		return "JM1";
	case '2':
		return "JM2";
	case '3':
		return "JM3";
	case '4':
		return "JM4";
	default:
		return data;
	}
}
