#include "checkBox.h"

using namespace Ui::WidgetsEdit;

CheckBox::CheckBox(ToolController *controller)
	: Tool(new QCheckBox("checkBox"), controller)
{
	mCheckBox = dynamic_cast<QCheckBox *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/checkBox.png");
	mTitle = "Check Box";
	connect(mCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
}

void CheckBox::toggled(bool checked)
{
	emit propertyChanged("checked", QVariant(checked));
}
