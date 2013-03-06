#include "autoconfigurer.h"

#include "blocks/block.h"
#include "../sensorConstants.h"
#include "tracer.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details;

Autoconfigurer::Autoconfigurer(GraphicalModelAssistInterface const &graphicalModelApi
							   , BlocksTable * const blocksTable
							   , ErrorReporterInterface * const errorReporter
							   , RobotModel *robotModel
							   )
	: mGraphicalModelApi(&graphicalModelApi)
	, mBlocksTable(blocksTable)
	, mErrorReporter(errorReporter)
	, mRobotModel(robotModel)
{
	mUsedSensors.resize(4);
}

bool Autoconfigurer::configure(Id const &diagram)
{
	bool result = true;

	mUsedSensors[0] = static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType").toInt());
	mUsedSensors[1] = static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType").toInt());
	mUsedSensors[2] = static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType").toInt());
	mUsedSensors[3] = static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType").toInt());

	IdList const children = mGraphicalModelApi->graphicalRepoApi().children(diagram);

	foreach (Id const &child, children) {
		blocks::Block const * const block = mBlocksTable->block(child);
		foreach (blocks::Block::SensorPortPair const &sensorPortPair, block->usedSensors()) {
			sensorType::SensorTypeEnum const sensor = sensorPortPair.first;
			int const port = sensorPortPair.second;
			if (mUsedSensors[port] != sensorType::unused && mUsedSensors[port] != sensor) {
				mErrorReporter->addError(QObject::tr("Sensor configuration conflict, please check that sensor ports are used consistently in a program"), child);
				result = false;
			} else if (mUsedSensors[port] == sensorType::unused) {
				mUsedSensors[port] = sensor;
			}
		}
	}

	if (result) {
		Tracer::debug(tracer::initialization, "Autoconfigurer::configure", "Autoconfiguring sensors in a model");
		mRobotModel->configureSensors(
				mUsedSensors[0]
				, mUsedSensors[1]
				, mUsedSensors[2]
				, mUsedSensors[3]
			);
	}

	return result;
}
