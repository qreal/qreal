#include "spinBox.h"

using namespace qReal::widgetsEdit;

SpinBox::SpinBox(ToolController *controller)
	: AbstractSpinBox(new SpinBoxWidget, controller)
{
	mSpinBox = dynamic_cast<SpinBoxWidget *>(widget());
	mTitle = tr("Spin Box");
	mTag = "SpinBox";
	mIcon = QIcon(":/icons/widgetsEditor/spinBox.png");
	connect(mSpinBox, SIGNAL(valueChanged(int))
		, this, SLOT(valueChanged(int)));
}

void SpinBox::valueChanged(int i)
{
	emit propertyChanged("value", QVariant(i));
}

int SpinBox::maximum() const
{
	return mSpinBox->maximum();
}

int SpinBox::minimum() const
{
	return mSpinBox->minimum();
}

QString SpinBox::prefix() const
{
	return mSpinBox->prefix();
}

QString SpinBox::suffix() const
{
	return mSpinBox->suffix();
}

int SpinBox::singleStep() const
{
	return mSpinBox->singleStep();
}

int SpinBox::value() const
{
	return mSpinBox->value();
}

void SpinBox::setMaximum(int maximum)
{
	mSpinBox->setMaximum(maximum);
}

void SpinBox::setMinimum(int minimum)
{
	mSpinBox->setMinimum(minimum);
}

void SpinBox::setPrefix(QString const &prefix)
{
	mSpinBox->setPrefix(prefix);
}

void SpinBox::setSuffix(QString const &suffix)
{
	mSpinBox->setSuffix(suffix);
}

void SpinBox::setSingleStep(int step)
{
	mSpinBox->setSingleStep(step);
}

void SpinBox::setValue(int value)
{
	mSpinBox->setValue(value);
}

SpinBoxWidget::SpinBoxWidget()
	: QSpinBox(), PropertyEditor(this)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged1(int)));
}

void SpinBoxWidget::setPropertyValue(const QString &value)
{
	bool ok = false;
	int val = value.toInt(&ok);
	if (ok) {
		QSpinBox::setValue(val);
	}
}

void SpinBoxWidget::onValueChanged1(int value)
{
	setValueInRepo(QString::number(value));
}
