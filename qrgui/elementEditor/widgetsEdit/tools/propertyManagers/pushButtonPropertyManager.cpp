#include "pushButtonPropertyManager.h"

using namespace Ui::WidgetsEdit;

PushButtonPropertyManager::PushButtonPropertyManager(PushButton *pushButton)
	: ButtonPropertyManagerBase(pushButton)
{
	mPushButton = dynamic_cast<QPushButton *>(mTool->widget());
	if (!mPushButton)
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

void PushButtonPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mPushButton || !containsProperty(name))
	{
		ButtonPropertyManagerBase::onPropertyChanged(name, value);
		return;
	}
	if (name == "flat")
	{
		mPushButton->setFlat(value.toBool());
	}
}

QVariant PushButtonPropertyManager::propertyValue(const QString &name)
{
	if (!mPushButton || !containsProperty(name))
	{
		return ButtonPropertyManagerBase::propertyValue(name);
	}
	if (name == "flat")
	{
		return QVariant(mPushButton->isFlat());
	}
	return QVariant();
}

void PushButtonPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("flat", QVariant::Bool);
}

bool PushButtonPropertyManager::containsProperty(const QString &name)
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
