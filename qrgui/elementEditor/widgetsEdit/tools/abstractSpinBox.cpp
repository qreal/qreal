#include "abstractSpinBox.h"

using namespace qReal::widgetsEdit;

AbstractSpinBox::AbstractSpinBox(QAbstractSpinBox *spinBox
		, ToolController *controller)
	: Tool(spinBox, controller)
{
	mAbstractSpinBox = spinBox;
}

AbstractSpinBoxProxy::AbstractSpinBoxProxy(QAbstractSpinBox *spinBox)
	: ToolProxy(spinBox), mAbstractSpinBox(spinBox)
{
}

bool AbstractSpinBoxProxy::isAccelerated() const
{
	return mAbstractSpinBox->isAccelerated();
}

QAbstractSpinBox::CorrectionMode AbstractSpinBoxProxy::correctionMode() const
{
	return mAbstractSpinBox->correctionMode();
}

bool AbstractSpinBoxProxy::hasButtons() const
{
	return mAbstractSpinBox->buttonSymbols() != QAbstractSpinBox::NoButtons;
}

bool AbstractSpinBoxProxy::isReadonly() const
{
	return mAbstractSpinBox->isReadOnly();
}

QString AbstractSpinBoxProxy::specialValueText() const
{
	return mAbstractSpinBox->specialValueText();
}

bool AbstractSpinBoxProxy::hasWrapping() const
{
	return mAbstractSpinBox->wrapping();
}

void AbstractSpinBoxProxy::setAccelerated(bool accelerated)
{
	mAbstractSpinBox->setAccelerated(accelerated);
}

void AbstractSpinBoxProxy::setCorrectionMode(QAbstractSpinBox::CorrectionMode mode)
{
	mAbstractSpinBox->setCorrectionMode(mode);
}

void AbstractSpinBoxProxy::setHasButtons(bool hasButtons)
{
	QAbstractSpinBox::ButtonSymbols const symbols = hasButtons
			? QAbstractSpinBox::UpDownArrows
			: QAbstractSpinBox::NoButtons;
	mAbstractSpinBox->setButtonSymbols(symbols);
}

void AbstractSpinBoxProxy::setReadonly(bool readonly)
{
	mAbstractSpinBox->setReadOnly(readonly);
}

void AbstractSpinBoxProxy::setSpecialValueText(QString const &text)
{
	mAbstractSpinBox->setSpecialValueText(text);
}

void AbstractSpinBoxProxy::setWrapping(bool wrapping)
{
	mAbstractSpinBox->setWrapping(wrapping);
}
