#include "propertyEditor.h"

using namespace qReal::widgetsEdit;

PropertyEditor::PropertyEditor(QWidget *editor)
	: mEditor(editor)
{
}

NodeElement *PropertyEditor::getNode() const
{
	QWidget *root = mEditor;
	while (root && !root->graphicsProxyWidget()) {
		root = root->parentWidget();
	}
	return root ? dynamic_cast<NodeElement *>(root->graphicsProxyWidget()) : NULL;
}

QString PropertyEditor::binding() const
{
	return mPropertyName;
}

QString PropertyEditor::propertyName() const
{
	return mPropertyName;
}

void PropertyEditor::setPropertyName(QString const &propertyName)
{
	mPropertyName = propertyName;
}

QString PropertyEditor::value() const
{
	return mValue;
}

void PropertyEditor::setValue(const QString &value)
{
	mValue = value;
}
