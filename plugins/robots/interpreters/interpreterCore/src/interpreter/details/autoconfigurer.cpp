#include "interpreterCore/interpreter/details/autoconfigurer.h"

#include <qrkernel/exception/exception.h>

#include <interpreterBase/blocksBase/robotsBlock.h>
#include <interpreterBase/robotModel/robotModelInterface.h>
#include <utils/tracer.h>

using namespace interpreterCore::interpreter::details;
using namespace qReal;
using namespace interpreterBase::robotModel;

Autoconfigurer::Autoconfigurer(const GraphicalModelAssistInterface &graphicalModelApi
		, BlocksTable &blocksTable
		, qReal::ErrorReporterInterface &errorReporter)
	: mGraphicalModelApi(graphicalModelApi)
	, mBlocksTable(blocksTable)
	, mErrorReporter(errorReporter)
{
}

bool Autoconfigurer::configure(QList<qReal::Id> const &diagrams, const QString &robotModelName)
{
	for (const Id &diagram : diagrams) {
		const IdList children = mGraphicalModelApi.graphicalRepoApi().children(diagram);

		for (const Id &child : children) {
			interpreterBase::blocksBase::RobotsBlock * const block
					= dynamic_cast<interpreterBase::blocksBase::RobotsBlock *>(mBlocksTable.block(child));
			if (!block) {
				continue;
			}

			QMap<PortInfo, DeviceInfo> const usedDevices = block->usedDevices();
			for (const PortInfo &port : usedDevices.keys()) {
				const DeviceInfo device = usedDevices[port];
				const DeviceInfo existingDevice = currentConfiguration(robotModelName, port);
				if (!existingDevice.isNull() && !existingDevice.isA(device)) {
					mErrorReporter.addError(QObject::tr("Sensor configuration conflict, please check that sensor"\
							" ports are used consistently in a program"), child);
					return false;
				} else if (existingDevice.isNull()) {
					/// @todo: Do it loudly, user must notice it
					deviceConfigurationChanged(robotModelName, port, device, Reason::automaticConfiguration);
				}
			}
		}
	}

	return true;
}
