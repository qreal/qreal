#include "autoconfigurer.h"

#include <qrkernel/exception/exception.h>

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>
#include <utils/tracer.h>

using namespace interpreterCore::interpreter::details;
using namespace qReal;
using namespace interpreterBase::robotModel;

Autoconfigurer::Autoconfigurer(GraphicalModelAssistInterface const &graphicalModelApi
		, BlocksTable &blocksTable
		, qReal::ErrorReporterInterface &errorReporter)
	: mGraphicalModelApi(graphicalModelApi)
	, mBlocksTable(blocksTable)
	, mErrorReporter(errorReporter)
{
}

bool Autoconfigurer::configure(QList<qReal::Id> const &diagrams, QString const &robotModelName)
{
	for (Id const &diagram : diagrams) {
		IdList const children = mGraphicalModelApi.graphicalRepoApi().children(diagram);

		for (Id const &child : children) {
			interpreterBase::blocksBase::BlockInterface const * const block = mBlocksTable.block(child);
			if (!block) {
				continue;
			}

			QMap<PortInfo, DeviceInfo> const usedSensors = block->usedSensors();
			for (PortInfo const &port : usedSensors.keys()) {
				DeviceInfo const device = usedSensors[port];
				DeviceInfo const existingDevice = currentConfiguration(robotModelName, port);
				if (!existingDevice.isNull() && existingDevice != device) {
					mErrorReporter.addError(QObject::tr("Sensor configuration conflict, please check that sensor"\
							" ports are used consistently in a program"), child);
					return false;
				} else if (existingDevice.isNull()) {
					/// @todo: Do it loudly, user must notice it
					deviceConfigurationChanged(robotModelName, port, device);
				}
			}
		}
	}

	return true;
}
