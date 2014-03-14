#pragma once

#include <QtCore/QString>
#include <QtCore/QScopedPointer>

#include "commonTwoDModel/engine/twoDModelControlInterface.h"

namespace twoDModel {

class D2RobotModel;

class TwoDModelFacade : public TwoDModelControlInterface
{
public:
	TwoDModelFacade();
	~TwoDModelFacade();
	qReal::ActionInfo &showTwoDModelWidgetActionInfo() override;

private:
	qReal::ActionInfo mTwoDModelActionInfo;  // Has ownership over contained QAction object.
	QScopedPointer<D2RobotModel> mTwoDModel;
};

}
