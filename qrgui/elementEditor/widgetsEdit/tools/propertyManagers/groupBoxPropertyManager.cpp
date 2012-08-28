#include "groupBoxPropertyManager.h"

using namespace Ui::WidgetsEdit;

GroupBoxPropertyManager::GroupBoxPropertyManager(GroupBox *groupBox)
	: LayoutToolPropertyManager(groupBox)
{
	mGroupBox = dynamic_cast<QGroupBox *>(mTool->widget());
	if (!mGroupBox)
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

void GroupBoxPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mGroupBox || !containsProperty(name))
	{
		LayoutToolPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "alignment")
	{
		mGroupBox->setAlignment(alignmentTypeToEnum(
									mAlignmentTypes[value.toInt()]));
	}
	if (name == "flat")
	{
		mGroupBox->setFlat(value.toBool());
	}
	if (name == "title")
	{
		mGroupBox->setTitle(value.toString());
	}
}

QVariant GroupBoxPropertyManager::propertyValue(const QString &name)
{
	if (!mGroupBox || !containsProperty(name))
	{
		return LayoutToolPropertyManager::propertyValue(name);
	}
	if (name == "alignment")
	{
		return QVariant(mAlignmentTypes.indexOf(
							alignmentEnumToType(mGroupBox->alignment())));
	}
	if (name == "flat")
	{
		return QVariant(mGroupBox->isFlat());
	}
	if (name == "title")
	{
		return QVariant(mGroupBox->title());
	}
	return QVariant();
}

void GroupBoxPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("alignment",
									   QtVariantPropertyManager::enumTypeId());
	mAlignmentTypes << "Left" << "Center" << "Right";
	mProperties << QPair<QString, int>("flat", QVariant::Bool);
	mProperties << QPair<QString, int>("title", QVariant::String);
}

bool GroupBoxPropertyManager::containsProperty(const QString &name)
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

void GroupBoxPropertyManager::addEnum(const QString &name)
{
	if (name == "alignment")
	{
		addEnumProperty(name, mAlignmentTypes);
	}
}

Qt::Alignment GroupBoxPropertyManager::alignmentTypeToEnum(const QString &type)
{
	if (type == "Left")
	{
		return Qt::AlignLeft | Qt::AlignVCenter;
	}
	if (type == "Center")
	{
		return Qt::AlignCenter | Qt::AlignVCenter;
	}
	if (type == "Right")
	{
		return Qt::AlignRight | Qt::AlignVCenter;
	}
	return Qt::AlignLeft | Qt::AlignVCenter;
}

QString GroupBoxPropertyManager::alignmentEnumToType(Qt::Alignment alignment)
{
	if (alignment.testFlag(Qt::AlignLeft))
	{
		return "Left";
	}
	if (alignment.testFlag(Qt::AlignCenter))
	{
		return "Center";
	}
	if (alignment.testFlag(Qt::AlignRight))
	{
		return "Right";
	}
	return "Left";
}
