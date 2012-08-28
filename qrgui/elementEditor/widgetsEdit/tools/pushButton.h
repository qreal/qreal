#pragma once

#include <QtGui/QPushButton>

#include "tool.h"
#include "../private/toolController.h"

namespace Ui
{
namespace WidgetsEdit
{

class PushButton : public Tool
{
public:
	PushButton(ToolController *controller);

private:
	QPushButton *mPushButton;
};

}
}
