#include "scrollAreaPropertyManager.h"

using namespace Ui::WidgetsEdit;

ScrollAreaPropertyManager::ScrollAreaPropertyManager(ScrollArea *scrollArea)
	: AbstractScrollAreaPropertyManager(scrollArea)
{
	mScrollArea = dynamic_cast<QScrollArea *>(mTool->widget());
	if (!mScrollArea)
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

void ScrollAreaPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mScrollArea || !containsProperty(name))
	{
		AbstractScrollAreaPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "widgetResizable")
	{
		mScrollArea->setWidgetResizable(value.toBool());
	}
}

QVariant ScrollAreaPropertyManager::propertyValue(const QString &name)
{
	if (!mScrollArea || !containsProperty(name))
	{
		return AbstractScrollAreaPropertyManager::propertyValue(name);
	}
	if (name == "widgetResizable")
	{
		return QVariant(mScrollArea->widgetResizable());
	}
	return QVariant();
}

void ScrollAreaPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("widgetResizable", QVariant::Bool);
}

bool ScrollAreaPropertyManager::containsProperty(const QString &name)
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
