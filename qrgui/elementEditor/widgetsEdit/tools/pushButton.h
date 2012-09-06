#pragma once

#include <QtGui/QPushButton>

#include "tool.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
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
