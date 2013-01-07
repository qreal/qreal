#include "checkBox.h"

using namespace qReal::widgetsEdit;

CheckBox::CheckBox(ToolController *controller)
	: AbstractButton(new CheckBoxWidget("checkBox"), controller)
{
	mCheckBox = dynamic_cast<CheckBoxWidget *>(widget());
	mTitle = tr("Check Box");
	mTag = "CheckBox";
	mIcon = QIcon(":/icons/widgetsEditor/checkBox.png");
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

void CheckBoxWidget::setPropertyValue(const QString &value)
{
	QString const lowerValue = value.toLower();
	if (lowerValue == "true" || lowerValue == "false") {
		mFormat = Boolean;
		setChecked(lowerValue == "true");
	} else {
		bool ok;
		int intValue = value.toInt(&ok);
		if (ok) {
			mFormat = Numeric;
			mLastTrueInt = intValue != 0 ? intValue : mLastTrueInt;
			setChecked(intValue != 0);
		}
	}
}
