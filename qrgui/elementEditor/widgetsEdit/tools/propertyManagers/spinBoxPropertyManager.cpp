#include "spinBoxPropertyManager.h"

using namespace qReal::widgetsEdit;

SpinBoxPropertyManager::SpinBoxPropertyManager(SpinBox *spinBox)
	: AbstractSpinBoxPropertyManager(spinBox)
{
	mSpinBox = dynamic_cast<QSpinBox *>(mTool->widget());
	if (!mSpinBox)
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

void SpinBoxPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mSpinBox || !containsProperty(name))
	{
		AbstractSpinBoxPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "maximum")
	{
		mSpinBox->setMaximum(value.toInt());
	}
	if (name == "minimum")
	{
		mSpinBox->setMinimum(value.toInt());
	}
	if (name == "prefix")
	{
		mSpinBox->setPrefix(value.toString());
	}
	if (name == "suffix")
	{
		mSpinBox->setSuffix(value.toString());
	}
	if (name == "singleStep")
	{
		mSpinBox->setSingleStep(value.toInt());
	}
	if (name == "value")
	{
		mSpinBox->setValue(value.toInt());
	}
}

QVariant SpinBoxPropertyManager::propertyValue(const QString &name)
{
	if (!mSpinBox || !containsProperty(name))
	{
		return AbstractSpinBoxPropertyManager::propertyValue(name);
	}
	if (name == "maximum")
	{
		return QVariant(mSpinBox->maximum());
	}
	if (name == "minimum")
	{
		return QVariant(mSpinBox->minimum());
	}
	if (name == "prefix")
	{
		return QVariant(mSpinBox->prefix());
	}
	if (name == "suffix")
	{
		return QVariant(mSpinBox->suffix());
	}
	if (name == "singleStep")
	{
		return QVariant(mSpinBox->singleStep());
	}
	if (name == "value")
	{
		return QVariant(mSpinBox->value());
	}
	return QVariant();
}

void SpinBoxPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("maximum", QVariant::Int);
	mProperties << QPair<QString, int>("minimum", QVariant::Int);
	mProperties << QPair<QString, int>("prefix", QVariant::String);
	mProperties << QPair<QString, int>("suffix", QVariant::String);
	mProperties << QPair<QString, int>("singleStep", QVariant::Int);
	mProperties << QPair<QString, int>("value", QVariant::Int);
}

bool SpinBoxPropertyManager::containsProperty(const QString &name)
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
