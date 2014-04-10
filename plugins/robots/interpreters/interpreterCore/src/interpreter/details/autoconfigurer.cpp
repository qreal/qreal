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

QHash<PortInfo, DeviceInfo> Autoconfigurer::configure(QList<qReal::Id> const &diagrams
		, RobotModelInterface &robotModel, bool &success)
{
	success = true;

	for (PortInfo const &port : robotModel.availablePorts()) {
		mUsedSensors[port] = currentConfiguration(robotModel.name(), port);
	}

	for (Id const &diagram : diagrams) {
		IdList const children = mGraphicalModelApi.graphicalRepoApi().children(diagram);

		for (Id const &child : children) {
			interpreterBase::blocksBase::BlockInterface const * const block = mBlocksTable.block(child);
			QMap<PortInfo, DeviceInfo> const usedSensors = block->usedSensors();
			foreach (PortInfo const &port, usedSensors.keys()) {
				DeviceInfo const device = usedSensors[port];
				if (!mUsedSensors[port].isNull() && mUsedSensors[port] != device) {
					mErrorReporter.addError(QObject::tr("Sensor configuration conflict, please check that sensor"\
							" ports are used consistently in a program"), child);
					success = false;
				} else if (mUsedSensors[port].isNull()) {
					mUsedSensors[port] = device;
				}
			}
		}
	}

	return QHash<PortInfo, DeviceInfo>();
}
