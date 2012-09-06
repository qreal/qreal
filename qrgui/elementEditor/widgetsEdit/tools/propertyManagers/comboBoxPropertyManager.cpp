#include "comboBoxPropertyManager.h"

using namespace qReal::widgetsEdit;

ComboBoxPropertyManager::ComboBoxPropertyManager(ComboBox *comboBox)
	: ToolPropertyManager(comboBox)
{
	mComboBox = dynamic_cast<QComboBox *>(mTool->widget());
	if (!mComboBox)
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

void ComboBoxPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mComboBox || !containsProperty(name))
	{
		ToolPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "duplicatesEnabled")
	{
		mComboBox->setDuplicatesEnabled(value.toBool());
	}
	if (name == "editable")
	{
		mComboBox->setEditable(value.toBool());
	}
	if (name == "frame")
	{
		mComboBox->setFrame(value.toBool());
	}
	if (name == "maxCount")
	{
		mComboBox->setMaxCount(value.toInt());
	}
	if (name == "maxVisibleItems")
	{
		mComboBox->setMaxVisibleItems(value.toInt());
	}
	if (name == "minimumContentsLength")
	{
		mComboBox->setMinimumContentsLength(value.toInt());
	}
	if (name == "sizeAdjustPolicy")
	{
		mComboBox->setSizeAdjustPolicy(
					sizeAdjustPolicyTypeToEnum(
						mSizeAdjustPolicyTypes[value.toInt()]));
	}
}

QVariant ComboBoxPropertyManager::propertyValue(const QString &name)
{
	if (!mComboBox || !containsProperty(name))
	{
		return ToolPropertyManager::propertyValue(name);
	}
	if (name == "duplicatesEnabled")
	{
		return QVariant(mComboBox->duplicatesEnabled());
	}
	if (name == "editable")
	{
		return QVariant(mComboBox->isEditable());
	}
	if (name == "frame")
	{
		return QVariant(mComboBox->hasFrame());
	}
	if (name == "maxCount")
	{
		return QVariant(mComboBox->maxCount());
	}
	if (name == "maxVisibleItems")
	{
		return QVariant(mComboBox->maxVisibleItems());
	}
	if (name == "minimumContentsLength")
	{
		return QVariant(mComboBox->minimumContentsLength());
	}
	if (name == "sizeAdjustPolicy")
	{
		return QVariant(mSizeAdjustPolicyTypes.indexOf(
							sizeAdjustPolicyEnumToType(
								mComboBox->sizeAdjustPolicy())));
	}
	return QVariant();
}

void ComboBoxPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("duplicatesEnabled", QVariant::Bool);
	mProperties << QPair<QString, int>("editable", QVariant::Bool);
	mProperties << QPair<QString, int>("frame", QVariant::Bool);
	mProperties << QPair<QString, int>("maxCount", QVariant::Int);
	mProperties << QPair<QString, int>("maxVisibleItems", QVariant::Int);
	mProperties << QPair<QString, int>("minimumContentsLength", QVariant::Int);
	mProperties << QPair<QString, int>("sizeAdjustPolicy",
									   QtVariantPropertyManager::enumTypeId());
	mSizeAdjustPolicyTypes << "AdjustToContents"
						   << "AdjustToContentsOnFirstShow"
						   << "AdjustToMinimumContentsLength";
}

bool ComboBoxPropertyManager::containsProperty(const QString &name)
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

void ComboBoxPropertyManager::addEnum(const QString &name)
{
	if (name == "sizeAdjustPolicy")
	{
		addEnumProperty(name, mSizeAdjustPolicyTypes);
	}
}

QComboBox::SizeAdjustPolicy ComboBoxPropertyManager::sizeAdjustPolicyTypeToEnum(
		const QString &type)
{
	if (type == "AdjustToContents")
	{
		return QComboBox::AdjustToContents;
	}
	if (type == "AdjustToContentsOnFirstShow")
	{
		return QComboBox::AdjustToContentsOnFirstShow;
	}
	if (type == "AdjustToMinimumContentsLength")
	{
		return QComboBox::AdjustToMinimumContentsLength;
	}
	return QComboBox::AdjustToContents;
}

QString ComboBoxPropertyManager::sizeAdjustPolicyEnumToType(
		QComboBox::SizeAdjustPolicy policy)

{
	switch(policy)
	{
	case QComboBox::AdjustToContents:
		return "AdjustToContents";
	case QComboBox::AdjustToContentsOnFirstShow:
		return "AdjustToContentsOnFirstShow";
	case QComboBox::AdjustToMinimumContentsLength:
		return "AdjustToMinimumContentsLength";
	default:
		return "AdjustToContents";
	}
}
