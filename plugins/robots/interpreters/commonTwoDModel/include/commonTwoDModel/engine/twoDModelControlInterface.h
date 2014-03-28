#pragma once

#include <QtCore/QString>
#include <qrgui/toolPluginInterface/actionInfo.h>
#include <interpreterBase/sensorsConfigurationProvider.h>

namespace twoDModel {

class TwoDModelControlInterface : public interpreterBase::SensorsConfigurationProvider
{
public:
	virtual ~TwoDModelControlInterface() {}
	virtual qReal::ActionInfo &showTwoDModelWidgetActionInfo() = 0;
};

}
