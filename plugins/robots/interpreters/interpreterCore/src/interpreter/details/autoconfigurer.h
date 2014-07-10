#pragma once

#include <interpreterBase/devicesConfigurationProvider.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "blocksTable.h"

namespace interpreterCore {
namespace interpreter {
namespace details {

/// Extended devices configuration validator. Travels through the diagram and compares
/// required by blocks devices with the user configuration. If conflict is detected
/// error is reported. If some block requires a device that is forgotten to be selected by
/// user then it is automaticly selected by this autoconfigurer.
class Autoconfigurer : public interpreterBase::DevicesConfigurationProvider
{
public:
	/// Constructor.
	/// @param graphicalModelApi - contains information about diagram.
	/// @param blocksTable - contains "code-behind" objects for blocks.
	/// @param errorReporter - error reporter to report errors to.
	Autoconfigurer(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, BlocksTable &blocksTable
			, qReal::ErrorReporterInterface &errorReporter);

	/// Performs extended validation of devices correspondence to the user-defined configuration.
	/// May display errors or automaticly modify sensors configuration.
	/// @returns false if two configurations have conflicts and true if everything is ok.
	bool configure(QList<qReal::Id> const &diagrams, QString const &robotModelName);

private:
	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	BlocksTable &mBlocksTable;
	qReal::ErrorReporterInterface &mErrorReporter;
};

}
}
}
