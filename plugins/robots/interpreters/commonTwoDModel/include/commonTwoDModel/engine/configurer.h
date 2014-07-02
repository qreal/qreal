#pragma once

#include <QtCore/QString>

#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/deviceInfo.h>

#include "commonTwoDModel/engine/twoDModelDisplayWidget.h"
#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

/// Can be used for customizing different 2D emulator aspects in concrete interpreter
class COMMON_TWO_D_MODEL_EXPORT Configurer
{
public:
	virtual ~Configurer() {}

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
	virtual QString sensorImagePath(interpreterBase::robotModel::DeviceInfo const &deviceType) const
	{
		Q_UNUSED(deviceType)

		return "";
	}

	/// Provides sensor image bounding rectangle by given device type or empty rectangle if default image size be used.
	virtual QRect sensorImageRect(interpreterBase::robotModel::DeviceInfo const &deviceType) const
	{
		Q_UNUSED(deviceType)

		return QRect();
	}
};

}
