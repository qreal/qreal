#include "plainTextEditPropertyManager.h"

using namespace Ui::WidgetsEdit;

PlainTextEditPropertyManager::PlainTextEditPropertyManager(PlainTextEdit *plainTextEdit)
	: AbstractScrollAreaPropertyManager(plainTextEdit)
{
	mPlainTextEdit = dynamic_cast<QPlainTextEdit *>(mTool->widget());
	if (!mPlainTextEdit)
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

void PlainTextEditPropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mPlainTextEdit || !containsProperty(name))
	{
		AbstractScrollAreaPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "backgroundVisible")
	{
		mPlainTextEdit->setBackgroundVisible(value.toBool());
	}
	if (name == "centerOnScroll")
	{
		mPlainTextEdit->setCenterOnScroll(value.toBool());
	}
	if (name == "cursorWidth")
	{
		mPlainTextEdit->setCursorWidth(value.toInt());
	}
	if (name == "overwriteMode")
	{
		mPlainTextEdit->setOverwriteMode(value.toBool());
	}
	if (name == "plainText")
	{
		mPlainTextEdit->setPlainText(value.toString());
	}
	if (name == "readOnly")
	{
		mPlainTextEdit->setReadOnly(value.toBool());
	}
	if (name == "undoRedoEnabled")
	{
		mPlainTextEdit->setUndoRedoEnabled(value.toBool());
	}
}

QVariant PlainTextEditPropertyManager::propertyValue(const QString &name)
{
	if (!mPlainTextEdit || !containsProperty(name))
	{
		return AbstractScrollAreaPropertyManager::propertyValue(name);
	}
	if (name == "backgroundVisible")
	{
		return QVariant(mPlainTextEdit->backgroundVisible());
	}
	if (name == "centerOnScroll")
	{
		return QVariant(mPlainTextEdit->centerOnScroll());
	}
	if (name == "cursorWidth")
	{
		return QVariant(mPlainTextEdit->cursorWidth());
	}
	if (name == "overwriteMode")
	{
		return QVariant(mPlainTextEdit->overwriteMode());
	}
	if (name == "plainText")
	{
		return QVariant(mPlainTextEdit->toPlainText());
	}
	if (name == "readOnly")
	{
		return QVariant(mPlainTextEdit->isReadOnly());
	}
	if (name == "undoRedoEnabled")
	{
		return QVariant(mPlainTextEdit->isUndoRedoEnabled());
	}
	return QVariant();
}

void PlainTextEditPropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("backgroundVisible", QVariant::Bool);
	mProperties << QPair<QString, int>("centerOnScroll", QVariant::Bool);
	mProperties << QPair<QString, int>("cursorWidth", QVariant::Int);
	mProperties << QPair<QString, int>("overwiteMode", QVariant::Bool);
	mProperties << QPair<QString, int>("plainText", QVariant::String);
	mProperties << QPair<QString, int>("readOnly", QVariant::Bool);
	mProperties << QPair<QString, int>("undoRedoEnabled", QVariant::Bool);
}

bool PlainTextEditPropertyManager::containsProperty(const QString &name)
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
