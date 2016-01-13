#pragma once

#include <QtCore/QString>

#include <qrkernel/ids.h>
#include <qrrepo/logicalRepoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorUtils/defs.h"

namespace constraints {
namespace generator {
namespace generatorParts {

class GeneratorForElements
{
public:
	static QString countRealConstraintForElement(
			const qReal::Id &constraintElement
			, const metaType &type
			, const QString &elementName
			, const QString &resultName
			, const int depth
			, const QString &addStr
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter
			);

private:
	static QString additionalCommonPartForConstraint(
			const QList<QString> &resBool
			, const QString &resultName
			, const int depth
			, const QString &addStr);
};


}
}
}
