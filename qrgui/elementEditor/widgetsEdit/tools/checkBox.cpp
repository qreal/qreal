#include "checkBox.h"

using namespace qReal::widgetsEdit;

CheckBox::CheckBox(ToolController *controller)
	: AbstractButton(new QCheckBox("checkBox"), controller)
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

bool CheckBox::checked() const
{
	return mCheckBox->checkState() == Qt::Checked;
}

void CheckBox::setChecked(bool checked)
{
	mCheckBox->setChecked(checked);
}
