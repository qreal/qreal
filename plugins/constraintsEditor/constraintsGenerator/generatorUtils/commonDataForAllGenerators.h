#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>

#include <qrrepo/logicalRepoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrkernel/ids.h>

namespace constraints {
namespace generator {
namespace generatorUtils {

class CommonDataForAllGenerators
{
public:
	CommonDataForAllGenerators(
			const qReal::Id &constraintId
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

private:
	const qrRepo::LogicalRepoApi &mApi;
	const qReal::Id mId;
	qReal::ErrorReporterInterface &mErrorReporter;
	QMap<QString, int> mCountsOfConstraintElementsInOneConstraint;
};

}
}
}
