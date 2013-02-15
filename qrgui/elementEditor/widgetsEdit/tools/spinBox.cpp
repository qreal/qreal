#include "spinBox.h"

using namespace qReal::widgetsEdit;

SpinBox::SpinBox(ToolController *controller)
	: AbstractSpinBox(new SpinBoxWidget, controller)
{
	mSpinBox = dynamic_cast<SpinBoxWidget *>(widget());
	mTitle = tr("Spin Box");
	mTag = "SpinBox";
	mIcon = QIcon(":/icons/widgetsEditor/spinBox.png");
	mProxy = new SpinBoxProxy(mSpinBox);
}

SpinBoxProxy::SpinBoxProxy(SpinBoxWidget *spinBox)
	: AbstractSpinBoxProxy(spinBox), mSpinBox(spinBox)
{
	connect(mSpinBox, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
}

void SpinBoxProxy::valueChanged(int i)
{
	onPropertyChanged("value", QVariant(i));
}

int SpinBoxProxy::maximum() const
{
	return mSpinBox->maximum();
}

int SpinBoxProxy::minimum() const
{
	return mSpinBox->minimum();
}

QString SpinBoxProxy::prefix() const
{
	return mSpinBox->prefix();
}

QString SpinBoxProxy::suffix() const
{
	return mSpinBox->suffix();
}

int SpinBoxProxy::singleStep() const
{
	return mSpinBox->singleStep();
}

int SpinBoxProxy::value() const
{
	return mSpinBox->value();
}

void SpinBoxProxy::setMaximum(int maximum)
{
	mSpinBox->setMaximum(maximum);
}

void SpinBoxProxy::setMinimum(int minimum)
{
	mSpinBox->setMinimum(minimum);
}

void SpinBoxProxy::setPrefix(QString const &prefix)
{
	mSpinBox->setPrefix(prefix);
}

void SpinBoxProxy::setSuffix(QString const &suffix)
{
	mSpinBox->setSuffix(suffix);
}

void SpinBoxProxy::setSingleStep(int step)
{
	mSpinBox->setSingleStep(step);
}

void SpinBoxProxy::setValue(int value)
{
	mSpinBox->setValue(value);
}

SpinBoxWidget::SpinBoxWidget()
	: QSpinBox(), PropertyEditor(this)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(onThisValueChanged(int)));
}

void SpinBoxWidget::setPropertyValue(const QVariant &value)
{
	bool ok = false;
	int val = value.toInt(&ok);
	if (ok) {
		QSpinBox::setValue(val);
	}
}

void SpinBoxWidget::onThisValueChanged(int value)
{
	setValueInRepo(QString::number(value));
}
