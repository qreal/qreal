#pragma once

#include <QtCore/QString>

#include <interpreterBase/robotModel/commonRobotModel.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {

class COMMON_TWO_D_MODEL_EXPORT TwoDRobotModel : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	TwoDRobotModel();

	bool needsConnection() const override;

	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;

	void setEngine(engine::TwoDModelEngineInterface &engine);

	utils::AbstractTimer *produceTimer() override;

//protected:
//	virtual interpreterBase::robotModel::DeviceInfo displayInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo speakerInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo buttonsInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo motorInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo encoderInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo touchSensorInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo sonarSensorInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo lightSensorInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo colorSensorInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo soundSensorInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo gyroscopeSensorInfo() const;
//	virtual interpreterBase::robotModel::DeviceInfo accelerometerSensorInfo() const;

private:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo
			) override;

	engine::TwoDModelEngineInterface *mEngine;  // Does not have ownership.
};

}
}
