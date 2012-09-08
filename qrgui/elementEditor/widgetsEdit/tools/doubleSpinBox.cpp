#include "doubleSpinBox.h"

using namespace qReal::widgetsEdit;

DoubleSpinBox::DoubleSpinBox(ToolController *controller)
	: AbstractSpinBox(new QDoubleSpinBox, controller)
{
	mDoubleSpinBox = dynamic_cast<QDoubleSpinBox *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/doubleSpinBox.png");
	mTitle = "Double Spin Box";
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
