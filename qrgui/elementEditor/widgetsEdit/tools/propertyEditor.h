#pragma once

#include <QtCore/QString>
#include <QtGui/QWidget>

#include "../../../umllib/nodeElement.h"
#include "../../../editorPluginInterface/propertyEditorInterface.h"

namespace qReal
{
namespace widgetsEdit
{

class PropertyEditor : public PropertyEditorInterface
{
public:
	virtual ~PropertyEditor() {}

	virtual QString binding() const;
	QString propertyName() const;
	void setPropertyName(QString const &propertyName);

	virtual QString value() const;
	virtual void setValue(QString const &value);

protected:
	explicit PropertyEditor(QWidget *editor);
	NodeElement *getNode() const;

private:
	QWidget *mEditor;
	QString mPropertyName;
	QString mValue;
};

}
}
