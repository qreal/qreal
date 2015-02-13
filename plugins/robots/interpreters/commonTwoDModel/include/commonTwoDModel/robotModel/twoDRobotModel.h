#pragma once

#include <QtCore/QString>

#include <interpreterBase/robotModel/commonRobotModel.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"
#include "commonTwoDModel/engine/twoDModelGuiFacade.h"
#include "commonTwoDModel/engine/twoDModelDisplayWidget.h"

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
	explicit TwoDRobotModel(const RobotModelInterface &realModel);
	TwoDRobotModel(const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
	utils::TimelineInterface &timeline() override;
	QList<interpreterBase::robotModel::PortInfo> configurablePorts() const override;
	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;

	/// Configures 2D model window`s engine for using it in 2D model devices emulators.
	void setEngine(engine::TwoDModelEngineInterface &engine);

	/// @todo move physical constants here

	/// Returns a path to an image that is used for robot item.
	virtual QString robotImage() const = 0;

	/// Returns a port that is used for left robot wheel by default.
	virtual interpreterBase::robotModel::PortInfo defaultLeftWheelPort() const = 0;

	/// Returns a port that is used for right robot wheel by default.
	virtual interpreterBase::robotModel::PortInfo defaultRightWheelPort() const = 0;

	/// Returns a pointer to a widget with display emulator for current robot model.
	virtual engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const = 0;

	/// Provides path to sensor image by given device type or empty string if default sensor image shall be used.
	virtual QString sensorImagePath(const interpreterBase::robotModel::DeviceInfo &deviceType) const
	{
		Q_UNUSED(deviceType)

		return "";
	}

	/// Provides sensor image bounding rectangle by given device type or empty rectangle if default image size be used.
	virtual QRect sensorImageRect(const interpreterBase::robotModel::DeviceInfo &deviceType) const
	{
		Q_UNUSED(deviceType)

		return QRect();
	}

	engine::TwoDModelEngineInterface *engine();

protected:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			const interpreterBase::robotModel::PortInfo &port
			, const interpreterBase::robotModel::DeviceInfo &deviceInfo
			) override;

	virtual interpreterBase::robotModel::DeviceInfo markerInfo() const;

private:
	const interpreterBase::robotModel::RobotModelInterface *mRealModel;
	engine::TwoDModelEngineInterface *mEngine = nullptr;  // Does not have ownership.
};

}
}
