#include "toolPropertyManager.h"

using namespace qReal::widgetsEdit;

ToolPropertyManager::ToolPropertyManager(Tool *tool)
	: PropertyManagerBase(tool)
{
	initProperties();
	QListIterator<QPair<QString, int> > *it
			= new QListIterator<QPair<QString, int> >(mProperties);
	while (it->hasNext())
	{
		QPair<QString, int> property = it->next();
		addProperty(property.first, property.second);
	}
}

void ToolPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	QWidget *widget = mTool->widget();
	if (name == "baseSize")
		widget->setBaseSize(value.toSize());
	else if (name == "enabled")
		widget->setEnabled(value.toBool());
	else if (name == "geometry")
		widget->setGeometry(value.toRect());
	else if (name == "maximumHeight")
	{
		mTool->setMaximumHeight(value.toInt());
		widget->setMaximumHeight(value.toInt());
	}
	else if (name == "maximumSize")
	{
		mTool->setMaximumSize(value.toSize());
		widget->setMaximumSize(value.toSize());
	}
	else if (name == "maximumWidth")
	{
		mTool->setMaximumWidth(value.toInt());
		widget->setMaximumWidth(value.toInt());
	}
	else if (name == "minimumHeight")
	{
		mTool->setMinimumHeight(value.toInt());
		widget->setMinimumHeight(value.toInt());
	}
	else if (name == "minimumSize")
	{
		mTool->setMinimumSize(value.toSize());
		widget->setMinimumSize(value.toSize());
	}
	else if (name == "minimumWidth")
	{
		mTool->setMinimumWidth(value.toInt());
		widget->setMinimumWidth(value.toInt());
	}
	else if (name == "sizeIncrement")
		widget->setSizeIncrement(value.toSize());
	else if (name == "toolTip")
		widget->setToolTip(value.toString());
}

QVariant ToolPropertyManager::propertyValue(const QString &name)
{
	QWidget *widget = mTool->widget();
	if (name == "baseSize")
		return QVariant(widget->baseSize());
	else if (name == "enabled")
		return QVariant(widget->isEnabled());
	else if (name == "geometry")
		return QVariant(widget->geometry());
	else if (name == "maximumHeight")
		return QVariant(widget->maximumHeight());
	else if (name == "maximumSize")
		return QVariant(widget->maximumSize());
	else if (name == "maximumWidth")
		return QVariant(widget->maximumWidth());
	else if (name == "minimumHeight")
		return QVariant(widget->minimumHeight());
	else if (name == "minimumSize")
		return QVariant(widget->minimumSize());
	else if (name == "minimumWidth")
		return QVariant(widget->minimumWidth());
	else if (name == "sizeIncrement")
		return QVariant(widget->sizeIncrement());
	else if (name == "toolTip")
		return QVariant(widget->toolTip());
	return QVariant();
}

void ToolPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("baseSize", QVariant::Size);
	mProperties << QPair<QString, int>("enabled", QVariant::Bool);
	mProperties << QPair<QString, int>("geometry", QVariant::Rect);
	mProperties << QPair<QString, int>("maximumHeight", QVariant::Int);
	mProperties << QPair<QString, int>("maximumSize", QVariant::Size);
	mProperties << QPair<QString, int>("maximumWidth", QVariant::Int);
	mProperties << QPair<QString, int>("minimumHeight", QVariant::Int);
	mProperties << QPair<QString, int>("minimumSize", QVariant::Size);
	mProperties << QPair<QString, int>("minimumWidth", QVariant::Int);
	mProperties << QPair<QString, int>("sizeIncrement", QVariant::Size);
	mProperties << QPair<QString, int>("toolTip", QVariant::String);
}
