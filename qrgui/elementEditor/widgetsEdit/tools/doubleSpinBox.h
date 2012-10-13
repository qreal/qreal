#pragma once

#include <QtGui/QDoubleSpinBox>

#include "abstractSpinBox.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class DoubleSpinBox : public AbstractSpinBox
{
	Q_OBJECT

	Q_PROPERTY(double maximum READ maximum WRITE setMaximum USER true DESIGNABLE true)
	Q_PROPERTY(double minimum READ minimum WRITE setMinimum USER true DESIGNABLE true)
	Q_PROPERTY(QString prefix READ prefix WRITE setPrefix USER true DESIGNABLE true)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix USER true DESIGNABLE true)
	Q_PROPERTY(double singleStep READ singleStep WRITE setSingleStep USER true DESIGNABLE true)
	Q_PROPERTY(double value READ value WRITE setValue USER true DESIGNABLE true)
	Q_PROPERTY(QString bindedPropertyName READ bindedPropertyName WRITE setBindedPropertyName USER true DESIGNABLE true)

public:
	DoubleSpinBox(ToolController *controller);

private slots:
	void valueChanged(double d);

private:
	double maximum() const;
	double minimum() const;
	QString prefix() const;
	QString suffix() const;
	double singleStep() const;
	double value() const;
	QString bindedPropertyName() const;

	void setMaximum(double maximum);
	void setMinimum(double minimum);
	void setPrefix(QString const &prefix);
	void setSuffix(QString const &suffix);
	void setSingleStep(double step);
	void setValue(double value);
	void setBindedPropertyName(QString const &name);

	QDoubleSpinBox *mDoubleSpinBox;
	QString mBindedPropertyName;
};

}
}
