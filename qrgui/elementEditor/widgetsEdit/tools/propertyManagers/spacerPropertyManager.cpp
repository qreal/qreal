#include "spacerPropertyManager.h"

using namespace qReal::widgetsEdit;

SpacerPropertyManager::SpacerPropertyManager(Spacer *spacer)
	: ToolPropertyManager(spacer)
{
	mSpacer = dynamic_cast<QWidget *>(mTool->widget());
	if (!mSpacer)
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

void SpacerPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mSpacer || !containsProperty(name))
	{
		ToolPropertyManager::onPropertyChanged(name, value);
		return;
	}
}

QVariant SpacerPropertyManager::propertyValue(const QString &name)
{
	if (!mSpacer || !containsProperty(name))
	{
		return ToolPropertyManager::propertyValue(name);
	}
	return QVariant();
}

void SpacerPropertyManager::initProperties()
{
}

bool SpacerPropertyManager::containsProperty(const QString &name)
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
