#include "labelPropertyManager.h"

using namespace qReal::widgetsEdit;

LabelPropertyManager::LabelPropertyManager(Label *label)
	: FramePropertyManager(label)
{
	mLabel = dynamic_cast<QLabel *>(mTool->widget());
	if (!mLabel)
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

void LabelPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mLabel || !containsProperty(name))
	{
		FramePropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "alignment")
	{
		mLabel->setAlignment(alignmentTypeToEnum(
									mAlignmentTypes[value.toInt()]));
	}
	if (name == "indent")
	{
		mLabel->setIndent(value.toInt());
	}
	if (name == "margin")
	{
		mLabel->setMargin(value.toInt());
	}
	if (name == "openExternalLinks")
	{
		mLabel->setOpenExternalLinks(value.toBool());
	}
	if (name == "scaledContents")
	{
		mLabel->setScaledContents(value.toBool());
	}
	if (name == "text")
	{
		mLabel->setText(value.toString());
	}
	if (name == "textFormat")
	{
		mLabel->setTextFormat(textFormatTypeToEnum(
									mTextFormatTypes[value.toInt()]));
	}
	if (name == "wordWrap")
	{
		mLabel->setWordWrap(value.toBool());
	}
}

QVariant LabelPropertyManager::propertyValue(const QString &name)
{
	if (!mLabel || !containsProperty(name))
	{
		return FramePropertyManager::propertyValue(name);
	}
	if (name == "alignment")
	{
		return QVariant(mAlignmentTypes.indexOf(
							alignmentEnumToType(mLabel->alignment())));
	}
	if (name == "indent")
	{
		return QVariant(mLabel->indent());
	}
	if (name == "margin")
	{
		return QVariant(mLabel->margin());
	}
	if (name == "openExternalLinks")
	{
		return QVariant(mLabel->openExternalLinks());
	}
	if (name == "scaledContents")
	{
		return QVariant(mLabel->hasScaledContents());
	}
	if (name == "text")
	{
		return QVariant(mLabel->text());
	}
	if (name == "textFormat")
	{
		return QVariant(mTextFormatTypes.indexOf(
							textFormatEnumToType(mLabel->textFormat())));
	}
	if (name == "wordWrap")
	{
		return QVariant(mLabel->wordWrap());
	}
	return QVariant();
}

void LabelPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("alignment",
									   QtVariantPropertyManager::enumTypeId());
	mAlignmentTypes << "Left" << "Center" << "Right";
	mProperties << QPair<QString, int>("indent", QVariant::Int);
	mProperties << QPair<QString, int>("margin", QVariant::Int);
	mProperties << QPair<QString, int>("openExternalLinks", QVariant::Bool);
	mProperties << QPair<QString, int>("scaledContents", QVariant::Bool);
	mProperties << QPair<QString, int>("text", QVariant::String);
	mProperties << QPair<QString, int>("textFormat",
									   QtVariantPropertyManager::enumTypeId());
	mTextFormatTypes << "PlainText" << "RichText" << "AutoText";
	mProperties << QPair<QString, int>("wordWrap", QVariant::Bool);
}

bool LabelPropertyManager::containsProperty(const QString &name)
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

void LabelPropertyManager::addEnum(const QString &name)
{
	if (name == "alignment")
	{
		addEnumProperty(name, mAlignmentTypes);
	}
	if (name == "textFormat")
	{
		addEnumProperty(name, mTextFormatTypes);
	}
}

Qt::Alignment LabelPropertyManager::alignmentTypeToEnum(const QString &type)
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

QString LabelPropertyManager::alignmentEnumToType(Qt::Alignment alignment)
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

Qt::TextFormat LabelPropertyManager::textFormatTypeToEnum(const QString &type)
{
	if (type == "PlainText")
	{
		return Qt::PlainText;
	}
	if (type == "RichText")
	{
		return Qt::RichText;
	}
	if (type == "AutoText")
	{
		return Qt::AutoText;
	}
	return Qt::PlainText;
}

QString LabelPropertyManager::textFormatEnumToType(Qt::TextFormat format)
{
	switch(format)
	{
	case Qt::PlainText:
		return "PlainText";
	case Qt::RichText:
		return "RichText";
	case Qt::AutoText:
		return "AutoText";
	default:
		return "PlainText";
	}
}
