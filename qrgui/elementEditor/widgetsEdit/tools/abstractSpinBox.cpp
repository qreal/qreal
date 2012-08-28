#include "abstractSpinBox.h"

using namespace Ui::WidgetsEdit;

AbstractSpinBox::AbstractSpinBox(QAbstractSpinBox *spinBox
		, ToolController *controller)
	: Tool(spinBox, controller)
{
	mAbstractSpinBox = spinBox;
}
