#include "checkBox.h"

using namespace qReal::widgetsEdit;

CheckBox::CheckBox(ToolController *controller)
	: AbstractButton(new CheckBoxWidget("checkBox"), controller)
{
	mCheckBox = dynamic_cast<CheckBoxWidget *>(widget());
	mTitle = tr("Check Box");
	mTag = "CheckBox";
	mIcon = QIcon(":/icons/widgetsEditor/checkBox.png");
	mProxy = new CheckBoxProxy(mCheckBox);
}

CheckBoxProxy::CheckBoxProxy(CheckBoxWidget *checkBox)
	: AbstractButtonProxy(checkBox), mCheckBox(checkBox)
{
	connect(mCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
}

void CheckBoxProxy::toggled(bool checked)
{
	onPropertyChanged("checked", QVariant(checked));
}

bool CheckBoxProxy::isChecked() const
{
	return mCheckBox->isChecked();
}

void CheckBoxProxy::setChecked(bool checked)
{
	mCheckBox->setChecked(checked);
}

CheckBoxWidget::CheckBoxWidget(QString const &title)
	: QCheckBox(title), PropertyEditor(this)
	, mFormat(Boolean), mLastTrueInt(1)
{
	connect(this, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged()));
}

void CheckBoxWidget::onStateChanged()
{
	switch(mFormat) {
	case Boolean:
		setValueInRepo(isChecked() ? "true" : "false");
		break;
	case Numeric:
		setValueInRepo(isChecked() ? QString::number(mLastTrueInt) : "0");
		break;
	}
}

bool CheckBoxWidget::isChecked() const
{
	return checkState() == Qt::Checked;
}

void CheckBoxWidget::setPropertyValue(const QVariant &value)
{
	if (value.type() == QVariant::Bool) {
		mFormat = Boolean;
		setChecked(value.toBool());
	} else if (value.type() == QVariant::Int) {
		mFormat = Numeric;
		int intValue = value.toInt();
		mLastTrueInt = intValue != 0 ? intValue : mLastTrueInt;
		setChecked(intValue != 0);
	}
}
