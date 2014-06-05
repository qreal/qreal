#pragma once

#include <QtCore/QString>

#include <interpreterBase/robotModel/portInfo.h>

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

	/// @todo sensor images here

	/// Returns a pointer to a widget with display emulator for current robot model.
	virtual engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const = 0;
};

}
