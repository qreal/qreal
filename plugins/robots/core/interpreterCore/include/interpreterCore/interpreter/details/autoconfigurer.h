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

#include <kitBase/devicesConfigurationProvider.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "blocksTable.h"

namespace interpreterCore {
namespace interpreter {
namespace details {

/// Extended devices configuration validator. Travels through the diagram and compares
/// required by blocks devices with the user configuration. If conflict is detected
/// error is reported. If some block requires a device that is forgotten to be selected by
/// user then it is automaticly selected by this autoconfigurer.
class Autoconfigurer : public kitBase::DevicesConfigurationProvider
{
public:
	/// Constructor.
	/// @param graphicalModelApi - contains information about diagram.
	/// @param blocksTable - contains "code-behind" objects for blocks.
	/// @param errorReporter - error reporter to report errors to.
	Autoconfigurer(const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, BlocksTable &blocksTable
			, qReal::ErrorReporterInterface &errorReporter);

	/// Performs extended validation of devices correspondence to the user-defined configuration.
	/// May display errors or automaticly modify sensors configuration.
	/// @returns false if two configurations have conflicts and true if everything is ok.
	bool configure(QList<qReal::Id> const &diagrams, const QString &robotModelName);

private:
	const qReal::GraphicalModelAssistInterface &mGraphicalModelApi;
	BlocksTable &mBlocksTable;
	qReal::ErrorReporterInterface &mErrorReporter;
};

}
}
}
