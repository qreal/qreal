#include "comboBox.h"

using namespace qReal::widgetsEdit;

ComboBox::ComboBox(ToolController *controller)
	: Tool(new QComboBox, controller)
{
	mComboBox = dynamic_cast<QComboBox *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/comboBox.png");
	mTitle = "Combo Box";
}

bool ComboBox::duplicatesEnabled() const
{
	return mComboBox->duplicatesEnabled();
}

bool ComboBox::isEditable() const
{
	return mComboBox->isEditable();
}

bool ComboBox::hasFrame() const
{
	return mComboBox->hasFrame();
}

int ComboBox::maxCount() const
{
	return mComboBox->maxCount();
}

int ComboBox::maxVisibleItems() const
{
	return mComboBox->maxVisibleItems();
}

int ComboBox::minimumContentsLength() const
{
	return mComboBox->minimumContentsLength();
}

QComboBox::SizeAdjustPolicy ComboBox::sizeAdjustPolicy() const
{
	return mComboBox->sizeAdjustPolicy();
}

void ComboBox::setDuplicatesEnabled(bool enabled)
{
	mComboBox->setDuplicatesEnabled(enabled);
}

void ComboBox::setEditable(bool editable)
{
	mComboBox->setEditable(editable);
}

void ComboBox::setFrame(bool hasFrame)
{
	mComboBox->setFrame(hasFrame);
}

void ComboBox::setMaxCount(int maxCount)
{
	mComboBox->setMaxCount(maxCount);
}

void ComboBox::setMaxVisibleItems(int maxVisibleItems)
{
	mComboBox->setMaxVisibleItems(maxVisibleItems);
}

void ComboBox::setMinimumContentsLength(int length)
{
	mComboBox->setMinimumContentsLength(length);
}

void ComboBox::setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy policy)
{
	mComboBox->setSizeAdjustPolicy(policy);
}

QString ComboBox::bindedPropertyName() const
{
	return mBindedPropertyName;
}

void ComboBox::setBindedPropertyName(QString const &name)
{
	mBindedPropertyName = name;
}
