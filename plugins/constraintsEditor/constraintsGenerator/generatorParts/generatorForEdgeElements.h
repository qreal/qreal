#pragma once

#include <QtCore/QString>

#include <qrkernel/ids.h>
#include <qrrepo/logicalRepoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorUtils/defs.h"

namespace constraints {
namespace generator {
namespace generatorParts {

class GeneratorForEdgeElements
{
public:
	static QString countRealConstraintForEdgeElement(const qReal::Id &constraintElement
			, const QString &elementName
			, const QString &resultName
			, const int depth
			, const QString &addStr
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter);

	static QPair<QString, QList<QString>> countRealConstraintForOneEdgeElement(const qReal::Id &constraint
			, qReal::IdList &usedElements
			, const QString &elementName
			, const int depth
			, const QString &addStr
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter
			, const bool isMultiOr = false);
};

}
}
}
