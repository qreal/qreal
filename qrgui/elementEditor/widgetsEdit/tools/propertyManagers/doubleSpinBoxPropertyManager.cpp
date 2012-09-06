#include "doubleSpinBoxPropertyManager.h"

using namespace qReal::widgetsEdit;

DoubleSpinBoxPropertyManager::DoubleSpinBoxPropertyManager(DoubleSpinBox *doubleSpinBox)
	: AbstractSpinBoxPropertyManager(doubleSpinBox)
{
	mDoubleSpinBox = dynamic_cast<QDoubleSpinBox *>(mTool->widget());
	if (!mDoubleSpinBox)
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

void DoubleSpinBoxPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mDoubleSpinBox || !containsProperty(name))
	{
		AbstractSpinBoxPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "maximum")
	{
		mDoubleSpinBox->setMaximum(value.toDouble());
	}
	if (name == "minimum")
	{
		mDoubleSpinBox->setMinimum(value.toDouble());
	}
	if (name == "prefix")
	{
		mDoubleSpinBox->setPrefix(value.toString());
	}
	if (name == "suffix")
	{
		mDoubleSpinBox->setSuffix(value.toString());
	}
	if (name == "singleStep")
	{
		mDoubleSpinBox->setSingleStep(value.toDouble());
	}
	if (name == "value")
	{
		mDoubleSpinBox->setValue(value.toDouble());
	}
}

QVariant DoubleSpinBoxPropertyManager::propertyValue(const QString &name)
{
	if (!mDoubleSpinBox || !containsProperty(name))
	{
		return AbstractSpinBoxPropertyManager::propertyValue(name);
	}
	if (name == "maximum")
	{
		return QVariant(mDoubleSpinBox->maximum());
	}
	if (name == "minimum")
	{
		return QVariant(mDoubleSpinBox->minimum());
	}
	if (name == "prefix")
	{
		return QVariant(mDoubleSpinBox->prefix());
	}
	if (name == "suffix")
	{
		return QVariant(mDoubleSpinBox->suffix());
	}
	if (name == "singleStep")
	{
		return QVariant(mDoubleSpinBox->singleStep());
	}
	if (name == "value")
	{
		return QVariant(mDoubleSpinBox->value());
	}
	return QVariant();
}

void DoubleSpinBoxPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("maximum", QVariant::Double);
	mProperties << QPair<QString, int>("minimum", QVariant::Double);
	mProperties << QPair<QString, int>("prefix", QVariant::String);
	mProperties << QPair<QString, int>("suffix", QVariant::String);
	mProperties << QPair<QString, int>("singleStep", QVariant::Double);
	mProperties << QPair<QString, int>("value", QVariant::Double);
}

bool DoubleSpinBoxPropertyManager::containsProperty(const QString &name)
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
