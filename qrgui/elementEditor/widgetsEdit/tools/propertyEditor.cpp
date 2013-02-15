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

void PropertyEditor::setPropertyValue(QVariant const &value)
{
	mValue = value.toString();
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

void PropertyEditor::setValueInRepo(QString const &value)
{
	NodeElement *node = getNode();
	if (node && !propertyName().isEmpty()) {
		node->setLogicalProperty(propertyName(), value);
	}
}

void PropertyEditor::setEnumValues(const QStringList &values)
{
	mEnumValues = values;
}

QStringList PropertyEditor::enumValues() const
{
	return mEnumValues;
}

bool PropertyEditor::isEnumEditor() const
{
	return !mEnumValues.isEmpty();
}
