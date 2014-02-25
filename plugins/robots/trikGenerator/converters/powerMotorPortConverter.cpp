#include "powerMotorPortConverter.h"

using namespace qReal::robots::generators::converters;

QStringList PowerMotorPortConverter::convert(QString const &data) const
{
	QStringList const ports = data.toUpper().split(QRegExp("[^\\d|A|B|C|JM\\d|M1]"), QString::SkipEmptyParts);
	QStringList result;
	for (QString const &port : ports) {
		switch (port[0].toLatin1()) {
		case 'A':
			result << "JM1";
			break;
		case 'B':
			result << "JM2";
			break;
		case 'C':
			/// @todo this M1 disapoints me :(
			result << "M1";
			break;
		case '1':
			result << "JM1";
			break;
		case '2':
			result << "JM2";
			break;
		case '3':
			result << "M1";
			break;
		case '4':
			result << "JM3";
			break;
		default:
			result << port;
		}
	}

	return result;
}
