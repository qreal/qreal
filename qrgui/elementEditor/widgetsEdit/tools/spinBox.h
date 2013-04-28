#pragma once

#include <QtWidgets/QSpinBox>

#include "abstractSpinBox.h"
#include "../private/toolController.h"
#include "propertyEditor.h"

namespace qReal
{
namespace widgetsEdit
{

class SpinBoxWidget : public QSpinBox, public PropertyEditor
{
	Q_OBJECT

public:
	SpinBoxWidget();
	virtual ~SpinBoxWidget() {}

	virtual void setPropertyValue(QVariant const &value);

private slots:
	void onThisValueChanged(int value);
};

class SpinBoxProxy : public AbstractSpinBoxProxy
{
	Q_OBJECT

	Q_PROPERTY(int maximum READ maximum WRITE setMaximum USER true DESIGNABLE true)
	Q_PROPERTY(int minimum READ minimum WRITE setMinimum USER true DESIGNABLE true)
	Q_PROPERTY(QString prefix READ prefix WRITE setPrefix USER true DESIGNABLE true)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix USER true DESIGNABLE true)
	Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep USER true DESIGNABLE true)
	Q_PROPERTY(int value READ value WRITE setValue USER true DESIGNABLE true)
	Q_PROPERTY(QString binding READ propertyName WRITE setPropertyName USER true DESIGNABLE true)
	BINDING_TOOL(mSpinBox)

public:
	explicit SpinBoxProxy(SpinBoxWidget *spinBox);

private slots:
	int maximum() const;
	int minimum() const;
	QString prefix() const;
	QString suffix() const;
	int singleStep() const;
	int value() const;

	void setMaximum(int maximum);
	void setMinimum(int minimum);
	void setPrefix(QString const &prefix);
	void setSuffix(QString const &suffix);
	void setSingleStep(int step);
	void setValue(int value);

	void valueChanged(int i);

private:
	SpinBoxWidget *mSpinBox;
};

class SpinBox : public AbstractSpinBox
{
	Q_OBJECT

public:
	explicit SpinBox(ToolController *controller);

private:
	SpinBoxWidget *mSpinBox;
};

}
}
