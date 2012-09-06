#include "abstractSpinBox.h"

using namespace qReal::widgetsEdit;

AbstractSpinBox::AbstractSpinBox(QAbstractSpinBox *spinBox
		, ToolController *controller)
	: Tool(spinBox, controller)
{
	mAbstractSpinBox = spinBox;
}
