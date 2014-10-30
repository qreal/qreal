#include "portNameConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

PortNameConverter::PortNameConverter(QString const &pathToTemplates
		, QList<interpreterBase::robotModel::PortInfo> const &ports)
	: TemplateParametrizedConverter(pathToTemplates)
	, mPorts(ports)
{
}

QString PortNameConverter::convert(QString const &portNameOrAlias) const
{
	QString portName;
	for (interpreterBase::robotModel::PortInfo const &port : mPorts) {
		if (port.name() == portNameOrAlias || port.nameAliases().contains(portNameOrAlias)) {
			portName = port.name();
			if (port.name() == portNameOrAlias) {
				// Port name is first-priority cause, for example, M1 in TRIK may be an alias for B1.
				break;
			}
		}
	}

	if (portName.isEmpty()) {
		/// @todo: Report an error somewhere
		return QString();
	}

	QString const portTemplate = QString("ports/%1.t").arg(portName);
	return QFile(pathToRoot() + "/" + portTemplate).exists() ? readTemplate(portTemplate) : portName;
}
