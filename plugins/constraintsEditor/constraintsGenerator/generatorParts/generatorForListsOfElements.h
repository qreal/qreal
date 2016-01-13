#pragma once

#include <QtCore/QString>
#include <QtCore/QPair>

#include <qrkernel/ids.h>
#include <qrrepo/logicalRepoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace constraints {
namespace generator {
namespace generatorParts {

class GeneratorForListsOfElements
{
public:
	static QPair<QString, QList<QString> > countConstraintForChildrens(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

	static QPair<QString, QList<QString> > countConstraintForOutgoingLinks(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

	static QPair<QString, QList<QString> > countConstraintForIncomingLinks(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

	static QPair<QString, QList<QString> > countConstraintForOutgoingNodes(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

	static QPair<QString, QList<QString> > countConstraintForIncomingNodes(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

private:
	static QPair<QString, QList<QString> > countConstraintForListOfElements(const qReal::Id &constraint
			, const QString &elementName
			, const QString &resElementName
			, const QString &functionName
			, const QString &resType
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);
};

}
}
}
