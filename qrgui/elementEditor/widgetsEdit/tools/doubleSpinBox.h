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

	Q_PROPERTY(double maximum READ maximum WRITE setMaximum USER true)
	Q_PROPERTY(double minimum READ minimum WRITE setMinimum USER true)
	Q_PROPERTY(QString prefix READ prefix WRITE setPrefix USER true)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix USER true)
	Q_PROPERTY(double singleStep READ singleStep WRITE setSingleStep USER true)
	Q_PROPERTY(double value READ value WRITE setValue USER true)

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

	void setMaximum(double maximum);
	void setMinimum(double minimum);
	void setPrefix(QString const &prefix);
	void setSuffix(QString const &suffix);
	void setSingleStep(double step);
	void setValue(double value);

	QDoubleSpinBox *mDoubleSpinBox;

};

}
}
