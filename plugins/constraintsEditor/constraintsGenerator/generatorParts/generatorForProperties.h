#pragma once

#include <QtCore/QString>
#include <QtCore/QPair>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrrepo/logicalRepoApi.h>

namespace constraints {
namespace generator {
namespace generatorParts {

class GeneratorForProperties
{
public:
	static QPair<QString, QString> countPropertyCharacteristicForConstraintElement(
			const qReal::Id &constraint
			, const QString &characteristicName
			, const QString &defaultValue
			, const QString &property
			, QString sign
			, QString value
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, qReal::ErrorReporterInterface &errorReporter);

	static QString generateExistsProperty(
			const QString &resElementName
			, const QString &elementName
			, const qReal::Id &constraint
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api);
};

}
}
}
