#pragma once

#include <QtGui/QRadioButton>

#include "tool.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class RadioButton : public Tool
{
	Q_OBJECT

public:
	RadioButton(ToolController *controller);

private slots:
	void toggled(bool checked);

private:
	QRadioButton *mRadioButton;

};

}
}
