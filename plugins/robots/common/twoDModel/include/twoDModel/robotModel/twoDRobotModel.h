#pragma once

#include <QtCore/QString>

#include <kitBase/robotModel/commonRobotModel.h>

#include "twoDModel/twoDModelDeclSpec.h"
#include "twoDModel/engine/twoDModelDisplayWidget.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {

/// A robot model implementation common for all 2D model wrappers.
/// Accepts the given real robot model implementation proxying its devices, ports and convertible bases.
class TWO_D_MODEL_EXPORT TwoDRobotModel : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	explicit TwoDRobotModel(const RobotModelInterface &realModel);
	explicit TwoDRobotModel(const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
	utils::TimelineInterface &timeline() override;
	QList<kitBase::robotModel::PortInfo> configurablePorts() const override;
	QList<kitBase::robotModel::DeviceInfo> convertibleBases() const override;
	int priority() const override;

	/// Configures 2D model window`s engine for using it in 2D model devices emulators.
	void setEngine(engine::TwoDModelEngineInterface &engine);

	/// @todo move physical constants here

	/// Returns a path to an image that is used for robot item.
	virtual QString robotImage() const = 0;

	/// Returns a port that is used for left robot wheel by default.
	virtual kitBase::robotModel::PortInfo defaultLeftWheelPort() const = 0;

	/// Returns a port that is used for right robot wheel by default.
	virtual kitBase::robotModel::PortInfo defaultRightWheelPort() const = 0;

	/// Returns a pointer to a widget with display emulator for current robot model.
	virtual engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const = 0;

	/// Provides path to sensor image by given device type or empty string if default sensor image shall be used.
	virtual QString sensorImagePath(const kitBase::robotModel::DeviceInfo &deviceType) const
	{
		Q_UNUSED(deviceType)

		return "";
	}

	/// Provides sensor image bounding rectangle by given device type or empty rectangle if default image size be used.
	virtual QRect sensorImageRect(const kitBase::robotModel::DeviceInfo &deviceType) const
	{
		Q_UNUSED(deviceType)

		return QRect();
	}

protected:
	engine::TwoDModelEngineInterface *engine();

	kitBase::robotModel::robotParts::Device *createDevice(
			const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo
			) override;

	virtual kitBase::robotModel::DeviceInfo markerInfo() const;

private:
	const kitBase::robotModel::RobotModelInterface *mRealModel;
	engine::TwoDModelEngineInterface *mEngine = nullptr;  // Does not have ownership.
};

}
}
