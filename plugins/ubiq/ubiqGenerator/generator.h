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

#include <QtCore/QObject>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace ubiq {
namespace generator {

/// Main class of UbiqMobile generator, directs generation process.
class Generator : public QObject
{
	Q_OBJECT

public:
	Generator();
	virtual ~Generator();

	/// Initializes generator with logical model reference and means to report errors.
	void init(qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

public slots:
	/// Starts generation process.
	void generate();

private:
	/// Logical model, from which generator takes all information.
	qReal::LogicalModelAssistInterface const *mLogicalModel;

	/// Error reporter object, used to show generation errors in output window.
	qReal::ErrorReporterInterface *mErrorReporter;
};

}
}
