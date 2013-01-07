#include "radioButton.h"

using namespace qReal::widgetsEdit;

RadioButton::RadioButton(ToolController *controller)
	: AbstractButton(new QRadioButton("radioButton"), controller)
{
	mRadioButton = dynamic_cast<QRadioButton *>(widget());
	mTitle = tr("Radio Button");
	mTag = "RadioButton";
	mIcon = QIcon(":/icons/widgetsEditor/radioButton.png");
	connect(mRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
}

void RadioButton::toggled(bool checked)
{
	emit propertyChanged("checked", QVariant(checked));
}

bool RadioButton::isChecked() const
{
	return mRadioButton->isChecked();
}

void RadioButton::setChecked(bool checked)
{
	mRadioButton->setChecked(checked);
}
