#include "abstractScrollAreaPropertyManager.h"

using namespace Ui::WidgetsEdit;

AbstractScrollAreaPropertyManager::AbstractScrollAreaPropertyManager(
		AbstractScrollArea *abstractScrollArea)
	: FramePropertyManager(abstractScrollArea)
{
	mAbstractScrollArea = dynamic_cast<QAbstractScrollArea *>(mTool->widget());
	if (!mAbstractScrollArea)
	{
		return;
	}
	initProperties();
	QListIterator<QPair<QString, int> > *it
			= new QListIterator<QPair<QString, int> >(mProperties);
	while (it->hasNext())
	{
		QPair<QString, int> property = it->next();
		if (property.second != QtVariantPropertyManager::enumTypeId())
			addProperty(property.first, property.second);
		else
			addEnum(property.first);
	}
}

void AbstractScrollAreaPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mAbstractScrollArea || !containsProperty(name))
	{
		FramePropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "verticalScrollBarPolicy")
	{
		mAbstractScrollArea->setVerticalScrollBarPolicy(
					scrollBarPolicyTypeToEnum(
						mScrollBarPolicyTypes[value.toInt()]));
	}
	if (name == "horizontalScrollBarPolicy")
	{
		mAbstractScrollArea->setHorizontalScrollBarPolicy(
					scrollBarPolicyTypeToEnum(
						mScrollBarPolicyTypes[value.toInt()]));
	}
}

QVariant AbstractScrollAreaPropertyManager::propertyValue(const QString &name)
{
	if (!mAbstractScrollArea || !containsProperty(name))
	{
		return FramePropertyManager::propertyValue(name);
	}
	if (name == "verticalScrollBarPolicy")
	{
		return QVariant(mScrollBarPolicyTypes.indexOf(
							scrollBarPolicyEnumToType(
								mAbstractScrollArea->verticalScrollBarPolicy())));
	}
	if (name == "horizontalScrollBarPolicy")
	{
		return QVariant(mScrollBarPolicyTypes.indexOf(
							scrollBarPolicyEnumToType(
								mAbstractScrollArea->horizontalScrollBarPolicy())));
	}
	return QVariant();
}

void AbstractScrollAreaPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("verticalScrollBarPolicy",
									   QtVariantPropertyManager::enumTypeId());
	mProperties << QPair<QString, int>("horizontalScrollBarPolicy",
									   QtVariantPropertyManager::enumTypeId());
	mScrollBarPolicyTypes << "AsNeeded" << "AlwaysOff" << "AlwaysOn";
}

bool AbstractScrollAreaPropertyManager::containsProperty(const QString &name) const
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

void AbstractScrollAreaPropertyManager::addEnum(const QString &name)
{
	if (name == "verticalScrollBarPolicy")
	{
		addEnumProperty(name, mScrollBarPolicyTypes);
	}
	if (name == "horizontalScrollBarPolicy")
	{
		addEnumProperty(name, mScrollBarPolicyTypes);
	}
}

Qt::ScrollBarPolicy AbstractScrollAreaPropertyManager::scrollBarPolicyTypeToEnum(const QString &type)
{
	if (type == "AsNeeded")
		return Qt::ScrollBarAsNeeded;
	if (type == "AlwaysOff")
		return Qt::ScrollBarAlwaysOff;
	if (type == "AlwaysOn")
		return Qt::ScrollBarAlwaysOn;
	return Qt::ScrollBarAsNeeded;
}

QString AbstractScrollAreaPropertyManager::scrollBarPolicyEnumToType(Qt::ScrollBarPolicy policy)
{
	switch(policy)
	{
	case Qt::ScrollBarAsNeeded:
		return "AsNeeded";
	case Qt::ScrollBarAlwaysOff:
		return "AlwaysOff";
	case Qt::ScrollBarAlwaysOn:
		return "AlwaysOn";
	default:
		return "AsNeeded";
	}
}
