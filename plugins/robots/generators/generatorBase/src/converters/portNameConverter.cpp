#include "portNameConverter.h"

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

using namespace generatorBase::converters;
using namespace qReal;

PortNameConverter::PortNameConverter(const QString &pathToTemplates
		, QList<kitBase::robotModel::PortInfo> const &ports
		, qReal::ErrorReporterInterface &errorReporter)
	: TemplateParametrizedConverter(pathToTemplates)
	, mPorts(ports)
	, mErrorReporter(errorReporter)
{
}

QString PortNameConverter::convert(const QString &portNameOrAlias) const
{
	QString portName;
	for (const kitBase::robotModel::PortInfo &port : mPorts) {
		if (port.name() == portNameOrAlias || port.nameAliases().contains(portNameOrAlias)) {
			portName = port.name();
			if (port.name() == portNameOrAlias) {
				// Port name is first-priority cause, for example, M1 in TRIK may be an alias for B1.
				break;
			}
		}
	}

	if (portName.isEmpty()) {
		mErrorReporter.addWarning(QObject::tr("Port %1 is unknown. It will be generated as-is.").arg(portNameOrAlias));
		return portNameOrAlias;
	}

	const QString portTemplate = QString("ports/%1.t").arg(portName);
	return readTemplateIfExists(portTemplate, portName);
}
