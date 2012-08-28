#pragma once

#include <QtGui/QGroupBox>

#include "layoutTool.h"

namespace Ui
{
namespace WidgetsEdit
{

int const GROUP_BOX_DEFAULT_WIDTH = 150;
int const GROUP_BOX_DEFAULT_HEIGHT = 150;

class GroupBox : public LayoutTool
{
public:
	GroupBox(ToolController *controller);

private:
	QGroupBox *mGroupBox;

};

}
}
