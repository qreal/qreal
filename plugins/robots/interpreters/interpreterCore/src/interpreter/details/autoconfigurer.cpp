#include "autoconfigurer.h"

#include <qrkernel/exception/exception.h>

#include <interpreterBase/baseBlocks/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>
//#include "tracer.h"

using namespace interpreterCore::interpreter::details;

using namespace qReal;
using namespace interpreterBase::robotModel;

Autoconfigurer::Autoconfigurer(GraphicalModelAssistInterface const &graphicalModelApi
		, BlocksTable * const blocksTable
		, ErrorReporterInterface * const errorReporter
		, RobotModelInterface *robotModel
		)
		: mGraphicalModelApi(&graphicalModelApi)
		, mBlocksTable(blocksTable)
		, mErrorReporter(errorReporter)
		, mRobotModel(robotModel)
{
}

//QVector<SensorId> Autoconfigurer::configure(Id const &diagram, bool *success)
//{
//	if (!success) {
//		throw Exception("Pass correct pointer to boolean as 'success' parameter");
//	}

//	*success = true;

//	mUsedSensors[0] = static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType").toInt());
//	mUsedSensors[1] = static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType").toInt());
//	mUsedSensors[2] = static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType").toInt());
//	mUsedSensors[3] = static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType").toInt());

//	IdList const children = mGraphicalModelApi->graphicalRepoApi().children(diagram);

//	foreach (Id const &child, children) {
//		blocks::Block const * const block = mBlocksTable->block(child);
//		foreach (blocks::Block::SensorPortPair const &sensorPortPair, block->usedSensors()) {
//			robots::enums::sensorType::SensorTypeEnum const sensor = sensorPortPair.first;
//			int const port = sensorPortPair.second;
//			if (mUsedSensors[port] != robots::enums::sensorType::unused && mUsedSensors[port] != sensor) {
//				mErrorReporter->addError(QObject::tr("Sensor configuration conflict, please check that sensor ports are used consistently in a program"), child);
//				result = false;
//			} else if (mUsedSensors[port] == robots::enums::sensorType::unused) {
//				mUsedSensors[port] = sensor;
//			}
//		}
//	}

//	if (result) {
//		Tracer::debug(tracer::enums::initialization, "Autoconfigurer::configure", "Autoconfiguring sensors in a model");
//		mRobotModel->configureSensors(
//				mUsedSensors[0]
//				, mUsedSensors[1]
//				, mUsedSensors[2]
//				, mUsedSensors[3]
//			);
//	}

//	return QVector<SensorId>();
//}
