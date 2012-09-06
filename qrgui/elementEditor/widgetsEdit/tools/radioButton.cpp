#include "radioButton.h"

using namespace qReal::widgetsEdit;

RadioButton::RadioButton(ToolController *controller)
	: AbstractButton(new QRadioButton("radioButton"), controller)
{
	mRadioButton = dynamic_cast<QRadioButton *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/radioButton.png");
	mTitle = "Radio Button";
	connect(mRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
}

void RadioButton::toggled(bool checked)
{
	emit propertyChanged("checked", QVariant(checked));
}
