#pragma once

#include <QtGui/QComboBox>

#include "tool.h"

namespace Ui
{
namespace WidgetsEdit
{

class ComboBox : public Tool
{
public:
	ComboBox(ToolController *controller);

private:
	QComboBox *mComboBox;
};

}
}
