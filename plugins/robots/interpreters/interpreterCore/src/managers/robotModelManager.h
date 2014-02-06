#pragma once

#include <QtCore/QObject>

#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

#include "src/defaultRobotModel.h"

namespace interpreterCore {

/// Contains a reference to currently selected robot model and provides a notification when it changes.
/// This is the only place that has a valid up-to-date information about robot model, all other components shall
/// use it as a reference.
class RobotModelManager : public interpreterBase::robotModel::RobotModelManagerInterface
{
	Q_OBJECT

public:
	RobotModelManager();

	interpreterBase::robotModel::RobotModelInterface &model() const override;

	/// Changes currently selected robot model and notifies all about change by robotModelChanged signal.
	void setModel(interpreterBase::robotModel::RobotModelInterface * const robotModel);

private:
	interpreterBase::robotModel::RobotModelInterface *mRobotModel;  // Does not have ownership.
	DefaultRobotModel mDefaultRobotModel;
};

}
