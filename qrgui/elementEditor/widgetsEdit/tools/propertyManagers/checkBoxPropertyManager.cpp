#include "checkBoxPropertyManager.h"

using namespace qReal::widgetsEdit;

CheckBoxPropertyManager::CheckBoxPropertyManager(CheckBox *checkBox)
	: ButtonPropertyManagerBase(checkBox)
{
	mCheckBox = dynamic_cast<QCheckBox *>(mTool->widget());
	if (!mCheckBox)
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

void CheckBoxPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mCheckBox || !containsProperty(name))
	{
		ButtonPropertyManagerBase::onPropertyChanged(name, value);
		return;
	}
	if (name == "checked")
	{
		mCheckBox->setChecked(value.toBool());
	}
}

QVariant CheckBoxPropertyManager::propertyValue(const QString &name)
{
	if (!mCheckBox || !containsProperty(name))
	{
		return ButtonPropertyManagerBase::propertyValue(name);
	}
	if (name == "checked")
	{
		return QVariant(mCheckBox->checkState() == Qt::Checked);
	}
	return QVariant();
}

void CheckBoxPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("checked", QVariant::Bool);
}

bool CheckBoxPropertyManager::containsProperty(const QString &name)
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
