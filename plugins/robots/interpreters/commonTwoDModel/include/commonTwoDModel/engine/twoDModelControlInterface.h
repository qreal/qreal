#pragma once

#include <QtCore/QString>
#include <qrgui/toolPluginInterface/actionInfo.h>

namespace twoDModel {

class TwoDModelControlInterface
{
public:
	virtual ~TwoDModelControlInterface() {}
	virtual qReal::ActionInfo &showTwoDModelWidgetActionInfo() = 0;
};

}
