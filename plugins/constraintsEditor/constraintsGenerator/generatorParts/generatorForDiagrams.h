#pragma once

#include <QtCore/QString>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrrepo/logicalRepoApi.h>

namespace constraints {
namespace generator {
namespace generatorParts {

class GeneratorForDiagrams
{
public:
	static QString countRealConstraintOfDiagramElement(const qReal::Id &constraintElement
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter);

private:
	static QPair<bool, QString> handleConstraintsSelection(
			const qReal::Id &constraintElement
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter);
};

}
}
}
