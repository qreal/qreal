#pragma once

#include <QtCore/QString>

#include <interpreterBase/robotModel/commonRobotModel.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"
#include "commonTwoDModel/engine/twoDModelGuiFacade.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {

/// A robot model implementation common for all 2D model wrappers.
/// Accepts the given real robot model implementation proxying its devices, ports and convertible bases.
class COMMON_TWO_D_MODEL_EXPORT TwoDRobotModel : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	explicit TwoDRobotModel(interpreterBase::robotModel::RobotModelInterface const &realModel);

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
	utils::TimelineInterface &timeline() override;
	QList<interpreterBase::robotModel::PortInfo> configurablePorts() const override;
	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;

	/// Configures 2D model window`s engine for using it in 2D model devices emulators.
	void setEngine(engine::TwoDModelEngineInterface &engine);
	engine::TwoDModelEngineInterface *engine();

protected:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo
			) override;

private:
	interpreterBase::robotModel::RobotModelInterface const &mRealModel;
	engine::TwoDModelEngineInterface *mEngine = nullptr;  // Does not have ownership.
};

}
}
