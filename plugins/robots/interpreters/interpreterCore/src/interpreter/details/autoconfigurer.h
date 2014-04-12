#pragma once

#include <interpreterBase/devicesConfigurationProvider.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "blocksTable.h"

namespace interpreterCore {
namespace interpreter {
namespace details {

/// Extended devices configuration validator.
class Autoconfigurer : public interpreterBase::DevicesConfigurationProvider
{
public:
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
