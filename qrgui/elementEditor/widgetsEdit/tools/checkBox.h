#pragma once

#include <QtGui/QCheckBox>

#include "abstractButton.h"
#include "../private/toolController.h"
#include "propertyEditor.h"

namespace qReal
{
namespace widgetsEdit
{

class CheckBoxWidget : public QCheckBox, public PropertyEditor
{
	Q_OBJECT

public:
	explicit CheckBoxWidget(QString const &title);
	virtual ~CheckBoxWidget() {}

	bool isChecked() const;
	virtual void setPropertyValue(QString const &value);

private slots:
	void onStateChanged(int state);

};

class CheckBox : public AbstractButton
{
	Q_OBJECT

	Q_PROPERTY(bool checked READ isChecked WRITE setChecked USER true DESIGNABLE true)
	Q_PROPERTY(QString binding READ propertyName WRITE setPropertyName USER true DESIGNABLE true)
	BINDING_TOOL(mCheckBox)

public:
	explicit CheckBox(ToolController *controller);

private slots:
	void toggled(bool isChecked);

private:
	bool isChecked() const;
	void setChecked(bool checked);

	CheckBoxWidget *mCheckBox;
};

}
}
