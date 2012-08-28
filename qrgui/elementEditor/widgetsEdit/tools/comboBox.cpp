#include "comboBox.h"

using namespace Ui::WidgetsEdit;

ComboBox::ComboBox(ToolController *controller)
	: Tool(new QComboBox, controller)
{
	mComboBox = dynamic_cast<QComboBox *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/comboBox.png");
	mTitle = "Combo Box";
}
