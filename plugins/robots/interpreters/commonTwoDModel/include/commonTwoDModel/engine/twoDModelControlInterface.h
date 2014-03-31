#pragma once

#include <QtCore/QString>
#include <qrgui/toolPluginInterface/actionInfo.h>
#include <interpreterBase/sensorsConfigurationProvider.h>

namespace twoDModel {

class TwoDModelControlInterface
{
public:
	virtual ~TwoDModelControlInterface() {}
	virtual qReal::ActionInfo &showTwoDModelWidgetActionInfo() = 0;
	virtual interpreterBase::SensorsConfigurationProvider &sensorsConfigurationProvider() = 0;
};

}
