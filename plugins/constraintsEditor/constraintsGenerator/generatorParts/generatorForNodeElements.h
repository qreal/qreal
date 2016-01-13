#pragma once

#include <QtCore/QString>

#include <qrkernel/ids.h>
#include <qrrepo/logicalRepoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorUtils/defs.h"

namespace constraints {
namespace generator {
namespace generatorParts {

class GeneratorForNodeElements
{
public:
	static QString countRealConstraintForNodeElement(const qReal::Id &constraintElement
			, const QString &elementName
			, const QString &resultName
			, const int depth
			, const QString &addStr
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter
			);

	static QPair<QString, QList<QString>> countRealConstraintForOneNodeElement(
			const qReal::Id &constraint
			, qReal::IdList &usedElements
			, const QString &elementName
			, const int depth
			, const QString &addStr, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter
			, const bool isMultiOr = false
			);

	static QPair<QString, QList<QString>> countConstraintForBeginNode(qReal::Id const &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter
			);

	static QPair<QString, QList<QString> > countConstraintForEndNode(
			qReal::Id const &constraint
			, const QString &elementName
			, int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter);

	static QPair<QString, QList<QString> > countConstraintForParent(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter);

	static QPair<QString, QList<QString> > countConstraintForPropertyNode(qReal::Id const &constraint
			, const QString &elementName
			, const int depth
			, const QString &addStr
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			);

	static QPair<QString, QList<QString> > countConstraintForMultiOrNode(
			const qReal::Id &constraint
			, qReal::IdList &usedElements
			, const metaType &type
			, const QString &elementName
			, const int depth
			, const QString &addStr
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> countsOfConstraintElementsInOneConstraint);

	static QPair<QString, QList<QString> > countNeighborsElementsByOr(
			const qReal::Id &constraint
			, const QString &resConstraintBool
			, qReal::IdList &usedElements
			, const metaType &type
			, const QString &elementName
			, const int depth
			, const QString &addStr
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter);
};

}
}
}
