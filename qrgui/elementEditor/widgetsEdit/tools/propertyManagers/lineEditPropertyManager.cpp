#include "lineEditPropertyManager.h"

using namespace Ui::WidgetsEdit;

LineEditPropertyManager::LineEditPropertyManager(LineEdit *lineEdit)
	: ToolPropertyManager(lineEdit)
{
	mLineEdit = dynamic_cast<QLineEdit *>(mTool->widget());
	if (!mLineEdit)
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

void LineEditPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mLineEdit || !containsProperty(name))
	{
		ToolPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "alignment")
	{
		mLineEdit->setAlignment(alignmentTypeToEnum(
									mAlignmentTypes[value.toInt()]));
	}
	if (name == "echoMode")
	{
		mLineEdit->setEchoMode(echoModeTypeToEnum(
									mEchoModeTypes[value.toInt()]));
	}
	if (name == "frame")
	{
		mLineEdit->setFrame(value.toBool());
	}
	if (name == "inputMask")
	{
		mLineEdit->setInputMask(value.toString());
	}
	if (name == "maxLength")
	{
		mLineEdit->setMaxLength(value.toInt());
	}
	if (name == "placeholderText")
	{
		mLineEdit->setPlaceholderText(value.toString());
	}
	if (name == "readonly")
	{
		mLineEdit->setReadOnly(value.toBool());
	}
	if (name == "text")
	{
		mLineEdit->setText(value.toString());
	}
}

QVariant LineEditPropertyManager::propertyValue(const QString &name)
{
	if (!mLineEdit || !containsProperty(name))
	{
		return ToolPropertyManager::propertyValue(name);
	}
	if (name == "alignment")
	{
		return QVariant(mAlignmentTypes.indexOf(
							alignmentEnumToType(mLineEdit->alignment())));
	}
	if (name == "echoMode")
	{
		return QVariant(mEchoModeTypes.indexOf(
							echoModeEnumToType(mLineEdit->echoMode())));
	}
	if (name == "frame")
	{
		return QVariant(mLineEdit->hasFrame());
	}
	if (name == "inputMask")
	{
		return QVariant(mLineEdit->inputMask());
	}
	if (name == "maxLength")
	{
		return QVariant(mLineEdit->maxLength());
	}
	if (name == "placeholderText")
	{
		return QVariant(mLineEdit->placeholderText());
	}
	if (name == "readonly")
	{
		return QVariant(mLineEdit->isReadOnly());
	}
	if (name == "text")
	{
		return QVariant(mLineEdit->text());
	}
	return QVariant();
}

void LineEditPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("alignment",
									   QtVariantPropertyManager::enumTypeId());
	mAlignmentTypes << "Left" << "Center" << "Right";
	mProperties << QPair<QString, int>("echoMode",
									   QtVariantPropertyManager::enumTypeId());
	mEchoModeTypes << "Normal" << "NoEcho"
					<< "Password" << "PasswordEchoOnEdit";
	mProperties << QPair<QString, int>("frame", QVariant::Bool);
	mProperties << QPair<QString, int>("inputMask", QVariant::String);
	mProperties << QPair<QString, int>("maxLength", QVariant::Int);
	mProperties << QPair<QString, int>("placeholderText", QVariant::String);
	mProperties << QPair<QString, int>("readonly", QVariant::Bool);
	mProperties << QPair<QString, int>("text", QVariant::String);
}

bool LineEditPropertyManager::containsProperty(const QString &name)
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

void LineEditPropertyManager::addEnum(const QString &name)
{
	if (name == "alignment")
	{
		addEnumProperty(name, mAlignmentTypes);
	}
	if (name == "echoMode")
	{
		addEnumProperty(name, mEchoModeTypes);
	}
}

Qt::Alignment LineEditPropertyManager::alignmentTypeToEnum(const QString &type)
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

QString LineEditPropertyManager::alignmentEnumToType(Qt::Alignment alignment)
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

QLineEdit::EchoMode LineEditPropertyManager::echoModeTypeToEnum(QString const &type)
{
	if (type == "Normal")
	{
		return QLineEdit::Normal;
	}
	if (type == "NoEcho")
	{
		return QLineEdit::NoEcho;
	}
	if (type == "Password")
	{
		return QLineEdit::Password;
	}
	if (type == "PasswordEchoOnEdit")
	{
		return QLineEdit::PasswordEchoOnEdit;
	}
	return QLineEdit::Normal;
}

QString LineEditPropertyManager::echoModeEnumToType(QLineEdit::EchoMode echoMode)
{
	switch(echoMode)
	{
	case QLineEdit::Normal:
		return "Normal";
	case QLineEdit::NoEcho:
		return "NoEcho";
	case QLineEdit::Password:
		return "Password";
	case QLineEdit::PasswordEchoOnEdit:
		return "PasswordEchoOnEdit";
	default:
		return "Normal";
	}
}
