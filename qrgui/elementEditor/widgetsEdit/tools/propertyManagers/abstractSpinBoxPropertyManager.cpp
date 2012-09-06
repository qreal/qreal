#include "abstractSpinBoxPropertyManager.h"

using namespace qReal::widgetsEdit;

AbstractSpinBoxPropertyManager::AbstractSpinBoxPropertyManager(
		AbstractSpinBox *abstractSpinBox)
	: ToolPropertyManager(abstractSpinBox)
{
	mAbstractSpinBox = dynamic_cast<QAbstractSpinBox *>(mTool->widget());
	if (!mAbstractSpinBox)
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

void AbstractSpinBoxPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mAbstractSpinBox || !containsProperty(name))
	{
		ToolPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "accelerated")
	{
		mAbstractSpinBox->setAccelerated(value.toBool());
	}
	if (name == "correctionMode")
	{
		mAbstractSpinBox->setCorrectionMode(
					correctionModeTypeToEnum(
						mCorrectionModeTypes[value.toInt()]));
	}
	if (name == "frame")
	{
		mAbstractSpinBox->setFrame(value.toBool());
	}
	if (name == "hasButtons")
	{
		QAbstractSpinBox::ButtonSymbols symbols =
				value.toBool() ? QAbstractSpinBox::UpDownArrows
							   : QAbstractSpinBox::NoButtons;
		mAbstractSpinBox->setButtonSymbols(symbols);
	}
	if (name == "readOnly")
	{
		mAbstractSpinBox->setReadOnly(value.toBool());
	}
	if (name == "specialValueText")
	{
		mAbstractSpinBox->setSpecialValueText(value.toString());
	}
	if (name == "wrapping")
	{
		mAbstractSpinBox->setWrapping(value.toBool());
	}
}

QVariant AbstractSpinBoxPropertyManager::propertyValue(const QString &name)
{
	if (!mAbstractSpinBox || !containsProperty(name))
	{
		return ToolPropertyManager::propertyValue(name);
	}
	if (name == "accelerated")
	{
		return QVariant(mAbstractSpinBox->isAccelerated());
	}
	if (name == "correctionMode")
	{
		return QVariant(mCorrectionModeTypes.indexOf(
							correctionModeEnumToType(
								mAbstractSpinBox->correctionMode())));
	}
	if (name == "frame")
	{
		return QVariant(mAbstractSpinBox->hasFrame());
	}
	if (name == "hasButtons")
	{
		return QVariant(mAbstractSpinBox->buttonSymbols()
						!= QAbstractSpinBox::NoButtons);
	}
	if (name == "readOnly")
	{
		return QVariant(mAbstractSpinBox->isReadOnly());
	}
	if (name == "specialValueText")
	{
		return QVariant(mAbstractSpinBox->specialValueText());
	}
	if (name == "wrapping")
	{
		return QVariant(mAbstractSpinBox->wrapping());
	}
	return QVariant();
}

void AbstractSpinBoxPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("accelerated", QVariant::Bool);
	mProperties << QPair<QString, int>("correctionMode",
									   QtVariantPropertyManager::enumTypeId());
	mCorrectionModeTypes << "CorrectToPreviousValue"
						 << "CorrectToNearestValue";
	mProperties << QPair<QString, int>("frame", QVariant::Bool);
	mProperties << QPair<QString, int>("hasButtons", QVariant::Bool);
	mProperties << QPair<QString, int>("readOnly", QVariant::Bool);
	mProperties << QPair<QString, int>("specialValueText", QVariant::String);
	mProperties << QPair<QString, int>("wrapping", QVariant::Bool);
}

bool AbstractSpinBoxPropertyManager::containsProperty(const QString &name)
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

void AbstractSpinBoxPropertyManager::addEnum(const QString &name)
{
	if (name == "correctionMode")
	{
		addEnumProperty(name, mCorrectionModeTypes);
	}
}

QAbstractSpinBox::CorrectionMode AbstractSpinBoxPropertyManager::correctionModeTypeToEnum(
		const QString &type)
{
	if (type == "CorrectToPreviousValue")
		return QAbstractSpinBox::CorrectToPreviousValue;
	if (type == "CorrectToNearestValue")
		return QAbstractSpinBox::CorrectToNearestValue;
	return QAbstractSpinBox::CorrectToPreviousValue;
}

QString AbstractSpinBoxPropertyManager::correctionModeEnumToType(
		QAbstractSpinBox::CorrectionMode mode)
{
	switch(mode)
	{
	case QAbstractSpinBox::CorrectToPreviousValue:
		return "CorrectToPreviousValue";
	case QAbstractSpinBox::CorrectToNearestValue:
		return "CorrectToNearestValue";
	default:
		return "CorrectToPreviousValue";
	}
}
