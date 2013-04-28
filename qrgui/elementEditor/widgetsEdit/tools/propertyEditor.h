#pragma once

#include <QtCore/QString>
#include <QtWidgets/QWidget>

#include "../../../umllib/nodeElement.h"
#include "../../../editorPluginInterface/propertyEditorInterface.h"

namespace qReal
{
namespace widgetsEdit
{

// Cannot insert Q_PROPERTY(binding ...) declaration here;
// probably because Q_PROPERTY is a macros too and it does
// not notice declaration under macros
#define BINDING_TOOL(x) \
private: \
	QString propertyName() const \
	{ \
		return x->propertyName(); \
	} \
	void setPropertyName(QString const &propertyName) \
	{ \
		x->setPropertyName(propertyName); \
	} \

class PropertyEditor : public PropertyEditorInterface
{
public:
	virtual ~PropertyEditor() {}

	virtual void setPropertyValue(QVariant const &value);
	virtual QString binding() const;
	virtual void setEnumValues(QStringList const &values);
	QString propertyName() const;
	void setPropertyName(QString const &propertyName);
	void setValueInRepo(QVariant const &value);

protected:
	explicit PropertyEditor(QWidget *editor);
	NodeElement *getNode() const;
	QStringList enumValues() const;
	bool isEnumEditor() const;

private:
	QWidget *mEditor;
	QString mValue;
	QString mPropertyName;
	QStringList mEnumValues;
};

}
}
