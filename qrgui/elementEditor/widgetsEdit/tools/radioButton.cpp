#include "radioButton.h"

using namespace qReal::widgetsEdit;

RadioButton::RadioButton(ToolController *controller)
	: AbstractButton(new QRadioButton("radioButton"), controller)
{
	mRadioButton = dynamic_cast<QRadioButton *>(widget());
	mTitle = tr("Radio Button");
	mTag = "RadioButton";
	mIcon = QIcon(":/icons/widgetsEditor/radioButton.png");
	mProxy = new RadioButtonProxy(mRadioButton);
}

RadioButtonProxy::RadioButtonProxy(QRadioButton *radioButton)
	: AbstractButtonProxy(radioButton), mRadioButton(radioButton)
{
	connect(mRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
}

void RadioButtonProxy::toggled(bool checked)
{
	onPropertyChanged("checked", QVariant(checked));
}

bool RadioButtonProxy::isChecked() const
{
	return mRadioButton->isChecked();
}

void RadioButtonProxy::setChecked(bool checked)
{
	mRadioButton->setChecked(checked);
}
