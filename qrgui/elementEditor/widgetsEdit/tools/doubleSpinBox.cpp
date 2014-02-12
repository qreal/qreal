#include "doubleSpinBox.h"

using namespace qReal::widgetsEdit;

DoubleSpinBox::DoubleSpinBox(ToolController *controller)
	: AbstractSpinBox(new DoubleSpinBoxWidget, controller)
{
	mDoubleSpinBox = dynamic_cast<DoubleSpinBoxWidget *>(widget());
	mTitle = tr("Double Spin Box");
	mTag = "DoubleSpinBox";
	mIcon = QIcon(":/icons/widgetsEditor/doubleSpinBox.png");
	mProxy = new DoubleSpinBoxProxy(mDoubleSpinBox);
}

DoubleSpinBoxProxy::DoubleSpinBoxProxy(DoubleSpinBoxWidget *doubleSpinBox)
	: AbstractSpinBoxProxy(doubleSpinBox), mDoubleSpinBox(doubleSpinBox)
{
	connect(mDoubleSpinBox, SIGNAL(valueChanged(double))
		, this, SLOT(valueChanged(double)));
}

void DoubleSpinBoxProxy::valueChanged(double d)
{
	onPropertyChanged("value", QVariant(d));
}

double DoubleSpinBoxProxy::maximum() const
{
	return mDoubleSpinBox->maximum();
}

double DoubleSpinBoxProxy::minimum() const
{
	return mDoubleSpinBox->minimum();
}

QString DoubleSpinBoxProxy::prefix() const
{
	return mDoubleSpinBox->prefix();
}

QString DoubleSpinBoxProxy::suffix() const
{
	return mDoubleSpinBox->suffix();
}

double DoubleSpinBoxProxy::singleStep() const
{
	return mDoubleSpinBox->singleStep();
}

double DoubleSpinBoxProxy::value() const
{
	return mDoubleSpinBox->value();
}

void DoubleSpinBoxProxy::setMaximum(double maximum)
{
	mDoubleSpinBox->setMaximum(maximum);
}

void DoubleSpinBoxProxy::setMinimum(double minimum)
{
	mDoubleSpinBox->setMinimum(minimum);
}

void DoubleSpinBoxProxy::setPrefix(QString const &prefix)
{
	mDoubleSpinBox->setPrefix(prefix);
}

void DoubleSpinBoxProxy::setSuffix(QString const &suffix)
{
	mDoubleSpinBox->setSuffix(suffix);
}

void DoubleSpinBoxProxy::setSingleStep(double step)
{
	mDoubleSpinBox->setSingleStep(step);
}

void DoubleSpinBoxProxy::setValue(double value)
{
	mDoubleSpinBox->setValue(value);
}

DoubleSpinBoxWidget::DoubleSpinBoxWidget()
	: QDoubleSpinBox(), PropertyEditor(this)
{
	connect(this, SIGNAL(valueChanged(QString)), this, SLOT(onValueChanged(QString)));
}

void DoubleSpinBoxWidget::setPropertyValue(const QVariant &value)
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
