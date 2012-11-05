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

QString CheckBox::bindedPropertyName() const
{
	return mCheckBox->propertyName();
}

void CheckBox::setBindedPropertyName(QString const &name)
{
	mCheckBox->setPropertyName(name);
}

CheckBoxWidget::CheckBoxWidget(QString const &title)
	: QCheckBox(title), PropertyEditor(this)
{
	connect(this, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));
}

void CheckBoxWidget::onStateChanged(int state)
{
	setCheckedState(state == Qt::Checked);
}

bool CheckBoxWidget::isChecked() const
{
	return checkState() == Qt::Checked;
}

void CheckBoxWidget::setCheckedState(bool checked)
{
	NodeElement *node = getNode();
	if (node && !propertyName().isEmpty()) {
		node->setLogicalProperty(propertyName(), checked ? "true" : "false");
	}
}

void CheckBoxWidget::setValue(const QString &value)
{
	setChecked(value.toLower() == "true" || value == "1");
}
