#include "doubleSpinBox.h"

using namespace qReal::widgetsEdit;

DoubleSpinBox::DoubleSpinBox(ToolController *controller)
	: AbstractSpinBox(new DoubleSpinBoxWidget, controller)
{
	mDoubleSpinBox = dynamic_cast<DoubleSpinBoxWidget *>(widget());
	mTitle = tr("Double Spin Box");
	mTag = "DoubleSpinBox";
	mIcon = QIcon(":/icons/widgetsEditor/doubleSpinBox.png");
	connect(mDoubleSpinBox, SIGNAL(valueChanged(double))
		, this, SLOT(valueChanged(double)));
}

void DoubleSpinBox::valueChanged(double d)
{
	emit propertyChanged("value", QVariant(d));
}

double DoubleSpinBox::maximum() const
{
	return mDoubleSpinBox->maximum();
}

double DoubleSpinBox::minimum() const
{
	return mDoubleSpinBox->minimum();
}

QString DoubleSpinBox::prefix() const
{
	return mDoubleSpinBox->prefix();
}

QString DoubleSpinBox::suffix() const
{
	return mDoubleSpinBox->suffix();
}

double DoubleSpinBox::singleStep() const
{
	return mDoubleSpinBox->singleStep();
}

double DoubleSpinBox::value() const
{
	return mDoubleSpinBox->value();
}

void DoubleSpinBox::setMaximum(double maximum)
{
	mDoubleSpinBox->setMaximum(maximum);
}

void DoubleSpinBox::setMinimum(double minimum)
{
	mDoubleSpinBox->setMinimum(minimum);
}

void DoubleSpinBox::setPrefix(QString const &prefix)
{
	mDoubleSpinBox->setPrefix(prefix);
}

void DoubleSpinBox::setSuffix(QString const &suffix)
{
	mDoubleSpinBox->setSuffix(suffix);
}

void DoubleSpinBox::setSingleStep(double step)
{
	mDoubleSpinBox->setSingleStep(step);
}

void DoubleSpinBox::setValue(double value)
{
	mDoubleSpinBox->setValue(value);
}

DoubleSpinBoxWidget::DoubleSpinBoxWidget()
	: QDoubleSpinBox(), PropertyEditor(this)
{
	connect(this, SIGNAL(valueChanged(QString)), this, SLOT(onValueChanged(QString)));
}

void DoubleSpinBoxWidget::setPropertyValue(const QString &value)
{
	bool ok = false;
	double val = value.toDouble(&ok);
	if (ok) {
		setValue(val);
	}
}

void DoubleSpinBoxWidget::onValueChanged(const QString &text)
{
	setValueInRepo(text);
}
