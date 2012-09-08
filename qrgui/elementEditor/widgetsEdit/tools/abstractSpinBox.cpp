#include "abstractSpinBox.h"

using namespace qReal::widgetsEdit;

AbstractSpinBox::AbstractSpinBox(QAbstractSpinBox *spinBox
		, ToolController *controller)
	: Tool(spinBox, controller)
{
	mAbstractSpinBox = spinBox;
}

bool AbstractSpinBox::isAccelerated() const
{
	return mAbstractSpinBox->isAccelerated();
}

QAbstractSpinBox::CorrectionMode AbstractSpinBox::correctionMode() const
{
	return mAbstractSpinBox->correctionMode();
}

bool AbstractSpinBox::hasButtons() const
{
	return mAbstractSpinBox->buttonSymbols() != QAbstractSpinBox::NoButtons;
}

bool AbstractSpinBox::isReadonly() const
{
	return mAbstractSpinBox->isReadOnly();
}

QString AbstractSpinBox::specialValueText() const
{
	return mAbstractSpinBox->specialValueText();
}

bool AbstractSpinBox::hasWrapping() const
{
	return mAbstractSpinBox->wrapping();
}

void AbstractSpinBox::setAccelerated(bool accelerated)
{
	mAbstractSpinBox->setAccelerated(accelerated);
}

void AbstractSpinBox::setCorrectionMode(QAbstractSpinBox::CorrectionMode mode)
{
	mAbstractSpinBox->setCorrectionMode(mode);
}

void AbstractSpinBox::setHasButtons(bool hasButtons)
{
	QAbstractSpinBox::ButtonSymbols const symbols = hasButtons
			? QAbstractSpinBox::UpDownArrows
			: QAbstractSpinBox::NoButtons;
	mAbstractSpinBox->setButtonSymbols(symbols);

}

void AbstractSpinBox::setReadonly(bool readonly)
{
	mAbstractSpinBox->setReadOnly(readonly);
}

void AbstractSpinBox::setSpecialValueText(QString const &text)
{
	mAbstractSpinBox->setSpecialValueText(text);
}

void AbstractSpinBox::setWrapping(bool wrapping)
{
	mAbstractSpinBox->setWrapping(wrapping);
}
