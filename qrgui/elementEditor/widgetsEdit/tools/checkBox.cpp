#include "checkBox.h"

using namespace qReal::widgetsEdit;

CheckBox::CheckBox(ToolController *controller)
	: AbstractButton(new CheckBoxWidget("checkBox"), controller)
{
	mCheckBox = dynamic_cast<CheckBoxWidget *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/checkBox.png");
	mTitle = "Check Box";
	connect(mCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
}

void CheckBox::toggled(bool checked)
{
	emit propertyChanged("checked", QVariant(checked));
}

bool CheckBox::isChecked() const
{
	return mCheckBox->isChecked();
}

void CheckBox::setChecked(bool checked)
{
	mCheckBox->setChecked(checked);
}

CheckBoxWidget::CheckBoxWidget(QString const &title)
	: QCheckBox(title), PropertyEditor(this)
{
	connect(this, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));
}

void CheckBoxWidget::onStateChanged(int state)
{
	setValueInRepo(state == Qt::Checked ? "true" : "false");
}

bool CheckBoxWidget::isChecked() const
{
	return checkState() == Qt::Checked;
}

void CheckBoxWidget::setPropertyValue(const QString &value)
{
	setChecked(value.toLower() == "true" || value == "1");
}
