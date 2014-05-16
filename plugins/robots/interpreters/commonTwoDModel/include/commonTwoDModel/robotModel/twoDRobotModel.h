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

	utils::TimelineInterface &timeline() override;

protected:
	engine::TwoDModelEngineInterface *engine();

	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo
			) override;

private:
	engine::TwoDModelEngineInterface *mEngine = nullptr;  // Does not have ownership.
};

}
}
