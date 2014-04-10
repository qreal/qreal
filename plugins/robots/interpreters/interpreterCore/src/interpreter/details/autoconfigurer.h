#pragma once

#include <interpreterBase/robotModel/robotModelInterface.h>
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
	/// @param success Output parameter that will be stored false if two configurations have conflicts
	///        and true if everything is ok.
	/// @return The devices configuration desired by blocks on diagram
	QHash<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> configure(
			QList<qReal::Id> const &diagrams
			, interpreterBase::robotModel::RobotModelInterface &robotModel
			, bool &success);

private:
	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	BlocksTable &mBlocksTable;
	qReal::ErrorReporterInterface &mErrorReporter;
	QHash<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> mUsedSensors;
};

}
}
}
