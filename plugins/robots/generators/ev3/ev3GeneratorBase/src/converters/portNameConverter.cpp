#include "portNameConverter.h"

using namespace ev3::converters;

PortNameConverter::PortNameConverter(QString const &pathToTemplates)
	: TemplateParametrizedConverter(pathToTemplates)
{
}

QString PortNameConverter::convert(QString const &portNameOrAlias) const
{
	QString portName = portNameOrAlias;
	QStringList myStringList = portName.toUpper().split(QRegExp("\\W+"), QString::SkipEmptyParts);
	QSet<QString> stringSet = QSet<QString>::fromList(myStringList);
	portName = "";

	foreach (const QString &value, stringSet) {
		portName += value;
	}

	if (portName.isEmpty()) {
		return QString();
	}

	for (int i = 0; i < portName.length(); i++) {
		if (portName.at(i) != 'A' && portName.at(i) != 'B'
				&& portName.at(i) != 'C' && portName.at(i) != 'D') {
			return QString();
		}
	}

	QString const portTemplate = QString("ports/%1.t").arg(portName);
	return readTemplateIfExists(portTemplate, portName);
}
