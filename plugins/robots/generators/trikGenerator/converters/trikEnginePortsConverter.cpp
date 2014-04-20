#include "trikEnginePortsConverter.h"

using namespace trik::converters;

TrikEnginePortsConverter::TrikEnginePortsConverter(QString const &pathToTemplates)
	: TemplateParametrizedMultiConverter(pathToTemplates)
{
}

QStringList TrikEnginePortsConverter::convert(QString const &data) const
{
	QStringList const ports = data.toUpper().split(QRegExp("[^\\d|A|B|C]"), QString::SkipEmptyParts);
	QStringList result;
	foreach (QString const &port, ports) {
		switch (port[0].toLatin1()) {
		case 'A':
			result << "2";
			break;
		case 'B':
			result << "1";
			break;
		case 'C':
			result << "2";
			break;
		default:
			result << port;
		}
	}

	return result;
}
