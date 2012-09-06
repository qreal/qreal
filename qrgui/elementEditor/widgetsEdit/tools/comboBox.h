#pragma once

#include <QtGui/QComboBox>

#include "tool.h"

namespace qReal
{
namespace widgetsEdit
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
