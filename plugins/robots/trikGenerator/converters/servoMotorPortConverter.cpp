#include "servoMotorPortConverter.h"

using namespace qReal::robots::generators::converters;

QStringList ServoMotorPortConverter::convert(QString const &data) const
{
	QStringList const ports = data.toUpper().split(QRegExp("[^\\d|A|B|C|JE\\d]"), QString::SkipEmptyParts);
	QStringList result;
	for (QString const &port : ports) {
		switch (port[0].toLatin1()) {
		case 'A':
			result << "JE1";
			break;
		case 'B':
			result << "JE2";
			break;
		case 'C':
			result << "JE3";
			break;
		case '1':
			result << "JE1";
			break;
		case '2':
			result << "JE2";
			break;
		case '3':
			result << "JE3";
			break;
		case '4':
			result << "JE4";
			break;
		default:
			result << port;
		}
	}

	return result;
}
