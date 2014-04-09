#pragma once

#include <QtCore/QString>
#include <qrgui/toolPluginInterface/actionInfo.h>
#include <interpreterBase/devicesConfigurationProvider.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

class COMMON_TWO_D_MODEL_EXPORT TwoDModelControlInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~TwoDModelControlInterface() {}
	virtual qReal::ActionInfo &showTwoDModelWidgetActionInfo() = 0;
	virtual interpreterBase::DevicesConfigurationProvider &devicesConfigurationProvider() = 0;
	virtual void init() = 0;

public slots:
	virtual void onStartInterpretation() = 0;
	virtual void onStopInterpretation() = 0;

signals:
	void runButtonPressed();
	void stopButtonPressed();
};

}
