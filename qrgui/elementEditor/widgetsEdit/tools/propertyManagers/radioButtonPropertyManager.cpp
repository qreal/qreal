#include "radioButtonPropertyManager.h"

using namespace qReal::widgetsEdit;

RadioButtonPropertyManager::RadioButtonPropertyManager(RadioButton *radioButton)
	: ButtonPropertyManagerBase(radioButton)
{
	mRadioButton = dynamic_cast<QRadioButton *>(mTool->widget());
	if (!mRadioButton)
	{
		return;
	}
	initProperties();
	QListIterator<QPair<QString, int> > *it
			= new QListIterator<QPair<QString, int> >(mProperties);
	while (it->hasNext())
	{
		QPair<QString, int> property = it->next();
		addProperty(property.first, property.second);
	}
}

void RadioButtonPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mRadioButton || !containsProperty(name))
	{
		ButtonPropertyManagerBase::onPropertyChanged(name, value);
		return;
	}
	if (name == "checked")
	{
		mRadioButton->setChecked(value.toBool());
	}
}

QVariant RadioButtonPropertyManager::propertyValue(const QString &name)
{
	if (!mRadioButton || !containsProperty(name))
	{
		return ButtonPropertyManagerBase::propertyValue(name);
	}
	if (name == "checked")
	{
		return QVariant(mRadioButton->isChecked());
	}
	return QVariant();
}

void RadioButtonPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("checked", QVariant::Bool);
}

bool RadioButtonPropertyManager::containsProperty(const QString &name)
{
	QListIterator<QPair<QString, int> > it =
			QListIterator<QPair<QString, int> >(mProperties);
	while (it.hasNext())
	{
		if (it.next().first == name)
		{
			return true;
		}
	}
	return false;
}
