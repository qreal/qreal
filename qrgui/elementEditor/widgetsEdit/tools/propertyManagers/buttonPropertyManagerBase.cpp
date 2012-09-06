#include "buttonPropertyManagerBase.h"

using namespace qReal::widgetsEdit;

ButtonPropertyManagerBase::ButtonPropertyManagerBase(Tool *buttonTool)
	: ToolPropertyManager(buttonTool)
{
	mButton = dynamic_cast<QAbstractButton *>(mTool->widget());
	if (!mButton)
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

void ButtonPropertyManagerBase::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mButton || !containsProperty(name))
	{
		ToolPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "autoRepeat")
	{
		mButton->setAutoRepeat(value.toBool());
	}
	if (name == "autoRepeatDelay")
	{
		mButton->setAutoRepeatDelay(value.toInt());
	}
	if (name == "autoRepeatInterval")
	{
		mButton->setAutoRepeatInterval(value.toInt());
	}
	if (name == "text")
	{
		mButton->setText(value.toString());
	}
}

QVariant ButtonPropertyManagerBase::propertyValue(const QString &name)
{
	if (!mButton || !containsProperty(name))
	{
		return ToolPropertyManager::propertyValue(name);
	}
	if (name == "autoRepeat")
	{
		return QVariant(mButton->autoRepeat());
	}
	if (name == "autoRepeatDelay")
	{
		return QVariant(mButton->autoRepeatDelay());
	}
	if (name == "autoRepeatInterval")
	{
		return QVariant(mButton->autoRepeatInterval());
	}
	if (name == "text")
	{
		return QVariant(mButton->text());
	}
	return QVariant();
}

void ButtonPropertyManagerBase::initProperties()
{
	mProperties << QPair<QString, int>("autoRepeat", QVariant::Bool);
	mProperties << QPair<QString, int>("autoRepeatDelay", QVariant::Int);
	mProperties << QPair<QString, int>("autoRepeatInterval", QVariant::Int);
	mProperties << QPair<QString, int>("text", QVariant::String);
}

bool ButtonPropertyManagerBase::containsProperty(const QString &name)
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
