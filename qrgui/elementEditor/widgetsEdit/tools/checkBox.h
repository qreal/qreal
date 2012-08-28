#pragma once

#include <QtGui/QCheckBox>

#include "tool.h"
#include "../private/toolController.h"

namespace Ui
{
namespace WidgetsEdit
{

class CheckBox : public Tool
{
	Q_OBJECT

public:
	CheckBox(ToolController *controller);

private slots:
	void toggled(bool checked);

private:
	QCheckBox *mCheckBox;

};

}
}
