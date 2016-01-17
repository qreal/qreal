/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
