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
	void setCheckedState(bool checked);
	virtual void setValue(QString const &value);

private slots:
	void onStateChanged(int state);

};

class CheckBox : public AbstractButton
{
	Q_OBJECT

	Q_PROPERTY(bool checked READ isChecked WRITE setChecked USER true DESIGNABLE true)
	Q_PROPERTY(QString bindedPropertyName READ bindedPropertyName WRITE setBindedPropertyName USER true DESIGNABLE true)

public:
	explicit CheckBox(ToolController *controller);

private slots:
	void toggled(bool isChecked);

private:
	bool isChecked() const;
	QString bindedPropertyName() const;

	void setChecked(bool checked);
	void setBindedPropertyName(QString const &name);

	CheckBoxWidget *mCheckBox;
	QString mBindedPropertyName;
};

}
}
