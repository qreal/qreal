#pragma once

#include <QtCore/QString>
#include <QtCore/QScopedPointer>

#include <interpreterBase/robotModel/robotModelInterface.h>

#include "commonTwoDModel/engine/twoDModelControlInterface.h"

namespace twoDModel {

class D2RobotModel;

class TwoDModelFacade : public TwoDModelControlInterface
{
public:
	TwoDModelFacade(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~TwoDModelFacade();
	qReal::ActionInfo &showTwoDModelWidgetActionInfo() override;
	interpreterBase::SensorsConfigurationProvider &sensorsConfigurationProvider() override;

private:
	qReal::ActionInfo mTwoDModelActionInfo;  // Has ownership over contained QAction object.
	QScopedPointer<D2RobotModel> mTwoDModel;
};

}
